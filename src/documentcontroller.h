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

#ifndef QSOURCEEDIT_DOCCONTROLLER_H
#define QSOURCEEDIT_DOCCONTROLLER_H

#include <QObject>

#include "documentposition.h"

class QString;
class QKeyEvent;

namespace QSourceEdit
{

class DocumentView;
class Document;

/**
 * @brief Interprets QKeyEvent's into operations on the document.
 */
class DocumentController
	: public QObject
{
	Q_OBJECT

	public:
		DocumentController(DocumentView &view,
			QObject *parent = 0);
		
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);
		Document &document();
		DocumentView &view();
		DocumentPosition &position();
	
	private Q_SLOTS:
		void onTextInserted(const DocumentPosition &pos,
			const QString &text);
	
	private:
		bool insertCaps();
	
		DocumentView *m_view;
		DocumentPosition m_position;
		bool m_shiftPressed;

};

}

#endif
