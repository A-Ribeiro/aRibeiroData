# "For this is how God loved the world:
# he gave his only Son, so that everyone
# who believes in him may not perish
# but may have eternal life."
#
# John 3:16

find_path(ARIBEIRODATA_INCLUDE_DIR aRibeiroData/aRibeiroData.h)
find_library(ARIBEIRODATA_LIBRARIES NAMES aRibeiroData)

if(ARIBEIRODATA_INCLUDE_DIR AND ARIBEIRODATA_LIBRARIES)
	set(ARIBEIRODATA_FOUND ON)
endif()

if(ARIBEIRODATA_FOUND)
	if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
		MESSAGE(STATUS "Found ${CMAKE_FIND_PACKAGE_NAME} include:  ${ARIBEIRODATA_INCLUDE_DIR}/aRibeiroData/aRibeiroData.h")
		MESSAGE(STATUS "Found ${CMAKE_FIND_PACKAGE_NAME} library: ${ARIBEIRODATA_LIBRARIES}")
	endif()
else()
	if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could NOT find ${CMAKE_FIND_PACKAGE_NAME} development files")
	endif()
endif()
