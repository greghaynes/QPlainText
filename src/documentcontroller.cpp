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
#include <QDebug>

namespace QSourceView
{

DocumentController::DocumentController(DocumentView &view)
	: m_view(&view)
	, m_shiftPressed(false)
{
}

/*
 * First the text is inserted into document, which does not modify the caret.
 * Then the caret is updated.
 */
void DocumentController::keyPressEvent(QKeyEvent *event)
{
	QString insert;
	DocumentRange remove;
	int caretline = view().caretPosition().line();
	int caretcolumn = view().caretPosition().column();
	DocumentPosition pos;
	
	switch(event->key())
	{
		case Qt::Key_Return:
		case Qt::Key_Enter:
			insert = "\n";
			caretline++;
			caretcolumn = 0;
			break;
		case Qt::Key_Up:
			if(caretline)
				caretline--;
			break;
		case Qt::Key_Down:
			caretline++;
			break;
		case Qt::Key_Left:
			if(caretcolumn)
				caretcolumn--;
			break;
		case Qt::Key_Right:
			caretcolumn++;
			break;
		case Qt::Key_Shift:
			m_shiftPressed = true;
			break;
		default:
			insert = event->text();
			caretcolumn++;
	}

	if(!insert.isEmpty())
		document().insertText(view().caretPosition(), insert);

	pos.setLine(caretline);
	pos.setColumn(caretcolumn);
	
	try
	{
		view().setCaretPosition(pos);
	} 
	catch(std::out_of_range)
	{
	}
}

void DocumentController::keyReleaseEvent(QKeyEvent *event)
{
	switch(event->key())
	{
		case Qt::Key_Shift:
			m_shiftPressed = false;
			break;
	}
}

Document &DocumentController::document()
{
	return view().document();
}

DocumentView &DocumentController::view()
{
	return *m_view;
}

bool DocumentController::insertCaps()
{
	return m_shiftPressed;
}

}