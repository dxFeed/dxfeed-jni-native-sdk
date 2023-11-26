cmake_minimum_required(VERSION 3.15)
project(DxFeedSample)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

message("--- DxFeed samples --- ")

include_directories(${PROJECT_NAME} INTERFACE ../jni-lib/include)

link_directories(${OUTPUT_PATH})

add_library(DX_FEED_JNI_NATIVE_SDK SHARED IMPORTED)
if (WIN32)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/DxFeedJniNativeSdk.dll)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_IMPLIB ${OUTPUT_PATH}/DxFeedJniNativeSdk.lib)
elseif (APPLE)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/DxFeedJniNativeSdk.dylib)
elseif (UNIX)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/DxFeedJniNativeSdk.so)
endif ()

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} DX_FEED_JNI_NATIVE_SDK)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set_target_properties(${PROJECT_NAME} PROPERTIES
      LINKER_LANGUAGE CXX
      LINK_FLAGS "-Wl,-rpath,@executable_path/"
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

message("--- Finished loading DxFeed samples  --- ")
