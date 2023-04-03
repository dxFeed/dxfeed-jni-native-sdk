cmake_minimum_required(VERSION 3.15)
#set(CMAKE_OSX_ARCHITECTURES arm64)

set(CMAKE_CXX_STANDARD 17)
project(native_app)

message("PROJECT_NAME = " ${PROJECT_NAME})

message("CMAKE_BUILD_RPATH = " ${CMAKE_CURRENT_BINARY_DIR})
set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

set(JAVA_HOME /Users/Andrey.Mikhalev/Downloads/labsjdk-ce-11.0.18-jvmci-22.3-b13/Contents/Home)
#set(JAVA_HOME D:/Tools/labsjdk-ce-17.0.6-jvmci-22.3-b13)
set(JNI_COMMON_INCLUDE ${JAVA_HOME}/include)

add_subdirectory(jni-lib)
message(JNI_INCLUDE: " ${JNI_INCLUDE}")

include_directories(${PROJECT_NAME} INTERFACE ${JNI_INCLUDE})
include_directories(${PROJECT_NAME} INTERFACE jni-lib/include)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} native_jni)

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
endif()