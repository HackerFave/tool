cmake_project(ZMQ_WRAPPER)

set(ZMQ_WRAPPER_SRCS ${ZMQ_WRAPPER_SOURCE_DIR}/src)
set(ZMQ_WRAPPER_INCS ${ZMQ_WRAPPER_SOURCE_DIR}/include
                     ${ZMQ_WRAPPER_SOURCE_DIR}/../common/include
                     ${ZMQ_WRAPPER_SOURCE_DIR}/../fbs/out
                      )

add_definitions(-DBOOST_ALL_DYN_LINK)
set(ZMQ_WRAPPER_LIBS zmq boost_log)
set(PROGRAM_NAME           zmq_wrapper)

cmake_module(
    NAME      ${PROGRAM_NAME}
    TYPE      SHARED
    SOURCE    ${ZMQ_WRAPPER_SRCS}
    INCLUDE   ${ZMQ_WRAPPER_INCS}
    INTERFACE ${ZMQ_WRAPPER_INCS}
    LINK_LIB  ${ZMQ_WRAPPER_LIBS}
    PACKAGE   ${PROGRAM_NAME}
)