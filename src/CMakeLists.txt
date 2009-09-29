#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
project(qsourceview)

find_package(Qt4 COMPONENTS QtCore QtGui QtTest REQUIRED)

include(${QT_USE_FILE})

set(QSOURCEVIEW_HDRS
	documentposition.h
	documentrange.h
	document.h
	standarddocument.h)

set(QSOURCEVIEW_SRCS
	documentposition.cpp
	documentrange.cpp
	document.cpp
	standarddocument.cpp)

QT4_AUTOMOC(${QSOURCEVIEW_SRCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/src)

add_library(qsourceview SHARED ${QSOURCEVIEW_SRCS})

set_target_properties(qsourceview
	PROPERTIES
	VERSION 0.1
	SOVERSION 1)

target_link_libraries(qsourceview ${QT_LIBRARIES})

QT4_AUTOMOC(test/teststandarddocument.cpp)

add_executable(teststandarddocument
	test/teststandarddocument.cpp)

target_link_libraries(teststandarddocument ${QT_LIBRARIES} qsourceview)

install(TARGETS qsourceview
	DESTINATION lib)

INSTALL(FILES ${QSOURCEVIEW_HDRS}
	DESTINATION include/libqsourceview-1.0/libqsourceview)
