cmake_minimum_required(VERSION 3.29)
project(pltxt2htm_csharp_translator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(PLTXT2HTM_HEADER "${CMAKE_CURRENT_LIST_DIR}/../../include/pltxt2htm/pltxt2htm.hh")
set(OUTPUT_CS "${CMAKE_CURRENT_LIST_DIR}/generated/Pltxt2htm.Generated.cs")

add_custom_command(
  OUTPUT "${OUTPUT_CS}"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_LIST_DIR}/generated"
  COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_LIST_DIR}/src/translate.py"
          --header "${PLTXT2HTM_HEADER}"
          --output "${OUTPUT_CS}"
  DEPENDS "${PLTXT2HTM_HEADER}" "${CMAKE_CURRENT_LIST_DIR}/src/translate.py"
  COMMENT "Translating include/pltxt2htm headers into C#"
  VERBATIM
)

add_custom_target(translate ALL DEPENDS "${OUTPUT_CS}")
