# Subdirectory make files
include("${CMAKE_CURRENT_LIST_DIR}/board/CMakeLists.txt")

# Subdirectory source files
list(APPEND kinets_firmware_SOURCES
"${CMAKE_CURRENT_LIST_DIR}/assert.c"
"${CMAKE_CURRENT_LIST_DIR}/debug_console.c"
"${CMAKE_CURRENT_LIST_DIR}/led_blink.c"
"${CMAKE_CURRENT_LIST_DIR}/main.c"
"${CMAKE_CURRENT_LIST_DIR}/timer_utils.c"
"${CMAKE_CURRENT_LIST_DIR}/startup_MK82F25615.S"
)

# Subdirectory include paths
list(APPEND kinets_firmware_INCLUDES
"${CMAKE_CURRENT_LIST_DIR}"
)
