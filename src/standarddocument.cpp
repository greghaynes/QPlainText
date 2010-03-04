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

namespace QSourceEdit
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
}

StandardDocument::~StandardDocument()
{
	delete d;
}

QString StandardDocument::text(const DocumentRange &range) const
{
	QString ret = 0;
	int i, startline, startcol, endline, endcol;
	if(isValidPosition(range.start()) && isValidPosition(range.end(), false, true))
	{
		startline = range.start().line();
		startcol = range.start().column();
		endline = range.end().line();
		endcol = range.end().column();
		if(endcol == -1)
			endcol = d->lines[endline].length();
		if(startline == endline)
		{
			if(endcol == -1)
			{
				ret.append(d->lines[startline]);
				ret.remove(0, startcol);
			}
			else
			{
				ret.append(d->lines[startline].mid(startcol, endcol - startcol));
			}
		}
		else
		{
			ret.append(d->lines[startline]);
			ret.remove(0, startcol);
			ret.append("\n");
			for(i = (startline+1); i < endline; i++)
			{
				ret.append(d->lines[i]);
				ret.append("\n");
			}
			ret.append(d->lines[endline].left(endcol));
		}

	}
	else
	{
		qDebug() << "Attempt to retrieve text from invalid position "
			<< range.start().line() << ":" << range.start().column()
			<< " - " << range.end().line() << ":" << range.end().column();
	}
	return ret;
}

int StandardDocument::lineCount() const
{
	return d->lines.size();
}

int StandardDocument::lineLength(int line) const
{
	if(d->lines.size() > line)
		return d->lines[line].size();
	return -1;
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
	if (line == -1)
		return DocumentPosition();
	if (d->lines.last().size() == 0)
		column = 0;
	else
		column = d->lines.last().size()-1;
	return DocumentPosition(line, column);
}

bool StandardDocument::onInsertText(const DocumentPosition &position,
	const QString &insText)
{
	// Check insert position
	if(position.line() < 0
	   || position.line() > d->lines.size()
	   || (position.line() < d->lines.size() 
			&& position.column() > d->lines[position.line()].size())
	   || (position.line() == d->lines.size() && position.column() != 0))
	{
		qDebug() << "Attempting to insert text into invalid position: " 
			<< position.line() << "," << position.column();
		return false;
	}

	if(insText.size() <= 0)
		return true;

	QStringList insLines = insText.split('\n', QString::KeepEmptyParts);
	if(insLines.size() == 1)
	{
		if(d->lines.size() > position.line())
			d->lines[position.line()].insert(position.column(), insText);	
		else
			d->lines.insert(position.line(), insText);
	}
	else if(insLines.size() > 1)
	{
		QString chop;
		if(d->lines.length() > position.line())
		{
			int choplen = d->lines[position.line()].length() - position.column();
			QString chop = d->lines[position.line()].right(choplen);
			d->lines[position.line()].chop(choplen);
		}
		insLines.last().append(chop);
		if(insLines.last().isEmpty())
			insLines.removeLast();
		int i = position.line() + 1;
		for(i = 0;i < insLines.length();i++)
		{
			d->lines.insert(i, insLines[i]);
		}
	}
	else
		return false;
	return true;
}

bool StandardDocument::onRemoveText(const DocumentRange &range)
{
	int startline, endline;
	int startcol, endcol;
	int i;

	if(isValidPosition(range.start()) && isValidPosition(range.end(), false, true))
	{
		startline = range.start().line();
		startcol = range.start().column();
		endline = range.end().line();
		endcol = range.end().column();
		if(startline == endline)
		{
			if(endcol == -1)
				d->lines[startline].remove(startcol, d->lines[startline].length() - startcol);
			else
				d->lines[startline].remove(startcol, endcol - startcol);
		}
		else
		{
			d->lines[startline].remove(startcol, endcol - startcol);
			startline += 1;
			for(i = startline;i < endline;i++)
				d->lines.removeAt(startline);
			d->lines[startline-1].append(d->lines[startline].right(d->lines[startline].length() - endcol));
			d->lines.removeAt(startline);
		}
		return true;
	}
	else
		return false;
}

bool StandardDocument::isValidPosition(const DocumentPosition &pos,
	bool lines_plus_one,
	bool allow_end_col) const
{
	if(lines_plus_one)
	{
		if(pos.line() >= 0 && pos.line() <= d->lines.size())
		{
			if(allow_end_col && pos.column() == -1)
				return true;
			else if(pos.column() == 0)
				return true;
			else
				return pos.column() > 0
					&& pos.column() < d->lines[pos.line()].size();
		}
	}
	else
	{
		if(pos.line() >= 0 && pos.line() < d->lines.size())
		{
			if(allow_end_col && pos.column() == -1)
				return true;
			else if(pos.column() == 0)
				return true;
			else
				return pos.column() > 0
					&& pos.column() < d->lines[pos.line()].size();
		}
	}
	return false;
}

bool StandardDocument::isNewline(QChar ch) const
{
	return ch == '\n';
}

}
