INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR} ${ROSS_BINARY_DIR} /home/vivek/research/dynamic_perceptron/livermore/new_template_model/igraph/src/.libs /home/vivek/research/dynamic_perceptron/livermore/new_template_model/igraph/include)

SET(dynPer_srcs
  dynPer_888.c		dynPer_888.h)

ADD_EXECUTABLE(dynPer ${dynPer_srcs})

TARGET_LINK_LIBRARIES(dynPer ROSS m)
