cmake_minimum_required(VERSION 3.15)
#set(CMAKE_OSX_ARCHITECTURES arm64)
project(native_jni_sdk)

set(CMAKE_CXX_STANDARD 17)

set(JAVA_HOME /Users/Andrey.Mikhalev/Documents/Tools/graalvm-ce-java17-22.1.0-dev/Contents/Home)
#set(JAVA_HOME D:/Tools/labsjdk-ce-17.0.6-jvmci-22.3-b13)
message("JAVA_HOME = " ${JAVA_HOME})

set(JNI_COMMON_INCLUDE ${JAVA_HOME}/include)
message("JNI_COMMON_INCLUDE = " ${JNI_COMMON_INCLUDE})

if (WIN32)
  # Set the DLLEXPORT variable to export symbols
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/win32)
elseif (APPLE)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/darwin)
elseif (UNIX)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/linux)
endif ()


set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
message("CMAKE_BUILD_RPATH = " ${OUTPUT_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

add_subdirectory(jni-lib)
add_subdirectory(samples)

add_custom_target(${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} native_jni time_and_sale)