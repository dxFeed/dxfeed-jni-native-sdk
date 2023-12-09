cmake_minimum_required(VERSION 3.15)
project(DxFeedConnect)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)

message("--- DxFeedConnect samples --- ")

message("--- Setting up: found JNI and JRE --- ")

# Find include directories for Java headers
set(JAVA_HOME $ENV{JAVA_HOME})
message("  JAVA_HOME = " ${JAVA_HOME})

set(JNI_COMMON_INCLUDE ${JAVA_HOME}/include)
message("  JNI_COMMON_INCLUDE = " ${JNI_COMMON_INCLUDE})

if (WIN32)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/win32)
elseif (APPLE)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/darwin)
elseif (UNIX)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/linux)
endif ()

message("  JNI_INCLUDE: ${JNI_INCLUDE}")
include_directories(${PROJECT_NAME} INTERFACE ${JNI_INCLUDE})

set(JAVA_LIB_PATH "${JAVA_HOME}/lib/server")
message("  JAVA_LIB_PATH: ${JAVA_LIB_PATH}")

# Setup output directory as "build"
set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
message("  CMAKE_BUILD_RPATH = " ${OUTPUT_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_PATH})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_PATH})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_PATH})
endforeach()

# Specify include directory where to find API headers
include_directories(${PROJECT_NAME} INTERFACE ../../jni-lib/include)

# Specify linkage directory where to find DxFeedJniNativeSdk.dylib|dll|.so
link_directories(${OUTPUT_PATH})

# Add DxFeedJniNativeSdk.dylib|dll|.so as SHARED dependency
add_library(DX_FEED_JNI_NATIVE_SDK SHARED IMPORTED)
if (WIN32)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/DxFeedJniNativeSdk.dll)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_IMPLIB ${OUTPUT_PATH}/DxFeedJniNativeSdk.lib)
elseif (APPLE)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/DxFeedJniNativeSdk.dylib)
elseif (UNIX)
  set_property(TARGET DX_FEED_JNI_NATIVE_SDK PROPERTY IMPORTED_LOCATION ${OUTPUT_PATH}/DxFeedJniNativeSdk.so)
endif ()

# Add source for our sample
add_executable(${PROJECT_NAME}
    main.cpp
)

# Link DxFeedJniNativeSdk.dylib|dll|.so to our executable
target_link_libraries(${PROJECT_NAME} DX_FEED_JNI_NATIVE_SDK)

# Additional: compiler options that actually could be commented
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
#      -O3
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
