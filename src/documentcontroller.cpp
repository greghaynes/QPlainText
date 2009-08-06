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

#include "documentcontroller.h"
#include "document.h"
#include "documentview.h"
#include "documentposition.h"
#include "documentrange.h"

#include <QKeyEvent>

namespace QSourceView
{

DocumentController::DocumentController(DocumentView &view)
	: m_view(&view)
{
}

void DocumentController::keyPressEvent(QKeyEvent *event)
{
	QString insert;
	bool is_insert = false;
	bool is_remove = false;
	int col_advance = 0;
	int line_advance = 0;
	DocumentPosition pos;
	
	switch(event->key())
	{
		case Qt::Key_Return:
		case Qt::Key_Enter:
			insert = "\n";
			line_advance = 1;
			is_insert = true;
			break;
		case Qt::Key_Up:
			line_advance = -1;
			break;
		case Qt::Key_Down:
			line_advance = 1;
			break;
		case Qt::Key_Left:
			col_advance = -1;
			break;
		case Qt::Key_Right:
			col_advance = 1;
		default:
			insert = event->text();
			col_advance = 1;
	}
	// The unsigned int wrapping causes these to work for positive and negative ends
	if(!((line_advance+view().caretPosition().line()) > document().lineCount()))
		pos.setLine(view().caretPosition().line()+line_advance);
	else
		pos.setLine(view().caretPosition().line());
	if(!((col_advance+view().caretPosition().column()) > document().text(pos.line()).size()))
		pos.setColumn(view().caretPosition().column()+col_advance);
	else
		pos.setColumn(view().caretPosition().column());
	if(is_insert)
	{
	}
	else if(is_remove)
	{
	}
	view().setCaretPosition(pos);
}

Document &DocumentController::document()
{
	return view().document();
}

DocumentView &DocumentController::view()
{
	return *m_view;
}

}