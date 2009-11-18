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

#include "documentposition.h"

#include "documentposition.moc"

namespace QSourceEdit
{

DocumentPosition::DocumentPosition(QObject *parent)
	: QObject(parent)
	, m_line(0)
	, m_column(0)
{
}

DocumentPosition::DocumentPosition(int line,
	int column,
	QObject *parent)
	: QObject(parent)
	, m_line(line)
	, m_column(column)
{
}

DocumentPosition::DocumentPosition(const DocumentPosition &other)
	: QObject(other.parent())
	, m_line(other.line())
	, m_column(other.column())
{
}

DocumentPosition &DocumentPosition::operator=(const DocumentPosition &other)
{
	m_line = other.line();
	m_column = other.column();
	return *this;
}

bool DocumentPosition::operator==(const DocumentPosition &other) const
{
	return line() == other.line() && column() == other.column();
}

bool DocumentPosition::operator<(const DocumentPosition &other) const
{
	if(other.line() == line())
		return column() < other.column();
	else
		return line() < other.line();
}

bool DocumentPosition::operator>(const DocumentPosition &other) const
{
	if(other.line() == line())
		return column() > other.column();
	else
		return line() > other.line();
}

bool DocumentPosition::operator<=(const DocumentPosition &other) const
{
	if(other.line() == line())
		return column() <= other.column();
	else
		return line() <= other.line();
}

bool DocumentPosition::operator>=(const DocumentPosition &other) const
{
	if(other.line() == line())
		return column() >= other.column();
	else
		return line() >= other.line();
}

int DocumentPosition::line() const
{
	return m_line;
}

int DocumentPosition::column() const
{
	return m_column;
}

void DocumentPosition::setLine(int line)
{
	m_line = line;
}

void DocumentPosition::setColumn(int column)
{
	m_column = column;
}

}
