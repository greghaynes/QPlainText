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

#include "standarddocument.h"

#include <QString>
#include <QStringList>
#include <QList>

namespace QSourceView
{

class StandardDocumentPrivate
{

	public:
		QList<QString> lines;

};

StandardDocument::StandardDocument(QObject *parent)
	: Document(parent)
	, d(new StandardDocumentPrivate)
{
	d->lines.append("");
}

StandardDocument::~StandardDocument()
{
	delete d;
}

QString StandardDocument::text(unsigned int line) const
{
	if( line < d->lines.size() )
	{
		return d->lines.at(line);
	}
	else
	{
		return QString();
	}
}

unsigned int StandardDocument::lineCount() const
{
	return d->lines.size();
}

unsigned int StandardDocument::lineSize(unsigned int line) const
{
	return d->lines[line].size();
}

void StandardDocument::clear()
{
	d->lines.clear();
}

DocumentPosition StandardDocument::end() const
{
	if(d->lines.size() > 0)
		return DocumentPosition(d->lines.size()-1, d->lines[d->lines.size()-1].size());
	else
		return DocumentPosition(0, 0);
}

void StandardDocument::onInsertText(const DocumentPosition &position,
	const QString &text)
{
}

void StandardDocument::onRemoveText(const DocumentRange &range)
{
}

}