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

#define DOC_CHECK_POS(x) \
	if (!isValidPosition(x)) \
		throw std::out_of_range("Inserting at invalid position.");

void StandardDocument::onInsertText(const DocumentPosition &position,
	QChar ch)
	throw(std::out_of_range, std::runtime_error)
{
	DOC_CHECK_POS(position);

	if(isNewline(ch))
	{
		// TODO: Make this understandable to muggles
		QString chop = d->lines[position.line()].right(d->lines[position.line()].length()-position.column());
		chop.prepend(ch);
		d->lines.insert(position.line()+1, chop);
	}
	else
		d->lines[position.line()].insert(position.column(), ch);
}

// TODO: Finish/Test this
void StandardDocument::onInsertText(const DocumentPosition &position,
	const QString &insText)
	throw(std::out_of_range, std::runtime_error)
{
	if(insText.size() <= 0)
		return;
	DOC_CHECK_POS(position);

	QStringList insLines = insText.split('\n', QString::KeepEmptyParts);
	if(insLines.size() == 1)
		d->lines[position.line()].insert(position.column(), insText);	
	else if(insLines.size() > 1)
	{
		int choplen = d->lines[position.line()].length() - position.column();
		QString chop = d->lines[position.line()].right(choplen);
		d->lines[position.line()].chop(choplen);
		insLines[0].append(chop);
		if(insLines.last().isEmpty())
			insLines.removeLast();
		int i = position.line() + 1;
		foreach(chop, insLines)
		{
			d->lines.insert(i, chop);
			i++;
		}
	}
	else
		throw std::runtime_error("Insertion list has size of zero.");
}

#undef DOC_CHECK_POS

void StandardDocument::onRemoveText(const DocumentRange &range)
	throw(std::out_of_range, std::runtime_error)
{
}

bool StandardDocument::isNewline(QChar ch) const
{
	return ch == '\n';
}

}
