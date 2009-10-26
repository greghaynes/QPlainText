#include "testdocumentview.h"
#include "standarddocument.h"
#include "documentview.h"

void TestDocumentView::empty()
{
	QSourceEdit::Document *doc = new QSourceEdit::StandardDocument();
	QSourceEdit::DocumentView view(*doc);

	QCOMPARE(doc->lineLength(0), 0);
	delete doc;
}

void TestDocumentView::typeHello()
{
}

QTEST_MAIN(TestDocumentView)
#include "testdocumentview.moc"

