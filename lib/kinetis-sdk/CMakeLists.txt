# Subdirectory make files

# Subdirectory source files
list(APPEND kinets_firmware_SOURCES
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/devices/MK82F25615/drivers/fsl_clock.c"
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/devices/MK82F25615/drivers/fsl_gpio.c"
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/devices/MK82F25615/drivers/fsl_lpuart.c"
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/devices/MK82F25615/drivers/fsl_qspi.c"
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/devices/MK82F25615/system_MK82F25615.c"
)

# Subdirectory include paths
list(APPEND kinets_firmware_INCLUDES
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/CMSIS/Include"
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/devices/MK82F25615"
"${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk-mk8x/devices/MK82F25615/drivers"
)
