# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/filip/esp/esp-idf/components/bootloader/subproject"
  "D:/Documentos/ESP-32/SE/Caixa-d-gua-SE/codigo_esp32/build/bootloader"
  "D:/Documentos/ESP-32/SE/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix"
  "D:/Documentos/ESP-32/SE/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/tmp"
  "D:/Documentos/ESP-32/SE/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Documentos/ESP-32/SE/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src"
  "D:/Documentos/ESP-32/SE/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Documentos/ESP-32/SE/Caixa-d-gua-SE/codigo_esp32/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
