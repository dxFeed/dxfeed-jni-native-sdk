cmake_minimum_required(VERSION 3.15)
project(DxFeedJniNativeSdk)

set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
include(cmake/SetupOutputDirectory.cmake)

add_subdirectory(jni-lib)
add_subdirectory(samples)
add_custom_target(OUTPUT DxFeedSample DEPENDS DxFeedJniNativeSdk)