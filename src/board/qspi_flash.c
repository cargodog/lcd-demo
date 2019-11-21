#include "qspi_flash.h"
#include <fsl_qspi.h>
#include <MK82F25615_features.h>
#include <fsl_clock.h>
#include <stdint.h>

// Status register bit-masks
#define FLASH_SR_BIT_MASK_WRITE_IN_PROG     (0b00000001)
#define FLASH_SR_BIT_MASK_WRITE_EN_LATCH    (0b00000010)
#define FLASH_SR_BIT_MASK_BLOCK_PROTECT     (0b00111100)
#define FLASH_SR_BIT_MASK_QUAD_ENABLE       (0b01000000)
#define FLASH_SR_BIT_MASK_PROTECT           (0b10000000)

// QSPI LUT commands
#define QSPI_LUT_QUAD_READ          (0)
#define QSPI_LUT_WRITE_EN           (4)
#define QSPI_LUT_CHIP_ERASE         (8)
#define QSPI_LUT_READ_STAT          (12)
#define QSPI_LUT_PAGE_PROG          (16)
#define QSPI_LUT_WRITE_REG          (20)
#define QSPI_LUT_READ_CFG           (24)
#define QSPI_LUT_ERASE_SECTOR       (28)

static qspi_flash_config_t flashConfig = {
    .flashA1Size = FLASH_CHIP_SIZE,
    .flashA2Size = 0,
    #if defined(FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE) && (FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE)
    .flashB1Size = FLASH_CHIP_SIZE,
    .flashB2Size = 0,
    #endif
    .dataHoldTime = 0,
    .CSHoldTime = 0,
    .CSSetupTime = 0,
    .cloumnspace = 0,
    .dataLearnValue = 0,
    .endian = kQSPI_64LittleEndian,
    .enableWordAddress = false,
    .lookuptable = {
        /* Seq0 :Quad Read */
        /* CMD:        0xEB - Quad Read, Single pad */
        /* ADDR:       0x18 - 24bit address, Quad pads */
        /* DUMMY:      0x06 - 6 clock cyles, Quad pads */
        /* READ:       0x80 - Read 128 bytes, Quad pads */
        /* JUMP_ON_CS: 0 */
        [0] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0xEB, QSPI_ADDR, QSPI_PAD_4, 0x18),
        [1] = QSPI_LUT_SEQ(QSPI_DUMMY, QSPI_PAD_4, 0x06, QSPI_READ, QSPI_PAD_4, 0x80),
        [2] = QSPI_LUT_SEQ(QSPI_JMP_ON_CS, QSPI_PAD_1, 0x0, 0, 0, 0),

        /* Seq1: Write Enable */
        /* CMD:      0x06 - Write Enable, Single pad */
        [4] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x06, 0, 0, 0),

        /* Seq2: Erase All */
        /* CMD:    0x60 - Erase All chip, Single pad */
        [8] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x60, 0, 0, 0),

        /* Seq3: Read Status */
        /* CMD:    0x05 - Read Status, single pad */
        /* READ:   0x01 - Read 1 byte */
        [12] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x05, QSPI_READ, QSPI_PAD_1, 0x1),

        /* Seq4: Page Program */
        /* CMD:    0x02 - Page Program, Single pad */
        /* ADDR:   0x18 - 24bit address, Single pad */
        /* WRITE:  0x80 - Write 128 bytes at one pass, Single pad */
        [16] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x02, QSPI_ADDR, QSPI_PAD_1, 0x18),
        [17] = QSPI_LUT_SEQ(QSPI_WRITE, QSPI_PAD_1, 0x80, 0, 0, 0),

        /* Seq5: Write Register */
        /* CMD:    0x01 - Write Status Register, single pad */
        /* WRITE:  0x01 - Write 1 byte of data, single pad */
        [20] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x01, QSPI_WRITE, QSPI_PAD_1, 0x1),

        /* Seq6: Read Config Register */
        /* CMD:  0x15 - Read Config register, single pad */
        /* READ: 0x01 - Read 1 byte */
        [24] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x15, QSPI_READ, QSPI_PAD_1, 0x1),

        /* Seq7: Erase Sector */
        /* CMD:  0x20 - Sector Erase, single pad */
        /* ADDR: 0x18 - 24 bit address, single pad */
        [28] = QSPI_LUT_SEQ(QSPI_CMD, QSPI_PAD_1, 0x20, QSPI_ADDR, QSPI_PAD_1, 0x18),

        /* Match MISRA rule */
        [63] = 0
    }
};
uint8_t pageBuffer[FLASH_PAGE_SIZE];
uint8_t sectorBuffer[FLASH_SECTOR_SIZE];


static inline void wait_while_qspi_busy(void)
{
    while(QSPI_GetStatusFlags(QSPI_PERIPHERAL) & (kQSPI_Busy | kQSPI_IPAccess) );   
}

static inline void wait_while_flash_busy(void)
{
    uint32_t val = 0;

    // Check WIP bit
    do {
        // Refresh data
        wait_while_qspi_busy();
        QSPI_ClearFifo(QSPI_PERIPHERAL, kQSPI_RxFifo);
        QSPI_ExecuteIPCommand(QSPI_PERIPHERAL, QSPI_LUT_READ_STAT);

        // Read status byte
        wait_while_qspi_busy();
        val = QSPI_PERIPHERAL->RBDR[0];

        // Clear ARDB area
        QSPI_ClearErrorFlag(QSPI_PERIPHERAL, kQSPI_RxBufferDrain);
    } while (val & FLASH_SR_BIT_MASK_WRITE_IN_PROG);
}

static inline void cmd_write_enable(void)
{
    wait_while_qspi_busy();
    QSPI_ExecuteIPCommand(QSPI_PERIPHERAL, QSPI_LUT_WRITE_EN);
}

static inline void enable_quad_mode(void)
{
    uint32_t val[4] = {FLASH_ENABLE_QUAD_CMD, 0, 0, 0};

    wait_while_qspi_busy();
    QSPI_SetIPCommandAddress(QSPI_PERIPHERAL, FSL_FEATURE_QSPI_AMBA_BASE);

    // Clear Tx FIFO
    QSPI_ClearFifo(QSPI_PERIPHERAL, kQSPI_TxFifo);

    // Write enable
    cmd_write_enable();

    // Write data into TX FIFO, needs to write at least 16 bytes of data
    QSPI_WriteBlocking(QSPI_PERIPHERAL, val, sizeof(val));

    // Set seq id, write register
    QSPI_ExecuteIPCommand(QSPI_PERIPHERAL, QSPI_LUT_WRITE_REG);

    // Wait until finished
    wait_while_flash_busy();
}

static inline void erase_sector(void * addr)
{
    assert((uint8_t *)addr >= QSPI_FLASH_BASE_ADDR);
    assert((uint8_t *)addr < QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE);

    // Wait for the QSPI peripheral to complete any previous operations
    wait_while_qspi_busy();

    // Stop any pending commands and enter command mode
    QSPI_ClearFifo(QSPI_PERIPHERAL, kQSPI_TxFifo);
    cmd_write_enable();

    // Erase a sector
    wait_while_qspi_busy();
    QSPI_SetIPCommandAddress(QSPI_PERIPHERAL, (uint32_t)addr);
    QSPI_ExecuteIPCommand(QSPI_PERIPHERAL, QSPI_LUT_ERASE_SECTOR);

    // Wait for operation to complete
    wait_while_flash_busy();

    #if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
    #endif
}

static inline void program_page(void * dest, const uint32_t * srcAddr)
{
    uint8_t * destAddr = (uint8_t *)dest;
    
    assert(destAddr >= QSPI_FLASH_BASE_ADDR);
    assert(destAddr < QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE);

    static const uint32_t qspiFIFOSize = FSL_FEATURE_QSPI_TXFIFO_DEPTH * sizeof(uint32_t);

    // Wait for the QSPI peripheral to complete any previous operations
    wait_while_qspi_busy();
    QSPI_ClearFifo(QSPI_PERIPHERAL, kQSPI_TxFifo);

    QSPI_SetIPCommandAddress(QSPI_PERIPHERAL, (uint32_t)destAddr);
    cmd_write_enable();
    wait_while_qspi_busy();

    // First write some data into TXFIFO to prevent from underrun
    QSPI_WriteBlocking(QSPI_PERIPHERAL, (uint32_t *) srcAddr, qspiFIFOSize);
    srcAddr += FSL_FEATURE_QSPI_TXFIFO_DEPTH;

    // Start the program
    QSPI_SetIPCommandSize(QSPI_PERIPHERAL, FLASH_PAGE_SIZE);
    QSPI_ExecuteIPCommand(QSPI_PERIPHERAL, QSPI_LUT_PAGE_PROG);

    // Write the remaining data
    QSPI_WriteBlocking(QSPI_PERIPHERAL, (uint32_t *) srcAddr, FLASH_PAGE_SIZE - qspiFIFOSize);
    srcAddr += (FLASH_PAGE_SIZE - qspiFIFOSize) / sizeof(uint32_t);

    // Wait until flash finished program
    wait_while_flash_busy();
    wait_while_qspi_busy();

    #if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
    #endif
}

void qspi_flash_init(void)
{
    static qspi_config_t qspiConfig = {0};

    // Configure QSPI peripheral to make the flash chip available
    // via the AHB memory region

    // Get QSPI default settings and configure the qspi
    QSPI_GetDefaultQspiConfig(&qspiConfig);

    // Set AHB buffer size for reading data through AHB bus
    if (FLASH_PAGE_SIZE <= FSL_FEATURE_QSPI_AHB_BUFFER_SIZE) {
        qspiConfig.AHBbufferSize[3] = FLASH_PAGE_SIZE;
    } else {
        qspiConfig.AHBbufferSize[3] = FSL_FEATURE_QSPI_AHB_BUFFER_SIZE;
    }

    // Apply the configuration to the QSPI peripheral
    QSPI_Init(QSPI_PERIPHERAL, &qspiConfig, QSPI0_CLK_FREQ);

    // Set the configuration for the attached flash chip
    QSPI_SetFlashConfig(QSPI_PERIPHERAL, &flashConfig);

    #if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(base);
    #endif

    // Configure the flash chip for QuadSPI operation
    enable_quad_mode();
}

void qspi_flash_erase_chip(void)
{
    // Wait for the QSPI peripheral to complete any previous operations
    wait_while_qspi_busy();

    // Enable flash commands
    QSPI_ClearFifo(QSPI_PERIPHERAL, kQSPI_TxFifo);
    cmd_write_enable();

    // Execute chip erase command
    QSPI_SetIPCommandAddress(QSPI_PERIPHERAL, (uint32_t)QSPI_FLASH_BASE_ADDR);
    QSPI_ExecuteIPCommand(QSPI_PERIPHERAL, QSPI_LUT_CHIP_ERASE);

    // Wait for operation to complete
    wait_while_flash_busy();

    #if defined(FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC) && (FSL_FEATURE_QSPI_SOCCR_HAS_CLR_LPCAC)
    QSPI_ClearCache(EXAMPLE_QSPI);
    #endif
}

void qspi_flash_erase_sectors(void * addr, uint32_t count)
{
    uint8_t * sectorAddr = (uint8_t *)addr;

    assert(sectorAddr >= QSPI_FLASH_BASE_ADDR);
    assert(sectorAddr < QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE);
    assert(sectorAddr + (FLASH_SECTOR_SIZE * count) < (QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE));

    while(count--) {
        // Erase the sector
        erase_sector(sectorAddr);

        // Increment to next sector address
        sectorAddr += FLASH_SECTOR_SIZE;
    }
}

void qspi_flash_erase_data(void * addr, uint32_t len)
{
    uint8_t * destAddr = (uint8_t *)addr;

    assert(destAddr >= QSPI_FLASH_BASE_ADDR);
    assert(destAddr < QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE);
    assert(destAddr + len < (QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE));

    uint8_t * sectorAddr = (uint8_t *)((uint32_t)destAddr & (UINT32_MAX - (FLASH_SECTOR_SIZE - 1)));

    // If not sector aligned, fill beginning of sector with existing data
    uint32_t i = 0;
    while(i < destAddr - sectorAddr) {
        sectorBuffer[i] = ((uint8_t *)sectorAddr)[i];
        i++;
    }

    // Clear the rest of the sector
    while(len--) {
        sectorBuffer[i++] = 0xFF;
        if(i >= FLASH_SECTOR_SIZE) {
            erase_sector(sectorAddr);
            i = 0;

            // Restore data that should not be erased
            if(sectorAddr < destAddr)
                qspi_flash_program_data(sectorAddr, sectorBuffer, sizeof(sectorBuffer));

            sectorAddr += FLASH_SECTOR_SIZE;
        }
    }

    // If not sector aligned, fill end of sector with existing data
    while(i > 0 && i < sizeof(sectorBuffer)) {
        sectorBuffer[i] = ((uint8_t *)sectorAddr)[i];
        i++;
    }

    if(i >= FLASH_SECTOR_SIZE) {
        erase_sector(sectorAddr);
        qspi_flash_program_data(sectorAddr, sectorBuffer, sizeof(sectorBuffer));
    }
}

void qspi_flash_program_data(void * dest, const void * src, uint32_t len)
{
    uint8_t * destAddr = (uint8_t *)dest;
    
    assert(destAddr >= QSPI_FLASH_BASE_ADDR);
    assert(destAddr < QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE);
    assert(destAddr + len < (QSPI_FLASH_BASE_ADDR + FLASH_CHIP_SIZE));

    const uint8_t * srcAddr = (const uint8_t *)src;
    uint8_t * pageAddr = (uint8_t *)((uint32_t)destAddr & (UINT32_MAX - (FLASH_PAGE_SIZE - 1)));

    #if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
    // Reduce frequency while clock divder is less than 2
    bool isDivNeedRestore = false;
    uint8_t qspiClockDiv = ((QSPI_PERIPHERAL->MCR & QuadSPI_MCR_SCLKCFG_MASK) >> QuadSPI_MCR_SCLKCFG_SHIFT) + 1U;
    if(qspiClockDiv == 1U) {
        // Reduce the frequency
        isDivNeedRestore = true;
        QSPI_Enable(QSPI_PERIPHERAL, false);
        QSPI_PERIPHERAL->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;
        QSPI_PERIPHERAL->MCR |= QuadSPI_MCR_SCLKCFG(1U);
        QSPI_Enable(QSPI_PERIPHERAL, true);
    }
    #endif

    // Pre-fill buffer if data is not page aligned
    uint32_t i = 0;
    while(i < destAddr - pageAddr)
        pageBuffer[i++] = 0xFF;
    
    // Fill buffer with data, and program a page at a time
    while(len--) {
        pageBuffer[i++] = *srcAddr++;
        if(i >= FLASH_PAGE_SIZE) {
            program_page(pageAddr, (uint32_t *)pageBuffer);
            i = 0;
            pageAddr += FLASH_PAGE_SIZE;
        }
    }

    // Pad the buffer if it is partially filled
    while(i > 0 && i < sizeof(pageBuffer)) 
        pageBuffer[i++] = 0xFF;

    // If there is still data to program, send it
    if(i >= FLASH_PAGE_SIZE)
        program_page(pageAddr, (uint32_t *)pageBuffer);

    #if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
    // Restore the frequency if needed
    if(isDivNeedRestore) {
        QSPI_Enable(QSPI_PERIPHERAL, false);
        QSPI_PERIPHERAL->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;
        QSPI_PERIPHERAL->MCR |= QuadSPI_MCR_SCLKCFG(0U);
        QSPI_Enable(QSPI_PERIPHERAL, true);
    }
    #endif
}

void qspi_flash_overwrite_data(void * dest, const void * src, uint32_t len)
{
    qspi_flash_erase_data(dest, len);
    qspi_flash_program_data(dest, src, len);
}

#ifdef QSPI_FLASH_TEST
#include "printf.h"
void qspi_flash_test(void)
{
    uint32_t i = 0;
    uint32_t buff[FLASH_SECTOR_SIZE / 4];

    // Test banner
    printf("\n\n\n\r");
    printf("-------------------- Flash Test Begin --------------------\n\r");

    // Initialize test data
    for (i = 0; i < sizeof(buff) / sizeof(buff[0]); i++) {
        buff[i] = i;
    }

    // Erase the first sector
    printf("Erase sector...");
    qspi_flash_erase_sectors(QSPI_FLASH_BASE_ADDR, 1);
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Program data
    printf("Program data...");
    const uint32_t off = 3; // Test data that is mis-aligned
    qspi_flash_program_data(QSPI_FLASH_BASE_ADDR + off, buff, sizeof(buff));
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Verify data
    printf("Verifying...");
    for (i = 0; i < sizeof(buff) / sizeof(buff[0]); i++) {
        if (*((uint32_t *)(QSPI_FLASH_BASE_ADDR + (4*i) + off)) != buff[i]) {
            printf( "Error! Data in %d is %d, but expected %d\r\n",
                    i, *((uint32_t *)(QSPI_FLASH_BASE_ADDR + (4*i) + off)), buff[i]);
        }
    }
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Program data
    printf("Erase first page of data...");
    qspi_flash_erase_data(QSPI_FLASH_BASE_ADDR + off, FLASH_PAGE_SIZE);
    memset(buff, 0xFF, FLASH_PAGE_SIZE); // Also erase in test data
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Verify data
    printf("Verifying...");
    for (i = 0; i < sizeof(buff) / sizeof(buff[0]); i++) {
        if (*((uint32_t *)(QSPI_FLASH_BASE_ADDR + (4*i) + off)) != buff[i]) {
            printf( "Error! Data in %d is %d, but expected %d\r\n",
                    i, *((uint32_t *)(QSPI_FLASH_BASE_ADDR + (4*i) + off)), buff[i]);
        }
    }
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Overwrite some data
    printf("Overwrite some data...");
    struct TestStruct {
        uint32_t a;
        uint8_t b;
        uint32_t c;
        uint16_t d;
    } testStruct = { .a = 7, .b = 3, .c =99, .d = 60000 };
    struct TestStruct * nvmStruct = (struct TestStruct *)(QSPI_FLASH_BASE_ADDR + 10000);
    qspi_flash_overwrite_data(nvmStruct, &testStruct, sizeof(testStruct));
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Verify data
    printf("Verifying...");
    if(testStruct.a != nvmStruct->a) {
        printf( "Error! Data in nvmStruct->a is %d, but expected %d\r\n",
                nvmStruct->a, testStruct.a);
    }
    if(testStruct.b != nvmStruct->b) {
        printf( "Error! Data in nvmStruct->b is %d, but expected %d\r\n",
                nvmStruct->b, testStruct.b);
    }
    if(testStruct.c != nvmStruct->c) {
        printf( "Error! Data in nvmStruct->c is %d, but expected %d\r\n",
                nvmStruct->c, testStruct.c);
    }
    if(testStruct.d != nvmStruct->d) {
        printf( "Error! Data in nvmStruct->d is %d, but expected %d\r\n",
                nvmStruct->d, testStruct.d);
    }
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Overwrite some data
    printf("Overwrite some data...");
    testStruct.a += 44;
    testStruct.b += 44;
    testStruct.c += 44;
    testStruct.d += 44;
    qspi_flash_overwrite_data(nvmStruct, &testStruct, sizeof(testStruct));
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Verify data
    printf("Verifying...");
    if(testStruct.a != nvmStruct->a) {
        printf( "Error! Data in nvmStruct->a is %d, but expected %d\r\n",
                nvmStruct->a, testStruct.a);
    }
    if(testStruct.b != nvmStruct->b) {
        printf( "Error! Data in nvmStruct->b is %d, but expected %d\r\n",
                nvmStruct->b, testStruct.b);
    }
    if(testStruct.c != nvmStruct->c) {
        printf( "Error! Data in nvmStruct->c is %d, but expected %d\r\n",
                nvmStruct->c, testStruct.c);
    }
    if(testStruct.d != nvmStruct->d) {
        printf( "Error! Data in nvmStruct->d is %d, but expected %d\r\n",
                nvmStruct->d, testStruct.d);
    }
    printf("\r\t\t\t\t\t\t     Done!\n\r");

    // Test footer
    printf("\n\r--------------------  Flash Test End  --------------------\n\r");
}
#endif
