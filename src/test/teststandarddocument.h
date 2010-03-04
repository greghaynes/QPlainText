#ifndef QSOURCEVIEW_TESTSTDDOCUMENT_H
#define QSOURCEVIEW_TESTSTDDOCUMENT_H

#include <QtTest/QtTest>

class TestStandardDocument
	: public QObject
{
	Q_OBJECT
	
	private Q_SLOTS:
		void singeLineInsert();
		void twoLineInsert();
		void innerLineInsert();
		void seperateTwoLineInsert();
		void append();
		void removeText();

};

#endif

