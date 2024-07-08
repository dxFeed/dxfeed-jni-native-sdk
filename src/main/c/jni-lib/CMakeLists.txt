cmake_minimum_required(VERSION 3.15)
project(DxFeedJniNativeSdk)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

message("--- Loading DxFeedJniNativeSdk --- ")

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "valid configurations" FORCE)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

set(API_INCLUDE include)
include_directories(${PROJECT_NAME} INTERFACE ${API_INCLUDE})

include(../cmake/PlatformInfo.cmake)
include(../cmake/FindJava.cmake)

if ("${OUTPUT_PATH}" STREQUAL "")
  # Specify a directory where to build
  set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
  include(../cmake/SetupOutputDirectory.cmake)
endif ()

if (WIN32)
  # Set the DLLEXPORT variable to export symbols
  # if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  # endif()
endif ()

set(SOURCE_FILES
    src/dllmain.cpp
    src/DxEndpoint.cpp
    src/DxEndpointBuilder.cpp
    src/DxEventT.cpp
    src/DxFeed.cpp
    src/DxIndexedEventSource.cpp
    src/DxPublisher.cpp
    src/DxSubscription.cpp
    src/DxSymbol.cpp
    src/utils/DxThreadException.cpp
    src/api/dxfg_api.cpp
    src/api/dxfg_catch_exception.cpp
    src/api/dxfg_dxfeed.cpp
    src/api/dxfg_endpoint.cpp
    src/api/dxfg_events.cpp
    src/api/dxfg_publisher.cpp
    src/api/dxfg_schedule.cpp
    src/api/dxfg_subscription.cpp
    src/api/dxfg_system.cpp
    src/api/dxfg_time_format.cpp
    src/api/graal_isolate.cpp
    src/events/CandleMapping.cpp
    src/events/ConfigurationMapping.cpp
    src/events/GreeksMapping.cpp
    src/events/MessageMapping.cpp
    src/events/OptionSaleMapping.cpp
    src/events/OrderMapping.cpp
    src/events/QuoteMapping.cpp
    src/events/ProfileMapping.cpp
    src/events/SeriesMapping.cpp
    src/events/SummaryMapping.cpp
    src/events/TheoPriceMapping.cpp
    src/events/TimeAnsSaleMapping.cpp
    src/events/TradeMapping.cpp
    src/events/UnderlyingMapping.cpp
    src/listeners/DxEventListener.cpp
    src/listeners/DxStateChangeListener.cpp
    src/listeners/DxSubscriptionChangeListener.cpp
    src/utils/ByteReader.cpp
    src/utils/ByteWriter.cpp
    src/utils/JNIBridgeImpl.cpp
    src/utils/JNIUtils.cpp
    src/utils/LoadJvmLib.cpp
    src/utils/NativeEventsList.cpp
    src/utils/java/DxFeedJniClass.cpp
    src/utils/java/JavaLangClass.cpp
    src/utils/java/JavaLangSystem.cpp
    src/utils/java/JavaLogger.cpp
    src/utils/java/DxTimeFormat.cpp
    src/utils/vm/InitJavaVm.cpp
    src/utils/vm/JavaVmInstance.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

link_directories(${JAVA_LIB_PATH})

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
      # "$<$<CONFIG:Release>:/GL>"
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

message("--- FinishedLoading DxFeedJniNativeSdk --- ")
