find_package(Boost 1.58 REQUIRED)
find_package(MySQL REQUIRED)
find_package(ROOT 6.06.02 COMPONENTS RMySQL REQUIRED)
find_package(CURL REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(QualityControl REQUIRED)
find_package(Common REQUIRED)

if (NOT MYSQL_FOUND)
    message(WARNING "MySQL not found, the corresponding classes won't be built.")
endif ()

o2_define_bucket(
  NAME
  o2_tobject2json

  DEPENDENCIES
  ${ROOT_LIBRARIES}
  ${CURL_LIBRARIES}
  ${MYSQL_LIBRARIES}
  ${ZeroMQ_LIBRARY_STATIC}
  ${QUALITY_CONTROL_LIBRARY}
  ${Common_LIBRARIES}

  SYSTEMINCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIRS}
  ${ROOT_INCLUDE_DIR}
  ${CURL_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIR}
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
  ${Common_LIBRARIES}

  SYSTEMINCLUDE_DIRECTORIES
  ${ZeroMQ_INCLUDE_DIR}
  ${Common_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)
