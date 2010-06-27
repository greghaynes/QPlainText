/*
 *   Copyright (C) 2009 Gregory Haynes <greg@greghaynes.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "standarddocumentview.h"
#include "standarddocumentviewinternal.h"
#include "keyboardhandler.h"
#include "caret.h"
#include "document.h"
#include "numberedlistwidget.h"

#include <QHBoxLayout>
#include <QTextEdit>
#include <QFont>
#include <QScrollBar>
#include <QDebug>

namespace QPlainText
{

class StandardDocumentViewPrivate
{

	public:
		StandardDocumentViewInternal *internalView;
		Caret *keyboardCaret;
		//NumberedListWidget *horiz_numbers;
		//NumberedListWidget *vert_numbers;
		QScrollBar *horiz_scrollBar;
		QScrollBar *vert_scrollBar;

};

StandardDocumentView::StandardDocumentView(Document &document)
	: DocumentView(document)
	, d(new StandardDocumentViewPrivate)
{
	d->internalView = new StandardDocumentViewInternal(*this);
	
	// Create keyboard caret
	d->keyboardCaret = new Caret();
	
	setupScrollBars();
	setupUi();
	setupSignals();
}

StandardDocumentView::~StandardDocumentView()
{
	// Delete carets
	DocumentPosition *p;

	delete d;
}

Caret &StandardDocumentView::keyboardCaret()
{
	return *d->keyboardCaret;
}

void StandardDocumentView::setInternalFont(const QFont &font)
{
	d->internalView->setFont(font);
}

void StandardDocumentView::setScrollBarEnabled(bool value, Qt::Orientation orientation)
{
}

void StandardDocumentView::documentTextInserted(const DocumentPosition &pos,
	const QString &text)
{
	Q_UNUSED(pos)
	Q_UNUSED(text)
	documentChanged();
}

void StandardDocumentView::documentTextRemoved(const DocumentRange &range)
{
	Q_UNUSED(range)
	documentChanged();
}

void StandardDocumentView::slotInternalViewResize(int width, int height)
{
	Q_UNUSED(width)
	Q_UNUSED(height)
	resizeScrollbar();
}

void StandardDocumentView::setupSignals()
{
	connect(&document(), SIGNAL(destroyed(QObject*)),
		this, SLOT(deleteLater()));
	connect(&document(), SIGNAL(textInserted(const DocumentPosition&, const QString&)),
		this, SLOT(documentTextInserted(const DocumentPosition&, const QString &)));
	connect(&document(), SIGNAL(textRemoved(const DocumentRange&)),
		this, SLOT(documentTextRemoved(const DocumentRange&)));
	connect(d->internalView, SIGNAL(sizeChanged(int, int)),
		this, SLOT(slotInternalViewResize(int, int)));
	connect(d->vert_scrollBar, SIGNAL(sliderMoved(int)),
		d->internalView, SLOT(setStartY(int)));
	connect(d->internalView, SIGNAL(startYChanged(int)),
		d->vert_scrollBar, SLOT(setValue(int)));
}

void StandardDocumentView::setupScrollBars()
{
	d->horiz_scrollBar = new QScrollBar(Qt::Horizontal);
	d->vert_scrollBar = new QScrollBar(Qt::Vertical);
	d->vert_scrollBar->setMaximum(d->internalView->documentOffsetY());
	d->vert_scrollBar->setSingleStep(1);
	d->horiz_scrollBar->setVisible(false);
}

void StandardDocumentView::setupUi()
{
	QHBoxLayout *hlayout = new QHBoxLayout();
	QVBoxLayout *vlayout = new QVBoxLayout(this);
	hlayout->setContentsMargins(0, 0, 0, 0);
	hlayout->setSpacing(0);
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->setSpacing(0);
	hlayout->addWidget(d->internalView);
	hlayout->addWidget(d->vert_scrollBar);
	vlayout->addLayout(hlayout);
	vlayout->addWidget(d->horiz_scrollBar);
	setLayout(vlayout);
}

void StandardDocumentView::resizeScrollbar()
{
	int endY = d->internalView->documentOffsetY();
	if(endY < 0)
	{
		d->vert_scrollBar->setVisible(false);
	}
	else
	{
		d->vert_scrollBar->setVisible(true);
		d->vert_scrollBar->setRange(0, endY);
	}
}

void StandardDocumentView::documentChanged()
{
	resizeScrollbar();
	d->internalView->update();
}

}

#include "standarddocumentview.moc"

