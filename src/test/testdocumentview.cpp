#include "testdocumentview.h"
#include "standarddocument.h"
#include "documentview.h"
#include "documentrange.h"
#include "documentcontroller.h"

#include <QKeyEvent>

void TestDocumentView::empty()
{
	QSourceEdit::Document *doc = new QSourceEdit::StandardDocument();
	QSourceEdit::DocumentView view(*doc);

	QCOMPARE(doc->lineLength(0), -1);
	delete doc;
}

void TestDocumentView::typeHello()
{
	delete (int*)0;
	QSourceEdit::Document *doc = new QSourceEdit::StandardDocument();
	QSourceEdit::DocumentView view(*doc);
	QString str("Hello");
	int i;

	for(i = 0; i < str.size(); i++)
	{
		QKeyEvent *ev = new QKeyEvent(QEvent::KeyPress, str.at(i).digitValue(), Qt::NoModifier, QString(str[i]));
		view.controller().keyPressEvent(ev);
		delete ev;
	}

	QCOMPARE(doc->lineLength(0), 5);
	QCOMPARE(doc->text(
		QSourceEdit::DocumentRange(
			QSourceEdit::DocumentPosition(0, 0),
			QSourceEdit::DocumentPosition(0, -1))), str);

	delete doc;
}

QTEST_MAIN(TestDocumentView)
#include "testdocumentview.moc"

