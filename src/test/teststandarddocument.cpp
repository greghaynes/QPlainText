#include "teststandarddocument.h"
#include "standarddocument.h"
#include "documentrange.h"
#include "documentposition.h"

void TestStandardDocument::singeLineInsert()
{
	QSourceEdit::StandardDocument doc;
	QString str("Hello, there.");
	doc.insert(QSourceEdit::DocumentPosition(), str);
	QCOMPARE(doc.lineCount(), 1);
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

QTEST_MAIN(TestStandardDocument)
#include "teststandarddocument.moc"

