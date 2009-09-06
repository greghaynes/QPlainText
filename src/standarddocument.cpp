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
#include <QDebug>

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

QString StandardDocument::text(int line) const
{
	if( line < d->lines.size() )
		return d->lines.at(line);
	else
		return QString();
}

int StandardDocument::lineCount() const
{
	return d->lines.size();
}

int StandardDocument::lineSize(int line) const
{
	return d->lines[line].size();
}

void StandardDocument::clear()
{
	d->lines.clear();
	d->lines.append("");
}

DocumentPosition StandardDocument::end() const
{
	unsigned int line = d->lines.size() - 1;
	unsigned int column;
	if (d->lines.last().size() == 0)
		column = 0;
	else
		column = d->lines.last().size()-1;
	return DocumentPosition(line, column);
}

void StandardDocument::onInsertText(const DocumentPosition &position,
	const QString &insText)
	throw(std::out_of_range, std::runtime_error)
{
	if(insText.size() <= 0)
		return;

	// Make sure the position is valid
	if(!isValidPosition(position))
		throw std::out_of_range("Inserting at invalid position.");

	QStringList insLines = insText.split('\n', QString::KeepEmptyParts);

	if(insLines.size() == 1)
	{
		d->lines[position.line()].insert(position.column(), insText);	
	}
	else if(insLines.size() > 1)
	{
		// Append text on insertion line after insert pos to end last of the insertion text
		insLines.last().append(d->lines[position.line()].right(d->lines[position.line()].size() - position.column()));
	}
	else
		throw std::runtime_error("Insertion list has size of zero.");
}

void StandardDocument::onRemoveText(const DocumentRange &range)
	throw(std::out_of_range, std::runtime_error)
{
}

}
