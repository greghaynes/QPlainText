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
	QString *text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(),
			QSourceEdit::DocumentPosition(0, -1)));
	QVERIFY(text != 0);
	if(text != 0)
	{
		QCOMPARE(*text, str);
		delete text;
	}
}

void TestStandardDocument::twoLineInsert()
{
	QSourceEdit::StandardDocument doc;
	QString str("Hello\nthere.");
	QVERIFY(doc.insert(QSourceEdit::DocumentPosition(), str));
	QCOMPARE(doc.lineCount(), 2);
	QCOMPARE(doc.lineLength(0), 5);
	QCOMPARE(doc.lineLength(1), 6);
	QString *text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(),
			QSourceEdit::DocumentPosition(0, -1)));
	QVERIFY(text != 0);
	if(text != 0)
	{
		QCOMPARE(*text, QString("Hello"));
		delete text;
	}
	text = doc.text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(1, 0),
			QSourceEdit::DocumentPosition(1, -1)));
	QVERIFY(text != 0);
	if(text != 0)
	{
		QCOMPARE(*text, QString("there."));
		delete text;
	}
}

QTEST_MAIN(TestStandardDocument)
#include "teststandarddocument.moc"

