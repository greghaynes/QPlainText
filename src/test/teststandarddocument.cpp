#include "teststandarddocument.h"
#include "standarddocument.h"
#include "documentrange.h"
#include "documentposition.h"

void TestStandardDocument::singeLineInsert()
{
	QSourceEdit::StandardDocument doc;
	QString str("Hello, there.");
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(), str));
	QCOMPARE(doc.lineCount(), 1);
	QCOMPARE(doc.lineLength(0), str.length());
	QString text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(),
			QSourceEdit::DocumentPosition(0, -1)));
	QCOMPARE(text, str);
}

void TestStandardDocument::twoLineInsert()
{
	QSourceEdit::StandardDocument doc;
	QString str("Hello\nthere.");
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(), str));
	QCOMPARE(doc.lineCount(), 2);
	QCOMPARE(doc.lineLength(0), 5);
	QCOMPARE(doc.lineLength(1), 6);
	QString text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(),
			QSourceEdit::DocumentPosition(0, -1)));
	QCOMPARE(text, QString("Hello"));
	text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(1, 0),
			QSourceEdit::DocumentPosition(1, -1)));
	QCOMPARE(text, QString("there."));
}

void TestStandardDocument::innerLineInsert()
{
	QSourceEdit::StandardDocument doc;
	QString str("Hello, there.");
	QString res = str;
	res.insert(2, str);
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(), str));
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(0, 2), str));
	QString text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(),
			QSourceEdit::DocumentPosition(0, -1)));
	QCOMPARE(text, res);
}

void TestStandardDocument::appendLine()
{
	QSourceEdit::StandardDocument doc;
	QString str("Hello, there.");
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(), str));
	str = "Hi!";
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(1, 0), str));
	QCOMPARE(doc.lineCount(), 2);
	QString text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(),
			QSourceEdit::DocumentPosition(1, -1)));
	str = "Hello, there.\nHi!";
	QCOMPARE(text, str);
}

void TestStandardDocument::removeText()
{
	QSourceEdit::StandardDocument doc;
	QString str("Hello, there.");
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(), str));
	QVERIFY(doc.remove(
			QSourceEdit::DocumentRange(
				QSourceEdit::DocumentPosition(0, 1),
				QSourceEdit::DocumentPosition(0, 4))));
	QString text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(),
			QSourceEdit::DocumentPosition(0, -1)));
	str = "Ho, there.";
	QCOMPARE(text, str);
}

QTEST_MAIN(TestStandardDocument)
#include "teststandarddocument.moc"

