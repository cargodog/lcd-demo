/*
 * QSPI flash driver
 * 
 * The Kinetis MCUs provide a QSPI memory controller to interface with an
 * external flash chip. When configured, this controller will cache data from
 * the flash chip, for easy access via the system memory-map (via AHB). This
 * driver will configure the QSPI peripheral to access a flash chip on either
 * QSPI0 or QSPI1 (user configurable, via `QSPI_PERIPHERAL).
 * 
 * Data can be written to the flash via the driver API, however reads should be
 * performed by accessing the memory-mapped region. The start of the memory
 * mapped region is `QSPI_FLASH_BASE_ADDR`, and maps to the beginning of the
 * flash chip.
 */

#ifndef _QSPI_FLASH_H_
#define _QSPI_FLASH_H_

#include <stdint.h>

// User config. Configure these according to your hardware configuration.
#define QSPI_PERIPHERAL         QuadSPI0
#define QSPI0_CLK_FREQ          CLOCK_GetFreq(kCLOCK_McgPll0Clk)
#define FLASH_PAGE_SIZE         (256)
#define FLASH_SECTOR_SIZE       (4096)
#define FLASH_CHIP_SIZE         (512 * FLASH_SECTOR_SIZE)
#define FLASH_ENABLE_QUAD_CMD   (0x40)


// Memory address to AHB mapped flash
#define QSPI_FLASH_BASE_ADDR    ((uint8_t *)FSL_FEATURE_QSPI_AMBA_BASE)

// Initialize the interface
void qspi_flash_init(void);

// Erase the entire chip
void qspi_flash_erase_chip(void);

// Erase N sectors, starting at `addr`. Note, `addr` MUST be sector aligned.
void qspi_flash_erase_sectors(void * addr, uint32_t count);

// Erase data at the specified address. (unaligned address is allowed)
void qspi_flash_erase_data(void * addr, uint32_t len);

// Program data at the specified address. (unaligned address is allowed)
// Note, this does not erase the sector, only performs a simple program.
// If data on the flash is already 0, this command cannot change those bits.
void qspi_flash_program_data(void * dest, const void * src, uint32_t len);

// Overwrite data at the specified address. (unaligned address is allowed)
// This will perform a full over-write, by read/modify/write of each sector.
void qspi_flash_overwrite_data(void * dest, const void * src, uint32_t len);


#ifdef QSPI_FLASH_TEST
void           qspi_flash_test(void);
#endif

#endif /* _QSPI_FLASH_H_ */
