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

#ifndef QSOURCEVIEW_DOCUMENT_CARET_H
#define QSOURCEVIEW_DOCUMENT_CARET_H

#include "documentposition.h"

#include <QObject>

namespace QSourceEdit
{

/**
  * @brief Represents a caret in a document view.
  */
class DocumentCaret
	: public QObject
	, public DocumentPosition
{
	Q_OBJECT

	public:
		/**
		  * @brief Create a DocumentCaret.
		  */
		DocumentCaret(QObject *parent = 0);
		
		/**
		  * @brief Create a DocumentCaret.
		  */
		DocumentCaret(int line,
			int column,
			QObject *parent);
	
	Q_SIGNALS:
		/**
		  * @brief The position of the caret has changed.
		  */
		void positionChanged(DocumentCaret *self = 0);
	
	protected:
		void onSetLine(int line);
		void onSetColumn(int line);

};

}

#endif
