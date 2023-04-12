cmake_minimum_required(VERSION 3.15)
#set(CMAKE_OSX_ARCHITECTURES arm64)
project(native_jni)

set(CMAKE_CXX_STANDARD 17)

message("native_jni CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
message("JNI_COMMON_INCLUDE: ${JNI_COMMON_INCLUDE}")

if (WIN32)
  # Set the DLLEXPORT variable to export symbols
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/win32)
  set(JNI_INCLUDE ${JNI_INCLUDE} PARENT_SCOPE)
elseif (APPLE)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/darwin)
  set(JNI_INCLUDE ${JNI_INCLUDE} PARENT_SCOPE)
elseif (UNIX)
  set(JNI_INCLUDE ${JNI_COMMON_INCLUDE} ${JNI_COMMON_INCLUDE}/linux)
  set(JNI_INCLUDE ${JNI_INCLUDE} PARENT_SCOPE)
endif ()

message("JNI_COMMON_INCLUDE: ${JNI_INCLUDE}")
include_directories(${PROJECT_NAME} INTERFACE ${JNI_INCLUDE})
set(API_INCLUDE include)
set(API_INCLUDE ${API_INCLUDE} PARENT_SCOPE)
include_directories(${PROJECT_NAME} INTERFACE ${API_INCLUDE})

message("-- Linking native_lib")
set(SOURCE_FILES
    JNIBridgeImpl.cpp
    src/Api.cpp
    src/Connection.cpp
    src/DxFeed.cpp
    src/JavaProperty.cpp
    src/LoadJavaVm.cpp
    src/LoadLibrary.cpp
    src/Subscription.cpp
    src/utils/Diagnostic.cpp
    src/utils/JNIUtils.cpp
    src/utils/ProcessPriority.cpp
    src/utils/StopWatch.cpp
    src/utils/StringUtils.cpp
    src/utils/TimeAndSaleFormatter.cpp
    src/utils/Timer.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

set(JAVA_LIB_PATH "${JAVA_HOME}/lib/server")
message("JAVA_LIB_PATH: ${JAVA_LIB_PATH}")

link_directories(${JAVA_LIB_PATH})

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FetchContent)

FetchContent_Declare(date
    GIT_REPOSITORY https://github.com/HowardHinnant/date.git
    GIT_TAG v3.0.1
    )
FetchContent_MakeAvailable(date)

target_link_libraries(${PROJECT_NAME} date)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
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