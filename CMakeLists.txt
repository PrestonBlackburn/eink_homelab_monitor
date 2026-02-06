cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(Pico_ePaper_Code)

pico_sdk_init()

# include(pico_extras_import.cmake)
set(PICO_EXTRAS_PATH /home/prestonb/hardware_projects/pico-extras)	# <==== Change the path to the pico-extras folder here
include(${PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)

include(wifi_config.cmake OPTIONAL)

# add a compilation subdirectory
add_subdirectory(lib/Config)
add_subdirectory(lib/e-Paper)
add_subdirectory(lib/Fonts)
add_subdirectory(lib/GUI)
add_subdirectory(utils)

# add a header directory
include_directories(./utils)
include_directories(./lib/Config)
include_directories(./lib/GUI)
include_directories(./lib/utils)

# generate an executable file
add_executable(eink_monitor 
main.c
)

target_compile_definitions(eink_monitor PRIVATE
	WIFI_SSID=\"${WIFI_SSID}\"
	WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
)

# enable usb output, disable uart output
pico_enable_stdio_usb(eink_monitor 1)
pico_enable_stdio_uart(eink_monitor 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(eink_monitor)

target_include_directories(eink_monitor PRIVATE ${CMAKE_CURRENT_LIST_DIR}) 

target_link_libraries(eink_monitor 
    utils 
    ePaper 
    GUI 
    Fonts 
    Config 
    pico_stdlib 
    hardware_spi

	pico_sleep

	hardware_uart
	hardware_rtc
	hardware_clocks
	hardware_sync

	pico_cyw43_arch_lwip_threadsafe_background
	pico_lwip_http
)