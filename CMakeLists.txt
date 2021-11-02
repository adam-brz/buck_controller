cmake_minimum_required(VERSION 3.12)
project(buck_controller)
enable_language(C)

set(F_CPU 16000000UL)
set(MCU atmega328p)
set(PROG_TYPE usbasp)

include(cmake/compiler.cmake)

file(GLOB SRC_FILES "src/*.c")
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME}.elf)

add_custom_target(strip ALL avr-strip ${CMAKE_PROJECT_NAME}.elf DEPENDS ${CMAKE_PROJECT_NAME})
add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex DEPENDS strip)
add_custom_target(eeprom avr-objcopy -j .eeprom  --set-section-flags=.eeprom="alloc,load"  --change-section-lma .eeprom=0 -O ihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.eep DEPENDS strip)

add_custom_target(upload avrdude -c ${PROG_TYPE} -p ${MCU} -U flash:w:${CMAKE_PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(upload_eeprom avrdude -c ${PROG_TYPE} -p ${MCU} -U eeprom:w:${CMAKE_PROJECT_NAME}.eep DEPENDS eeprom)
add_custom_target(fuses avrdude -c ${PROG_TYPE} -p ${MCU} -U lfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U lock:w:${LOCK_BIT}:m )

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_PROJECT_NAME}.hex;${CMAKE_PROJECT_NAME}.eeprom;${CMAKE_PROJECT_NAME}.lst")
