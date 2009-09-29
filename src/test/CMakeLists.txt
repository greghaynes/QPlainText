find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES}
	${CMAKE_SOURCE_DIR}/src)

add_definitions(${QT_DEFINITIONS})

qt4_generate_moc(teststandarddocument.cpp ${CMAKE_BINARY_DIR}/teststandarddocument.moc)
add_executable(teststandarddocument
	teststandarddocument.moc
	teststandarddocument.cpp
	${CMAKE_SOURCE_DIR}/src/standarddocument.cpp)
target_link_libraries(${QT_QMAIN_LIBRARY}
	${QT_QTCORE_LIBRARY}
	${QT_QTTEST_LIBRARY})

