INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR} ${ROSS_BINARY_DIR})

SET(dynPer_srcs
dynPer.c		dynPer.h)

ADD_EXECUTABLE(ross_dynPer ${dynPer_srcs})

TARGET_LINK_LIBRARIES(ross_dynPer ROSS m)
