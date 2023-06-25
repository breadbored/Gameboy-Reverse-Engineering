#
# Pi Pico lib
#

cmake_minimum_required(VERSION 3.22)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(../pico-sdk/pico_sdk_init.cmake)

project(gbtrader)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(gbtrader
    gbtrader.cpp
)

pico_enable_stdio_usb(gbtrader 1)
pico_enable_stdio_uart(gbtrader 0)
target_compile_definitions(gbtrader PRIVATE
PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=10000
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(gbtrader pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(gbtrader)
