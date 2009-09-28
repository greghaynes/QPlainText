#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

find_package(Qt4 REQUIRED)

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

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(qsourceview SHARED ${QSOURCEVIEW_SRCS})

set_target_properties(qsourceview
	PROPERTIES
	VERSION 0.1
	SOVERSION 1)

target_link_libraries(qsourceview ${QT_LIBRARIES})

install(TARGETS qsourceview
	DESTINATION lib)

INSTALL(FILES ${QSOURCEVIEW_HDRS}
	DESTINATION include/libqsourceview-1.0/libqsourceview)
