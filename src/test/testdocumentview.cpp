#include "testdocumentview.h"
#include "standarddocument.h"
#include "standarddocumentview.h"
#include "documentrange.h"
#include "keyboardhandler.h"

#include <QKeyEvent>

void TestDocumentView::empty()
{
	QPlainText::Document *doc = new QPlainText::StandardDocument();
	QPlainText::StandardDocumentView view(*doc);

	QCOMPARE(doc->lineLength(0), -1);
	delete doc;
}

void TestDocumentView::typeHello()
{
	delete (int*)0;
	QPlainText::Document *doc = new QPlainText::StandardDocument();
	QPlainText::StandardDocumentView view(*doc);
	QString str("Hello");
	int i;

	for(i = 0; i < str.size(); i++)
	{
		QKeyEvent *ev = new QKeyEvent(QEvent::KeyPress, str.at(i).digitValue(), Qt::NoModifier, QString(str[i]));
		view.keyboardHandler().keyPressEvent(ev);
		delete ev;
	}

	QCOMPARE(doc->lineLength(0), 5);
	QCOMPARE(doc->text(
		QPlainText::DocumentRange(
			QPlainText::DocumentPosition(0, 0),
			QPlainText::DocumentPosition(0, -1))), str);

	delete doc;
}

QTEST_MAIN(TestDocumentView)
#include "testdocumentview.moc"

