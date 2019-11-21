# Subdirectory make files
include("${CMAKE_CURRENT_LIST_DIR}/kinetis-sdk/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/microcli/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/printf/CMakeLists.txt")

# Subdirectory source files
list(APPEND kinets_firmware_SOURCES
)

# Subdirectory include paths
list(APPEND kinets_firmware_INCLUDES
"${CMAKE_CURRENT_LIST_DIR}"
)
