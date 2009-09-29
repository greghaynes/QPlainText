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
}

QTEST_MAIN(TestStandardDocument)
#include "teststandarddocument.moc"

