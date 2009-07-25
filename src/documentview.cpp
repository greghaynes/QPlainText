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
#include "numberedlistwidget.h"

#include <QHBoxLayout>
#include <QTextEdit>

#include "documentview.moc"

namespace QSourceView
{

class DocumentViewPrivate
{

	public:
		QTextEdit *textEdit;
		NumberedListWidget *horiz_numbers;
		NumberedListWidget *vert_numbers;

};

DocumentView::DocumentView(QTextDocument &document)
	: QWidget(0)
	, d(new DocumentViewPrivate)
{
	d->textEdit = new QTextEdit();
	d->textEdit->setDocument(&document);
	setupUi();
}

DocumentView::~DocumentView()
{
	delete d;
}

void DocumentView::setupUi()
{
	QHBoxLayout *hlayout = new QHBoxLayout(this);
	hlayout->addWidget(d->textEdit);
	setLayout(hlayout);
}

void DocumentView::enableHorizontalNumberWidget()
{
}

void DocumentView::enableVerticalNumberWidget()
{
}

}