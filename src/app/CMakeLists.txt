message(STATUS "app ... ")

ADD_EXECUTABLE(unpack            unpackApplication.cpp)
TARGET_LINK_LIBRARIES(unpack     MDUnpack)

ADD_EXECUTABLE(mytest            myTest.cpp)
TARGET_LINK_LIBRARIES(mytest     MDUnpack ${ROOT_LIBRARIES})

ADD_EXECUTABLE(mdrebuild         MDRebuild.cpp)
TARGET_LINK_LIBRARIES(mdrebuild  MDUnpack ${ROOT_LIBRARIES})