find_package(ROOT 6.06.02 COMPONENTS RMySQL REQUIRED)
find_package(CURL REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(QualityControl REQUIRED)
find_package(Common REQUIRED)

o2_define_bucket(
  NAME
  o2_tobject2json

  DEPENDENCIES
  ${ROOT_LIBRARIES}
  ${CURL_LIBRARIES}
  ${ZeroMQ_LIBRARY_STATIC}
  ${QUALITY_CONTROL_LIBRARY}
  ${Common_LIBRARIES}

  SYSTEMINCLUDE_DIRECTORIES
  ${ROOT_INCLUDE_DIR}
  ${CURL_INCLUDE_DIRS}
  ${ZeroMQ_INCLUDE_DIR}
  ${QUALITY_CONTROL_INCLUDE_DIRS}
  ${Common_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/include
)

o2_define_bucket(
  NAME
  o2_test_zmq

  DEPENDENCIES
  ${ZeroMQ_LIBRARY_STATIC}

  SYSTEMINCLUDE_DIRECTORIES
  ${ZeroMQ_INCLUDE_DIR}
)
