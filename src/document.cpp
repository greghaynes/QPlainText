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

#include "document.h"
#include "documentviewfactory.h"
#include "documentposition.h"

#include <QList>
#include <QString>
#include <QDebug>

#include "document.moc"

namespace QSourceView
{

class DocumentPrivate
{

	public:
		DocumentViewFactory *viewFactory;

};

Document::Document(QObject *parent)
	: QObject(parent)
	, d(new DocumentPrivate)
{
	d->viewFactory = new DocumentViewFactory(this);
}

Document::~Document()
{
	delete d;
}

DocumentViewFactory &Document::viewFactory()
{
	return *d->viewFactory;
}

void Document::setViewFactory(DocumentViewFactory &factory)
{
	DocumentView *view;
	Q_FOREACH(view, viewFactory().views())
		factory.insertView(*view);
	delete d->viewFactory;
	d->viewFactory = &factory;
	factory.setParent(this);
}

void Document::insertText(const DocumentPosition &position,
	const QString &text)
	throw(std::out_of_range)
{
	onInsertText(position, text);
	emit(textInserted(position, text));
	emit(textChanged());
}

void Document::removeText(const DocumentRange &range)
	throw(std::out_of_range)
{
	onRemoveText(range);
	emit(textRemoved(range));
	emit(textChanged());
}

void Document::appendText(const QString &text)
{
	insertText(end(), text);
}

}
