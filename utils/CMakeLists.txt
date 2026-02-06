aux_source_directory(. DIR_utils_SRCS)

include_directories(../lib/Config)
include_directories(../lib/GUI)
include_directories(../lib/e-Paper)

# Generate the link library
add_library(utils ${DIR_utils_SRCS})
target_link_libraries(utils 
    PUBLIC 
    Config 
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
)