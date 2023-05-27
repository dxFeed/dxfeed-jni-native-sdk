cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
project(time_and_sale)

message("sample CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})

include_directories(${PROJECT_NAME} INTERFACE ../jni-lib/include)
include_directories(${PROJECT_NAME} INTERFACE include)

message("JNI_INCLUDE: ${JNI_INCLUDE}")
include_directories(${PROJECT_NAME} INTERFACE ${JNI_INCLUDE})

message("OUTPUT_PATH:" ${OUTPUT_PATH})

link_directories(${OUTPUT_PATH})

add_library(NATIVE_JNI_LIB SHARED IMPORTED)
if (WIN32)
  message("WIN")
  set_property(TARGET NATIVE_JNI_LIB PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/native_jni.dll)
  set_property(TARGET NATIVE_JNI_LIB PROPERTY IMPORTED_IMPLIB ${OUTPUT_PATH}/native_jni.lib)
elseif (APPLE)
  message("APPLE")
  set_property(TARGET NATIVE_JNI_LIB PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/native_jni.dylib)
elseif (UNIX)
  message("UNIX")
  set_property(TARGET NATIVE_JNI_LIB PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/native_jni.so)
endif ()

add_executable(${PROJECT_NAME}
    main.cpp
    src/utils/StringUtils.cpp
    src/utils/TimeAndSaleFormatter.cpp
)

target_link_libraries(${PROJECT_NAME} NATIVE_JNI_LIB)

# ------------ LIB date ------------
include(FetchContent)
FetchContent_Declare(date
    GIT_REPOSITORY https://github.com/HowardHinnant/date.git
    GIT_TAG v3.0.1
)
FetchContent_MakeAvailable(date)
target_link_libraries(${PROJECT_NAME} date)
# ------------ LIB date ------------


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  # todo read: https://dev.my-gate.net/2021/08/04/understanding-rpath-with-cmake/

  # current solution https://stackoverflow.com/questions/50121387/cmake-use-rpath-for-linked-libraries-instead-of-the-full-path
  set_target_properties(${PROJECT_NAME} PROPERTIES
      LINKER_LANGUAGE CXX
      LINK_FLAGS "-Wl,-rpath,./"
      MACOSX_RPATH TRUE
      SKIP_BUILD_RPATH FALSE
      BUILD_WITH_INSTALL_RPATH TRUE
      INSTALL_RPATH_USE_LINK_PATH TRUE
      INSTALL_NAME_DIR "@rpath/")

  target_compile_options(${PROJECT_NAME} PRIVATE
      -std=c++17
      -stdlib=libc++
      -Wall
      -Wextra
      -Wno-unused-parameter
      -O3
      )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  target_compile_definitions(${PROJECT_NAME} PRIVATE
      _WIN32_WINDOWS
      _WINSOCK_DEPRECATED_NO_WARNINGS
      )
  target_compile_options(${PROJECT_NAME} PRIVATE
      /std:c++17
      /W3
      /bigobj
      /EHsc
      "$<$<CONFIG:Release>:/GL>"
      "$<$<CONFIG:Release>:/Ox>"
      "$<$<CONFIG:Release>:/Ob2>"
      "$<$<CONFIG:Release>:/Ot>"
      "$<$<CONFIG:Release>:/Oi>"
      "$<$<CONFIG:Release>:/Oy->"
      )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${PROJECT_NAME} PRIVATE
      -std=c++17
      -Wall
      -Wextra
      -Wno-unused-parameter
      -O3
      )
endif ()