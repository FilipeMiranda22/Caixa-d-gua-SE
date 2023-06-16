# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/wesqu/esp/esp-idf/components/bootloader/subproject"
  "C:/Caixa-d-gua-SE/codigo_esp32/build/bootloader"
  "C:/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix"
  "C:/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/tmp"
  "C:/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src"
  "C:/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
