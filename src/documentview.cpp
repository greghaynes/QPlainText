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

#include "documentview.h"
#include "documentviewfactory.h"
#include "documentviewinternal.h"
#include "renderer.h"
#include "document.h"
#include "numberedlistwidget.h"

#include <QHBoxLayout>
#include <QTextEdit>
#include <QFont>
#include <QScrollBar>

#include "documentview.moc"

namespace QSourceView
{

class DocumentViewPrivate
{

	public:
		Document *document;
		Renderer *renderer;
		DocumentViewInternal *internalView;
		NumberedListWidget *horiz_numbers;
		NumberedListWidget *vert_numbers;
		QScrollBar *horiz_scrollBar;
		QScrollBar *vert_scrollBar;

};

DocumentView::DocumentView(Document &document)
	: QWidget(0)
	, d(new DocumentViewPrivate)
{
	document.viewFactory().insertView(*this);
	d->document = &document;
	d->renderer = new Renderer(this);
	d->internalView = new DocumentViewInternal(*this, *d->renderer);
	d->horiz_scrollBar = new QScrollBar(Qt::Horizontal);
	d->vert_scrollBar = new QScrollBar(Qt::Vertical);
	d->vert_scrollBar->setMaximum(document.lineCount());
	d->horiz_scrollBar->setVisible(false);
	setupUi();
}

DocumentView::~DocumentView()
{
	delete d;
}

Document &DocumentView::document()
{
	return *d->document;
}

void DocumentView::enableHorizontalNumberWidget()
{
}

void DocumentView::enableVerticalNumberWidget()
{
}

void DocumentView::setupUi()
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

Renderer &DocumentView::renderer()
{
	return *d->renderer;
}

}