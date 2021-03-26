# - Try to find Jsoncpp
# Once done, this will define
#
#  JSONCPP_FOUND - system has Jsoncpp
#  JSONCPP_INCLUDE_DIRS - the Jsoncpp include directories
#  JSONCPP_LIBRARIES - link these to use Jsoncpp

message(STATUS "Searching JSONCPP")
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(JSONCPP jsoncpp)
# -----------------------------------------------------
# JSONCPP Libraries
# -----------------------------------------------------
find_library(JSONCPP_LIBRARIES
    NAMES jsoncpp
    HINTS ${JSONCPP_LOCATION}/lib
          ${JSONCPP_LOCATION}/lib32
          ${JSONCPP_LOCATION}/lib64
    DOC "The jsoncpp library"
)

# -----------------------------------------------------
# JSONCPP Include Directories
# -----------------------------------------------------
find_path(JSONCPP_INCLUDE_DIR
    NAMES json/json.h json/value.h
    PATH_SUFFIXES jsoncpp
    HINTS ${JSONCPP_LOCATION}/include/jsoncpp/json
          ${JSONCPP_LOCATION}/include/jsoncpp
          ${JSONCPP_LOCATION}/include
    DOC "The jsoncpp include directory"
)

message(STATUS "JSONCPP         lib : ${JSONCPP_LIBRARIES}")
message(STATUS "JSONCPP include dir : ${JSONCPP_INCLUDE_DIR}")

# -----------------------------------------------------
# handle the QUIETLY and REQUIRED arguments and set GSOAP_FOUND to TRUE if
# all listed variables are TRUE
# -----------------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSONCPP DEFAULT_MSG JSONCPP_LIBRARIES JSONCPP_INCLUDE_DIR)
mark_as_advanced(JSONCPP_INCLUDE_DIR JSONCPP_LIBRARIES)
