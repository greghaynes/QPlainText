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

QString StandardDocument::text(unsigned int line) const
{
	if( line < (unsigned int)d->lines.size() )
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
{
	// Make sure the position is valid
	if((lineCount()-1) < position.line()
	   || (text(position.line()).size()) < position.column())
		return;
	
	QStringList insList;
	if(insText[0] == '\n')
	{
		qDebug() << "Newline";
		insList.prepend("");
	}
	insList.append(insText.split('\n'));
	qDebug() << "Inserting " << insList.size() << " lines from " << insText;
	// Append first line to position line
	d->lines[position.line()].insert(position.column(), insList[0]);

	// Append bulk of lines after position line
	if(insList.size() > 1)
	{
		if(insList.size() > 2)
		{
			int i;
			for(i = 1;i < insList.size()-1; i++)
				d->lines.insert(position.line() + i, insList[i]);
		}
		// We want any text after insert to be appended on that line
		unsigned int endLine = position.line() + insList.size();
		if((lineCount()-1) < endLine)
			d->lines.append(insList.last());
		else
			d->lines[position.line()+insList.size()].prepend(insList.last());
	}
}

void StandardDocument::onRemoveText(const DocumentRange &range)
{
}

}
