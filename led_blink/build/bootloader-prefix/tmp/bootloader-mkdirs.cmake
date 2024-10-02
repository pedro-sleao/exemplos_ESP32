# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/pedro/esp/esp-idf/components/bootloader/subproject"
  "/home/pedro/Documentos/ESP32/led_blink/build/bootloader"
  "/home/pedro/Documentos/ESP32/led_blink/build/bootloader-prefix"
  "/home/pedro/Documentos/ESP32/led_blink/build/bootloader-prefix/tmp"
  "/home/pedro/Documentos/ESP32/led_blink/build/bootloader-prefix/src/bootloader-stamp"
  "/home/pedro/Documentos/ESP32/led_blink/build/bootloader-prefix/src"
  "/home/pedro/Documentos/ESP32/led_blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/pedro/Documentos/ESP32/led_blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/pedro/Documentos/ESP32/led_blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
