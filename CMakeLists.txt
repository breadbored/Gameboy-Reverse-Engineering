#
# Pi Pico lib
#

cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(gbtoolkit)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


#
# Gameboy Printer Executable
#

add_executable(gbprinter
    gbprinter/gbprinter.cpp
)

pico_enable_stdio_usb(gbprinter 1)
pico_enable_stdio_uart(gbprinter 0)
target_compile_definitions(gbprinter PRIVATE
PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=10000
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(gbprinter pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(gbprinter)


#
# Gameboy Pokemon Trader Gen 1 Executable
#

add_executable(gbtrader
    gbtrader/gbtrader.cpp
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


#
# Gameboy Serial Tester Executable
#

add_executable(gbserial
    gbserial/gbserial.cpp
)

pico_enable_stdio_usb(gbserial 1)
pico_enable_stdio_uart(gbserial 0)
target_compile_definitions(gbserial PRIVATE
PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=10000
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(gbserial pico_stdlib hardware_spi)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(gbserial)


#
# Gameboy Infrared Executable
#

add_executable(gbinfrared
    gbinfrared/gbinfrared.cpp
)

pico_enable_stdio_usb(gbinfrared 1)
pico_enable_stdio_uart(gbinfrared 0)
target_compile_definitions(gbinfrared PRIVATE
PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=10000
)


#
# Gameboy Sniffer Executable
#

add_executable(gbsniffer
    gbsniffer/gbsniffer.cpp
)

pico_enable_stdio_usb(gbsniffer 1)
pico_enable_stdio_uart(gbsniffer 0)
target_compile_definitions(gbsniffer PRIVATE
PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=10000
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(gbsniffer pico_stdlib hardware_spi)
target_include_directories(gbsniffer PRIVATE libgb)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(gbsniffer)