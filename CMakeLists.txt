INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR} ${ROSS_BINARY_DIR})

SET(airport_srcs
dynPer.c		dynPer.h)

ADD_EXECUTABLE(dynPer ${airport_srcs})

TARGET_LINK_LIBRARIES(dynPer ROSS m)
