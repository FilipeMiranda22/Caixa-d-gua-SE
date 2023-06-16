# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/wesqu/esp/esp-idf/components/bootloader/subproject"
  "C:/Users/wesqu/caixa-de-agua/build/bootloader"
  "C:/Users/wesqu/caixa-de-agua/build/bootloader-prefix"
  "C:/Users/wesqu/caixa-de-agua/build/bootloader-prefix/tmp"
  "C:/Users/wesqu/caixa-de-agua/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/wesqu/caixa-de-agua/build/bootloader-prefix/src"
  "C:/Users/wesqu/caixa-de-agua/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/wesqu/caixa-de-agua/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
