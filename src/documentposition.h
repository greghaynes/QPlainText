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

#ifndef QSOURCEVIEW_DOCUMENT_POSITION_H
#define QSOURCEVIEW_DOCUMENT_POSITION_H

namespace QSourceEdit
{

class DocumentPosition
{

	public:
		DocumentPosition();
		
		/**
		 * Counting starts at 0
		 */
		DocumentPosition(int line,
			int column);
		
		DocumentPosition &operator=(const DocumentPosition &other);
		bool operator==(const DocumentPosition &other) const;
		bool operator<(const DocumentPosition &other) const;
		bool operator>(const DocumentPosition &other) const;
		bool operator<=(const DocumentPosition &other) const;
		bool operator>=(const DocumentPosition &other) const;
		
		int line() const;
		int column() const;
		void setLine(int line);
		void setColumn(int column);
		
	private:
		int m_line;
		int m_column;

};

}

#endif
