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

namespace QPlainText
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
	QString ret;
	int i, startline, startcol, endline, endcol;
	if(isRetrievableRange(range))
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
		column = d->lines.last().size();
	return DocumentPosition(line, column);
}

bool StandardDocument::insert(const DocumentPosition &position,
	const QString &text)
{
	bool ret;
	if((ret = tryInsert(position, text)))
		emit(textInserted(position, text));
	return ret;
}

bool StandardDocument::remove(const DocumentRange &range)
{
	bool ret;
	if((ret = tryRemove(range)))
		emit(textRemoved(range));
	return ret;
}

QString StandardDocument::line(int line) const
{
	return d->lines[line];
}

bool StandardDocument::tryInsert(const DocumentPosition &position,
	const QString &insText)
{
	QStringList insLines;
	QString tailTxt;

	// Check insert position
	if(!isInsertablePosition(position))
	{
		qDebug() << "Attempting to insert text into invalid position: " 
			<< position.line() << "," << position.column();
		return false;
	}

	if(insText.size() <= 0)
		return true;

	insLines = insText.split('\n', QString::KeepEmptyParts);

	qDebug() << "Inserting " << insLines.size() << " lines";

	// insert first line with position's column
	insertSingleLine(position, insLines[0]);

	if(insLines.size() > 1)
	{
		// We have to append line text to last line
		tailTxt = d->lines[position.line()].right(
			d->lines[position.line()].size() - position.column() - insLines[0].length());
		insLines[insLines.size() - 1].append(tailTxt);
		d->lines[position.line()] = d->lines[position.line()].left(position.column() + insLines[0].length());
	}

	for(int i=1;i < insLines.size();i++)
		d->lines.insert(position.line() + i, insLines[i]);

	return true;
}

/* Assumes text contains no newlines, and position is valid */
void StandardDocument::insertSingleLine(const DocumentPosition &position,
	const QString &text)
{
	if(position.line() == d->lines.size())
	{
		if(position.column() == 0)
			d->lines.append(text);
		else
			qDebug() << "Appending new line with non zero column";
	}
	else
	{
		d->lines[position.line()].insert(position.column(), text);
	}
}

bool StandardDocument::tryRemove(const DocumentRange &range)
{
	int startline, endline;
	int startcol, endcol;
	int i;

	qDebug() << "Trying to remove (" << range.start().line() << ", "
			<< range.start().column() << "), ("
			<< range.end().line() << ", " << range.end().column() << ")";

	if(isRemovableRange(range))
	{
		startline = range.start().line();
		startcol = range.start().column();
		endline = range.end().line();
		endcol = range.end().column();

		qDebug() << "Removing (" << startline << ", " << startcol << "), ("
			<< endline << ", " << endcol << ")";

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
	{
		qDebug() << "Attempting to remove from invalid range (" << range.start().line() << ", "
		         << range.start().column() << "), ("
		         << range.end().line() << ", " << range.end().column() << ")";
		return false;
	}
}

bool StandardDocument::isRetrievableRange(const DocumentRange &range) const
{
	DocumentRange t_range(range);
	autoexpandRange(t_range);

	if(!t_range.isValid())
		return false;

	if(t_range.end().line() > lineCount())
	{
		if(!(t_range.end().line() == lineCount() &&
		   t_range.end().column() != 0))
			return false;
	}

	if(t_range.end().column() > lineLength(t_range.end().line()))
		return false;

	return true;
}

bool StandardDocument::isInsertablePosition(const DocumentPosition &pos) const
{
	if(pos.line() < 0 || pos.line() > d->lines.size())
		return false;

	// special case for lastline + 1
	if(pos.line() == d->lines.size())
	{
		if(pos.column() != 0)
			return false;
		return true;
	}

	if(pos.column() < 0 || pos.column() > line(pos.line()).size())
		return false;
	return true;
}

bool StandardDocument::isRemovableRange(const DocumentRange &range) const
{
	return false;
}

bool StandardDocument::isNewline(QChar ch) const
{
	return ch == '\n';
}

void StandardDocument::autoexpandRange(DocumentRange &range) const
{
	autoexpandPosition(range.start());
	autoexpandPosition(range.end());
}

void StandardDocument::autoexpandPosition(DocumentPosition &pos) const
{
	if(pos.line() == -1)
		pos.setLine(lineAutoexpandVal());
	if(pos.column() == -1)
		pos.setColumn(columnAutoexpandVal(pos.line()));
}

int StandardDocument::lineAutoexpandVal(void) const
{
	if(lineCount() == 0)
		return 0;
	return lineCount() - 1;
}

int StandardDocument::columnAutoexpandVal(int line) const
{
	if(line == -1)
		line = lineAutoexpandVal();

	if(line >= lineCount())
		return 0;

	if(lineLength(line) == 0)
		return 0;
	return lineLength(line) - 1;
}

}
