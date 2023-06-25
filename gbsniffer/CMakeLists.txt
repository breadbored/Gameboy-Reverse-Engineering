#
# Pi Pico lib
#

cmake_minimum_required(VERSION 3.22)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(../pico-sdk/pico_sdk_init.cmake)

project(gbsniffer)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(gbsniffer
    gbsniffer.cpp
)

pico_enable_stdio_usb(gbsniffer 1)
pico_enable_stdio_uart(gbsniffer 0)
target_compile_definitions(gbsniffer PRIVATE
PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=10000
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(gbsniffer pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(gbsniffer)
