# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/1_hDung/Espressif/frameworks/esp-idf-master/components/bootloader/subproject")
  file(MAKE_DIRECTORY "D:/1_hDung/Espressif/frameworks/esp-idf-master/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader"
  "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader-prefix"
  "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader-prefix/tmp"
  "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader-prefix/src/bootloader-stamp"
  "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader-prefix/src"
  "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/1_hDung/Git_Repo/Test_Code/ESP32/uart/uart_events/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
