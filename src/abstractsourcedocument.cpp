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

#include "abstractsourcedocument.h"

#include <QString>

#include "abstractsourcedocument.moc"

namespace QSourceView
{

class AbstractSourceDocumentPrivate
{
	public:
		QString encoding;
};

AbstractSourceDocument::AbstractSourceDocument(QObject *parent)
	: QObject(parent)
	, d(new AbstractSourceDocumentPrivate)
{
}

AbstractSourceDocument::AbstractSourceDocument(const QString &text,
	QObject *parent)
	: QObject(parent)
	, d(new AbstractSourceDocumentPrivate)
{
}

AbstractSourceDocument::~AbstractSourceDocument()
{
	delete d;
}

QString AbstractSourceDocument::encoding() const
{
	return d->encoding;
}

void AbstractSourceDocument::setEncoding(const QString &encoding)
{
	d->encoding = encoding;
}

}