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

#include "keyboardhandler.h"
#include "document.h"
#include "documentview.h"
#include "documentrange.h"

#include <QKeyEvent>
#include <QDebug>

#include "keyboardhandler.moc"

namespace QPlainText
{

KeyboardHandler::KeyboardHandler(DocumentView &view)
	: QObject(&view)
	, m_view(&view)
	, m_shiftPressed(false)
{
	connect(&view.document(), SIGNAL(textInserted(
			const DocumentPosition&, const QString &)),
		this, SLOT(onTextInserted(const DocumentPosition&, const QString&)));
}

/*
 * First the text is inserted into document, which does not modify the caret.
 * Then the caret is updated.
 */
void KeyboardHandler::keyPressEvent(QKeyEvent *event)
{
	QString insert;
	DocumentRange remove;
	int caretline = view().keyboardCaret().line();
	int caretcolumn = view().keyboardCaret().column();
	DocumentPosition pos;
	Document *doc = &view().document();
	
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
			if(caretcolumn >= doc->lineLength(caretline))
				caretcolumn = doc->lineLength(caretline) - 1;
			if(caretcolumn < 0)
				caretcolumn = 0;
			break;
		case Qt::Key_Down:
			if(caretline < (doc->lineCount() - 1))
				caretline++;
			if(caretcolumn >= doc->lineLength(caretline))
				caretcolumn = doc->lineLength(caretline) - 1;
			if(caretcolumn < 0)
				caretcolumn = 0;
			break;
		case Qt::Key_Left:
			if(caretcolumn)
				caretcolumn--;
			break;
		case Qt::Key_Right:
			if(caretcolumn < doc->lineLength(caretline))
				caretcolumn++;
			break;
		case Qt::Key_Shift:
			m_shiftPressed = true;
			break;
		case Qt::Key_Backspace:
			if(caretcolumn <= 0)
			{
				if(caretline == 0)
					break;
				remove.start().setLine(caretline - 1);
				remove.start().setColumn(-1);
				remove.end().setLine(caretline);
				remove.end().setColumn(caretcolumn);
				caretline--;
				caretcolumn;
			}
			else
			{
				remove.start().setLine(caretline);
				remove.start().setColumn(caretcolumn -1);
				remove.end().setLine(caretline);
				remove.end().setColumn(caretcolumn);
				caretcolumn--;
			}
			break;
		default:
			insert = event->text();
			caretcolumn++;
	}

	if(!insert.isEmpty())
		view().document().insert(view().keyboardCaret(), insert);
	if(!remove.isEmpty())
		view().document().remove(remove);
	view().keyboardCaret().setLine(caretline);
	view().keyboardCaret().setColumn(caretcolumn);
}

void KeyboardHandler::keyReleaseEvent(QKeyEvent *event)
{
	switch(event->key())
	{
		case Qt::Key_Shift:
			m_shiftPressed = false;
			break;
	}
}

DocumentView &KeyboardHandler::view()
{
	return *m_view;
}

void KeyboardHandler::onTextInserted(const DocumentPosition &pos,
	const QString &text)
{
	DocumentPosition *kbdPos = &view().keyboardCaret();
	if(pos.line() == kbdPos->line()
	   && pos.column() <= kbdPos->column())
		kbdPos->setColumn(kbdPos->column()+1);
}

bool KeyboardHandler::insertCaps()
{
	return m_shiftPressed;
}

}
