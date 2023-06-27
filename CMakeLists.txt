#
# Pi Pico lib
#

cmake_minimum_required(VERSION 3.22)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(gbtoolkit)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


#
# Gameboy Printer Executable
#
add_subdirectory(gbprinter)

#
# Gameboy Pokemon Trader Gen 1 Executable
#
add_subdirectory(gbtrader)

#
# Gameboy Serial Tester Executable
#
add_subdirectory(gbserial)

#
# Gameboy Infrared Executable
#
add_subdirectory(gbinfrared)

#
# Gameboy Sniffer Executable
#
add_subdirectory(gbsniffer)

#
# Gameboy All-In-One Tool (Paired with desktop_client)
#
add_subdirectory(gball)

#
# JoyBus
#
add_subdirectory(joybus)

#
# Famicom Keyboard
#
add_subdirectory(famicomkeyboard)

#
# SNES Controller Test
#
add_subdirectory(snescontroller)

#
# HID Test
#
add_subdirectory(HID)

#
# HID Test
#
add_subdirectory(snesHID)
