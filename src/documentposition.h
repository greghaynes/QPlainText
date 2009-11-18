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

/**
  * @brief Represents a position in a document
  */
class DocumentPosition
{

	public:
		/**
		  * @brief Create a document position instance.
		  *
		  * Line and column are initialized to 0.
		  */
		DocumentPosition();
		
		/**
		  * @brief Create a document position instance.
		  *
		  * Counting starts at 0
		  */
		DocumentPosition(int line,
			int column);
		
		/**
		  * @brief Make copy of other position
		  */
		DocumentPosition(const DocumentPosition &other);
		
		/**
		  * @brief Destroy the DocumentPosition.
		  */
		virtual ~DocumentPosition();
		
		/**
		  * @brief Assign position value of other position.
		  */
		DocumentPosition &operator=(const DocumentPosition &other);
		
		/**
		  * @brief Does position point to same location as other position.
		  */
		bool operator==(const DocumentPosition &other) const;
		
		/**
		  * @brief Is position pointing to location infront of other position.
		  */
		bool operator<(const DocumentPosition &other) const;
		
		/**
		  * @brief Is position pointing to location after other position.
		  */
		bool operator>(const DocumentPosition &other) const;
		
		/**
		  * @brief Is positon pointing before or at other position.
		  */
		bool operator<=(const DocumentPosition &other) const;
		
		/**
		  * @brief Is position pointing after or at other position.
		  */
		bool operator>=(const DocumentPosition &other) const;
		
		/**
		  * @brief Line of position.
		  */
		int line() const;
		
		/**
		  * @brief Column of position.
		  */
		int column() const;
		
		/**
		  * @brief Set the line of the position.
		  */
		void setLine(int line);
		
		/**
		  * @brief Set the column of the position.
		  */
		void setColumn(int column);
	
	protected:
		/**
		  * @brief Called after line number changes with old line value.
		  *
		  * This is meant to allow subclasses to implement signals on 
		  * position changed while keeping this class lightweight.
		  */
		virtual void onSetLine(int line) {}
		
		/**
		  * @brief Called after column changes with old column value.
		  *
		  * This is meant to allow subclasses to implement signals on 
		  * position changed while keeping this class lightweight.
		  */
		virtual void onSetColumn(int coulumn) {}
	
	private:
		int m_line;
		int m_column;

};

}

#endif
