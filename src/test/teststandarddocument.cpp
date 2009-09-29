#include "teststandarddocument.h"
#include "standarddocument.h"
#include "documentrange.h"
#include "documentposition.h"

void TestStandardDocument::singeLineInsert()
{
	QSourceView::StandardDocument doc;
	QString str("Hello, there.");
	doc.insert(QSourceView::DocumentPosition(), str);
	QVERIFY(doc.lineCount() == 1);
	QVERIFY(doc.text(QSourceView::DocumentRange(QSourceView::DocumentPosition(), QSourceView::DocumentPosition(0, -1))) == str);
}

QTEST_MAIN(TestStandardDocument)
#include "teststandarddocument.moc"

