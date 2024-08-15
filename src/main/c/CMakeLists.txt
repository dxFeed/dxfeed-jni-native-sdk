# Copyright (c) 2024 Devexperts LLC.
# SPDX-License-Identifier: MPL-2.0
cmake_minimum_required(VERSION 3.15)
project(DxFeedJniNativeSdk)

set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/demo/build)
include(cmake/SetupOutputDirectory.cmake)

add_subdirectory(jni-lib)
add_subdirectory(demo)
add_custom_target(OUTPUT DxFeedSample DEPENDS DxFeedJniNativeSdk)