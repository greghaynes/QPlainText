#ifndef QSOURCEVIEW_TESTDDOCUMENTVIEW_H
#define QSOURCEVIEW_TESTDDOCUMENTVIEW_H

#include <QtTest/QtTest>

class TestDocumentView
	: public QObject
{
	Q_OBJECT
	
	private Q_SLOTS:
		void empty();
		void typeHello();

};

#endif

