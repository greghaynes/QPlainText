#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
project(qsourceedit)

find_package(Qt4 COMPONENTS QtCore QtGui QtTest REQUIRED)

include(${QT_USE_FILE})

set(QSOURCEEDIT_HDRS
	documentposition.h
	documentcaret.h
	documentrange.h
	document.h
	documentview.h
	documentcontroller.h
	standarddocument.h)

set(QSOURCEEDIT_SRCS
	documentposition.cpp
	documentcaret.cpp
	documentrange.cpp
	document.cpp
	documentview.cpp
	documentviewinternal.cpp
	documentcontroller.cpp
	standarddocument.cpp)

QT4_AUTOMOC(${QSOURCEEDIT_SRCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/src)

add_library(qsourceedit SHARED ${QSOURCEEDIT_SRCS})

set_target_properties(qsourceedit
	PROPERTIES
	VERSION 0.1
	SOVERSION 1)

target_link_libraries(qsourceedit ${QT_LIBRARIES})

QT4_AUTOMOC(test/teststandarddocument.cpp
	test/testdocumentview.cpp)

add_executable(teststandarddocument
	test/teststandarddocument.cpp)

target_link_libraries(teststandarddocument ${QT_LIBRARIES} qsourceedit)

add_executable(testdocumentview
	test/testdocumentview.cpp)

target_link_libraries(testdocumentview ${QT_LIBRARIES} qsourceedit)

install(TARGETS qsourceedit
	DESTINATION lib)

INSTALL(FILES ${QSOURCEEDIT_HDRS}
	DESTINATION include/libqsourceview-1.0/libqsourceedit)

