########################################################################
#
# - Find QualityControl from Alice O2
#
#  QUALITY_CONTROL_INCLUDE_DIRS - Where to find headers
#  QUALITY_CONTROL_LIBRARY      - Library of QC
#  QUALITY_CONTROL_FOUND        - True if found
#
#  QUALITYCONTROL_ROOT  - environment variable set by aliBuild
#
#  Authors: Vladimir Kosmala
#
########################################################################

message(STATUS "Looking for QualityControl...")

set(QUALITYCONTROL_ROOT "$ENV{QUALITYCONTROL_ROOT}")

# Check if alibuild path is set
if (NOT QUALITYCONTROL_ROOT)
  message(STATUS "Looking for QualityControl... - QUALITYCONTROL_ROOT not set")
endif (NOT QUALITYCONTROL_ROOT)

# Find headers
find_path(
  QUALITY_CONTROL_INCLUDE_DIRS
  NAMES QualityControl/MonitorObject.h
  HINTS "${QUALITYCONTROL_ROOT}/include" ENV LD_LIBRARY_PATH
  PATH_SUFFIXES "../include"
)

# Find libraries
find_library(
  QUALITY_CONTROL_LIBRARY
  NAMES "libQualityControl.dylib" "libQualityControl.so"
  HINTS "${QUALITYCONTROL_ROOT}/lib" ENV LD_LIBRARY_PATH
)

if (QUALITY_CONTROL_INCLUDE_DIRS)
  Message(STATUS "Looking for QualityControl... - Found headers ${QUALITY_CONTROL_INCLUDE_DIRS}")
else (QUALITY_CONTROL_INCLUDE_DIRS)
  message(STATUS "Looking for QualityControl... - Headers not found")
  unset(QUALITY_CONTROL_INCLUDE_DIRS CACHE)
endif (QUALITY_CONTROL_INCLUDE_DIRS)

if (QUALITY_CONTROL_LIBRARY)
  Message(STATUS "Looking for QualityControl... - Found lib ${QUALITY_CONTROL_LIBRARY}")
else (QUALITY_CONTROL_LIBRARY)
  message(STATUS "Looking for QualityControl... - Lib not found")
  unset(QUALITY_CONTROL_LIBRARY CACHE)
endif (QUALITY_CONTROL_LIBRARY)

if (QUALITY_CONTROL_INCLUDE_DIRS AND QUALITY_CONTROL_LIBRARY)
  set(QUALITY_CONTROL_FOUND TRUE)
else (QUALITY_CONTROL_INCLUDE_DIRS AND QUALITY_CONTROL_LIBRARY)
  set(QUALITY_CONTROL_FOUND FALSE)
endif (QUALITY_CONTROL_INCLUDE_DIRS AND QUALITY_CONTROL_LIBRARY)

#unset(QUALITY_CONTROL_INCLUDE_DIRS CACHE)
#unset(QUALITY_CONTROL_LIBRARY CACHE)

# Send result of search or die
if (QUALITY_CONTROL_FOUND)
  mark_as_advanced(
    QUALITY_CONTROL_LIBRARY
    QUALITY_CONTROL_INCLUDE_DIRS
  )
else (QUALITY_CONTROL_FOUND)
  if (QUALITY_CONTROL_FIND_REQUIRED)
    message(FATAL_ERROR "Stop here because QUALITY_CONTROL not found and required.")
  endif (QUALITY_CONTROL_FIND_REQUIRED)
endif (QUALITY_CONTROL_FOUND)
