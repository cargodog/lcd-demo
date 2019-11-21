# Subdirectory make files

# Subdirectory source files
list(APPEND kinets_firmware_SOURCES
"${CMAKE_CURRENT_LIST_DIR}/microcli/microcli.c"
)

# Subdirectory include paths
list(APPEND kinets_firmware_INCLUDES
"${CMAKE_CURRENT_LIST_DIR}/"
"${CMAKE_CURRENT_LIST_DIR}/microcli/"
)