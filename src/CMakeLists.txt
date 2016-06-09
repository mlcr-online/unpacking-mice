message(STATUS "src ... ")

AUX_SOURCE_DIRECTORY(.                 SOURCE_FILES)
#message(STATUS "${SOURCE_FILES}")

ADD_LIBRARY(MDUnpack      SHARED            ${SOURCE_FILES})
#ADD_CUSTOM_COMMAND(TARGET MDUnpack POST_BUILD
#       COMMAND  ../../archive_all.sh)

INSTALL_TARGETS(/lib MDUnpack)
