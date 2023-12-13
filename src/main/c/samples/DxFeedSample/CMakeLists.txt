cmake_minimum_required(VERSION 3.15)
project(DxFeedSample)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "valid configurations" FORCE)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

message("--- DxFeedSample sample --- ")

# Specify include directory where to find API headers
include(../../cmake/FindJava.cmake)
include_directories(${PROJECT_NAME} INTERFACE ../../jni-lib/include)

# Specify a directory where to build
set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
include(../../cmake/SetupOutputDirectory.cmake)

# Add source for our sample
add_executable(${PROJECT_NAME}
    main.cpp
)

# Include common cmake file to find Java, specify linkage directory where to find and link
#   DxFeedJniNativeSdk.dylib|dll|.so as SHARED dependency
include(../../cmake/FindJniNativeSdk.cmake)
link_lib_from_dir(${OUTPUT_PATH})

# Link DxFeedJniNativeSdk.dylib|dll|.so to our executable
target_link_libraries(${PROJECT_NAME} DX_FEED_JNI_NATIVE_SDK)

# Specify target compile options (e.g.: RPATH for MacOS executables fo find .dylib on app startup
specify_target_compile_options()