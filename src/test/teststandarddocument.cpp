#include "teststandarddocument.h"
#include "standarddocument.h"
#include "documentrange.h"
#include "documentposition.h"

#include <iostream>

void TestStandardDocument::singeLineInsert()
{
	QPlainText::StandardDocument doc;
	QString str("Hello, there.");
	QVERIFY(doc.insert(QPlainText::DocumentPosition(), str));
	QCOMPARE(doc.lineCount(), 1);
	QCOMPARE(doc.lineLength(0), str.length());
	QString text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(),
			QPlainText::DocumentPosition(0, -1)));
	QCOMPARE(text, str);
}

void TestStandardDocument::twoLineInsert()
{
	QPlainText::StandardDocument doc;
	QString str("Hello\nthere.");
	QVERIFY(doc.insert(QPlainText::DocumentPosition(), str));
	QString text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(),
			QPlainText::DocumentPosition(0, -1)));
	QCOMPARE(text, QString("Hello"));
	text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(1, 0),
			QPlainText::DocumentPosition(1, -1)));
	QCOMPARE(text, QString("there."));
	QCOMPARE(doc.lineCount(), 2);
	QCOMPARE(doc.lineLength(0), 5);
	QCOMPARE(doc.lineLength(1), 6);
}

void TestStandardDocument::innerLineInsert()
{
	QPlainText::StandardDocument doc;
	QString str("Hello, there.");
	QString res = str;
	res.insert(2, str);
	QVERIFY(doc.insert(QPlainText::DocumentPosition(), str));
	QVERIFY(doc.insert(QPlainText::DocumentPosition(0, 2), str));
	QString text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(),
			QPlainText::DocumentPosition(0, -1)));
	QCOMPARE(text, res);
}

void TestStandardDocument::seperateTwoLineInsert()
{
	QPlainText::StandardDocument doc;
	QString str("Hello, there.");
	QVERIFY(doc.insert(QPlainText::DocumentPosition(), str));
	str = "Hi!";
	QVERIFY(doc.insert(QPlainText::DocumentPosition(1, 0), str));
	QCOMPARE(doc.lineCount(), 2);
	QString text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(),
			QPlainText::DocumentPosition(1, -1)));
	str = "Hello, there.\nHi!";
	QCOMPARE(text, str);
}

void TestStandardDocument::append()
{
	QPlainText::StandardDocument doc;
	QString str("Hello");
	QVERIFY(doc.append(str));
	QCOMPARE(doc.lineCount(), 1);
	QCOMPARE(doc.text(QPlainText::DocumentRange(
			QPlainText::DocumentPosition(0, 0),
			QPlainText::DocumentPosition(0, -1))),
		QString("Hello"));
}

void TestStandardDocument::appendNewlineText()
{
	QPlainText::StandardDocument doc;
	QString str("Hello");
	QVERIFY(doc.append("Hello"));
	QVERIFY(doc.append("\n"));
	QVERIFY(doc.append("World!"));
	QCOMPARE(doc.lineCount(), 2);
	QCOMPARE(doc.text(QPlainText::DocumentRange(
			QPlainText::DocumentPosition(0, 0),
			QPlainText::DocumentPosition(1, -1))),
		QString("Hello\nWorld!"));
}

void TestStandardDocument::removeText()
{
	QPlainText::StandardDocument doc;
	QString str("Hello, there.");
	QVERIFY(doc.insert(QPlainText::DocumentPosition(), str));
	QVERIFY(doc.remove(
			QPlainText::DocumentRange(
				QPlainText::DocumentPosition(0, 1),
				QPlainText::DocumentPosition(0, 4))));
	QString text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(),
			QPlainText::DocumentPosition(0, -1)));
	str = "Ho, there.";
	QCOMPARE(text, str);
}

void TestStandardDocument::removeMultilineText()
{
	QPlainText::StandardDocument doc;
	QString str("Hello,\nthere.");
	QVERIFY(doc.insert(QPlainText::DocumentPosition(), str));

	doc.remove(QPlainText::DocumentRange(
		QPlainText::DocumentPosition(0, -1),
		QPlainText::DocumentPosition(1, 0)));

	QString text = doc.text(QPlainText::DocumentRange(
		QPlainText::DocumentPosition(0, 0),
		QPlainText::DocumentPosition(0, -1)));
	str = "Hello,there.";
	QCOMPARE(text, str);
}

void TestStandardDocument::insertInternalNewline()
{
	QPlainText::StandardDocument doc;
	QString str("Hello, there.");
	QVERIFY(doc.insert(QPlainText::DocumentPosition(), str));
	QVERIFY(doc.insert(QPlainText::DocumentPosition(0, 5), "\n"));

	QString text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(),
			QPlainText::DocumentPosition(0, -1)));
	str = "Hello";
	QCOMPARE(text, str);
	
	text = doc.text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(1, 0),
			QPlainText::DocumentPosition(1, -1)));

	str = ", there.";
	QCOMPARE(text, str);
}

QTEST_MAIN(TestStandardDocument)
#include "teststandarddocument.moc"

