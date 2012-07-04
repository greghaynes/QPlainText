#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
cmake_minimum_required(VERSION 2.6)

project(qplaintext)

find_package(Qt4 COMPONENTS QtCore QtGui QtTest REQUIRED)

include(${QT_USE_FILE})

set(QPLAINTEXT_HDRS
	documentposition.h
	caret.h
	documentrange.h
	document.h
	documentview.h
	standarddocumentview.h
	keyboardhandler.h
	standarddocument.h
	positionalmarker.h
	selectedrange.h)

set(QPLAINTEXT_SRCS
	documentposition.cpp
	caret.cpp
	documentrange.cpp
	document.cpp
	documentview.cpp
	standarddocumentview.cpp
	standarddocumentviewinternal.cpp
	keyboardhandler.cpp
	standarddocument.cpp
	positionalmarker.cpp
	selectedrange.cpp)

QT4_AUTOMOC(${QPLAINTEXT_SRCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/src)

add_library(qplaintext SHARED ${QPLAINTEXT_SRCS})

set_target_properties(qplaintext
	PROPERTIES
	VERSION 0.1
	SOVERSION 1)

target_link_libraries(qplaintext ${QT_LIBRARIES})

QT4_AUTOMOC(test/teststandarddocument.cpp
	test/testdocumentview.cpp)

add_executable(teststandarddocument
	test/teststandarddocument.cpp)

target_link_libraries(teststandarddocument ${QT_LIBRARIES} qplaintext)

add_executable(testdocumentview
	test/testdocumentview.cpp)

target_link_libraries(testdocumentview ${QT_LIBRARIES} qplaintext)

install(TARGETS qplaintext
	DESTINATION lib)

INSTALL(FILES ${QPLAINTEXT_HDRS}
	DESTINATION include/libqplaintext-1.0/libqplaintext)

