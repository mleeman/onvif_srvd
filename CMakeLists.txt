cmake_minimum_required(VERSION 3.16)
project(onvif_server_daemon)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

OPTION(CMAKE_BUILD_TYPE "Build type" Release)

message("Configuring as " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_VERBOSE_MAKEFILE ON)
#    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
#    set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
    add_definitions(-DOSRVD_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

find_package(gSOAP REQUIRED)
find_package(JSONCPP REQUIRED)

#Create a cmake target that generate gsoap files
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/generated/onvif.h
    COMMAND ${GSOAP_WSDL2H} -c++11 -d -t ${CMAKE_SOURCE_DIR}/wsdl/typemap.dat -o ${CMAKE_BINARY_DIR}/generated/onvif.h ${CMAKE_SOURCE_DIR}/wsdl/*.wsdl ${CMAKE_SOURCE_DIR}/wsdl/*.xsd
    COMMENT "Creating gSOAP binding file"
)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/generated/onvifDeviceBindingService.cpp ${CMAKE_BINARY_DIR}/generated/onvifMediaBindingService.cpp ${CMAKE_BINARY_DIR}/generated/onvifPTZBindingService.cpp ${CMAKE_BINARY_DIR}/generated/onvifC.cpp
    COMMAND ${GSOAP_SOAPCPP2} -p onvif -c++11 -j -L -x -S -d ${CMAKE_BINARY_DIR}/generated -I${GSOAP_INCLUDE_DIR}:${GSOAP_INCLUDES_DIR} ${CMAKE_BINARY_DIR}/generated/onvif.h
    DEPENDS ${CMAKE_BINARY_DIR}/generated/onvif.h
    COMMENT "Creating gSOAP stubs and glue code"
)

add_custom_target(GSOAP_GENERATION_TARGET
    DEPENDS ${CMAKE_BINARY_DIR}/generated/onvif.h)

set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifH.h                      PROPERTIES GENERATED TRUE )
set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifDeviceBindingService.h   PROPERTIES GENERATED TRUE )
set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifMediaBindingService.h    PROPERTIES GENERATED TRUE )
set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifPTZBindingService.h      PROPERTIES GENERATED TRUE )
set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifStub.h                   PROPERTIES GENERATED TRUE )

set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifC.cpp                    PROPERTIES GENERATED TRUE )
set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifDeviceBindingService.cpp PROPERTIES GENERATED TRUE )
set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifMediaBindingService.cpp  PROPERTIES GENERATED TRUE )
set_source_files_properties( ${CMAKE_BINARY_DIR}/generated/onvifPTZBindingService.cpp    PROPERTIES GENERATED TRUE )

set_source_files_properties( ${GSOAP_INCLUDES_DIR}/custom/chrono_duration.h    PROPERTIES GENERATED TRUE )
set_source_files_properties( ${GSOAP_INCLUDES_DIR}/custom/chrono_duration.cpp    PROPERTIES GENERATED TRUE )
set_source_files_properties( ${GSOAP_INCLUDES_DIR}/custom/chrono_time_point.h    PROPERTIES GENERATED TRUE )
set_source_files_properties( ${GSOAP_INCLUDES_DIR}/custom/chrono_time_point.cpp    PROPERTIES GENERATED TRUE )

set(ONVIF_SRVD_GSOAP_HEADERS
  ${CMAKE_BINARY_DIR}/generated/onvifDeviceBindingService.h
  ${CMAKE_BINARY_DIR}/generated/onvifH.h
  ${CMAKE_BINARY_DIR}/generated/onvifMediaBindingService.h
  ${CMAKE_BINARY_DIR}/generated/onvifPTZBindingService.h
  ${CMAKE_BINARY_DIR}/generated/onvifStub.h
  ${GSOAP_INCLUDES_DIR}/custom/chrono_duration.h
  ${GSOAP_INCLUDES_DIR}/custom/chrono_time_point.h
  )

set(ONVIF_SRVD_GSOAP_SOURCES
  ${CMAKE_BINARY_DIR}/generated/onvifC.cpp
  ${CMAKE_BINARY_DIR}/generated/onvifDeviceBindingService.cpp
  ${CMAKE_BINARY_DIR}/generated/onvifMediaBindingService.cpp
  ${CMAKE_BINARY_DIR}/generated/onvifPTZBindingService.cpp
  ${GSOAP_INCLUDES_DIR}/custom/chrono_duration.cpp
  ${GSOAP_INCLUDES_DIR}/custom/chrono_time_point.cpp
  )

set(ONVIF_SRVD_HEADERS
  src/defines/singletone.h
  src/defines/smacros.h
  src/defines/version.h
  src/defines/soap_defines_head.h
  src/defines/soap_defines.h
  src/network/cethernetdevice.h
  src/st.h
  src/tools/logger/clogger.h
  src/tools/options/options.h
  src/tools/singletone/csingletone.h
  src/tools/tools_fs.h
  src/tools/tools_string.h
  src/onvif/entity/ccameraprofile.h
  src/onvif/entity/ccamerastream.h
  src/onvif/entity/ccameraptz.h
  src/onvif/entity/ccameras.h
  src/onvif/soap/csoap.h
  src/onvif/soap/cdevice.h
  src/onvif/soap/cmedia.h
  src/onvif/soap/cptz.h
  src/cmdline.h
  src/tools/shell.h
  src/tools/networking.h
  )

set(ONVIF_SRVD_SOURCES
  src/network/cethernetdevice.cpp
  src/main.cpp
  src/tools/logger/clogger.cpp
  src/tools/options/options.cpp
  src/tools/singletone/csingletone.cpp
  src/tools/tools_fs.cpp
  src/tools/tools_string.cpp
  src/onvif/entity/ccameraprofile.cpp
  src/onvif/entity/ccamerastream.cpp
  src/onvif/entity/ccameraptz.cpp
  src/onvif/entity/ccameras.cpp
  src/onvif/soap/csoap.cpp
  src/onvif/soap/cdevice.cpp
  src/onvif/soap/cmedia.cpp
  src/onvif/soap/cptz.cpp
  src/cmdline.cpp
  src/tools/shell.cpp
  src/tools/networking.cpp
  )

include_directories(
  src
  src/defines
  src/network
  src/onvif
  src/onvif/entity
  src/onvif/soap
  src/tools
  src/tools/logger
  src/tools/options
  src/tools/singletone
  ${CMAKE_BINARY_DIR}/generated/
  ${GSOAP_INCLUDE_DIR}
  ${GSOAP_INCLUDES_DIR}
  ${JSONCPP_INCLUDE_DIR})

add_definitions(-DSOAP_H_FILE=onvifH.h)
#add_definitions(-DWITH_PURE_VIRTUAL)

add_executable(onvif_srvd ${ONVIF_SRVD_GSOAP_HEADERS} ${ONVIF_SRVD_GSOAP_SOURCES} ${ONVIF_SRVD_HEADERS} ${ONVIF_SRVD_SOURCES})
add_dependencies(onvif_srvd GSOAP_GENERATION_TARGET)
target_link_libraries(onvif_srvd ${GSOAP_LIBRARIES} ${JSONCPP_LIBRARIES} stdc++fs)

