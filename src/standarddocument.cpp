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
#include "documentrange.h"

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

QString *StandardDocument::text(const DocumentRange &range) const
{
	QString *ret = 0;
	int i, j, startline, startcol, endline, endcol, lineoff;
	if(isValidPosition(range.start()) && isValidPosition(range.end()))
	{
		ret = new QString();
		startline = range.start().line();
		startcol = range.start().column();
		endline = range.end().line();
		endcol = range.end().column();
		if(startline == endline)
		{
			if(endcol == -1)
			{
				ret->append(d->lines[startline].right(startcol));
			}
			else
			{
				ret->append(d->lines[startline].mid(startcol, endcol - startcol));
			}
		}
		else
		{
			ret->append(d->lines[startline].right(startcol));
			for(i = startline+1; i < endline; i++)
			{
				ret->append(d->lines[i]);
			}
			ret->append(d->lines[endline].left(endcol));
		}

	}
	return ret;
}

int StandardDocument::lineCount() const
{
	return d->lines.size();
}

int StandardDocument::lineLength(int line) const
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
	if(!isValidPosition(x)) \
		return false;

// TODO: Finish/Test this
bool StandardDocument::onInsertText(const DocumentPosition &position,
	const QString &insText)
{
	if(insText.size() <= 0)
		return true;
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
		return false;
}

#undef DOC_CHECK_POS

void StandardDocument::onRemoveText(const DocumentRange &range)
{
}

bool StandardDocument::isValidPosition(const DocumentPosition &pos) const
{
	if(pos.line() > 0 && pos.line() < d->lines.size())
			return pos.column() > 0 && pos.column() < d->lines[pos.line()].size();
	return false;
}

bool StandardDocument::isNewline(QChar ch) const
{
	return ch == '\n';
}

}
