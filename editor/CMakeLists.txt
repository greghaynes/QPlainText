#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

set(EDITOR_SRCS
	main.cpp
	documentwidget.cpp)

QT4_AUTOMOC(${EDITOR_SRCS})
QT4_ADD_RESOURCES(EDITOR_RCC_SRCS editor.qrc)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

add_executable(editor ${EDITOR_SRCS} ${EDITOR_RCC_SRCS})

target_link_libraries(editor qsourceedit)
