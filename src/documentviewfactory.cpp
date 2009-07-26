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

#include "documentviewfactory.h"
#include "documentview.h"

#include <QPointer>
#include <QList>

#include "documentviewfactory.moc"

namespace QSourceView
{

class DocumentViewFactoryPrivate
{

	public:
		QList<DocumentView*> views;

};

DocumentViewFactory::DocumentViewFactory(QObject *parent)
	: QObject(parent)
	, d(new DocumentViewFactoryPrivate)
{
}

DocumentViewFactory::~DocumentViewFactory()
{
	delete d;
}

const QList<DocumentView*> DocumentViewFactory::views() const
{
	return d->views;
}

DocumentView *DocumentViewFactory::createView(Document &document) const
{
	return new DocumentView(document);
}

void DocumentViewFactory::insertView(DocumentView &view)
{
	connect(&view, SIGNAL(destroyed(QObject*)),
		this, SLOT(documentViewDestroyed(QObject*)));
	d->views.append(&view);
}

void DocumentViewFactory::documentViewDestroyed(QObject *obj)
{
	d->views.removeAll((DocumentView*)(obj));
}

}
