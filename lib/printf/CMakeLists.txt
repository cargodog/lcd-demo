# Subdirectory make files

# Subdirectory source files
list(APPEND kinets_firmware_SOURCES
"${CMAKE_CURRENT_LIST_DIR}/printf_port.c"
"${CMAKE_CURRENT_LIST_DIR}/printf/printf.c"
)

# Subdirectory include paths
list(APPEND kinets_firmware_INCLUDES
"${CMAKE_CURRENT_LIST_DIR}/"
"${CMAKE_CURRENT_LIST_DIR}/printf/"
)
