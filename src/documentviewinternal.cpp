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

#include "documentviewinternal.h"
#include "documentview.h"
#include "documentcontroller.h"
#include "document.h"
#include "documentrange.h"

#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QFontMetrics>
#include <QTimer>
#include <QCursor>
#include <QMouseEvent>
#include <QRect>

#include <QDebug>

#include "documentviewinternal.moc"

#define CARET_INTERVAL 500

namespace QSourceEdit
{

class TextCursor
	: public DocumentPosition
{

	public:
		TextCursor();
		
		bool is_visible;
		QTimer timer;

};

TextCursor::TextCursor()
	: DocumentPosition(0, 0)
	, is_visible(false)
{
}

DocumentViewInternal::DocumentViewInternal(DocumentView &parentView)
	: QWidget(&parentView)
	, m_view(&parentView)
	, m_startX(0)
	, m_startY(0)
	, m_caret(new TextCursor)
	, m_scrollWheelFactor(1) 
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setFocusPolicy(Qt::ClickFocus);
	setCursor(Qt::IBeamCursor);
	m_caret->timer.setSingleShot(false);
	connect(&m_caret->timer, SIGNAL(timeout()),
		this, SLOT(toggleCaretVisibility()));
	m_caret->timer.start(CARET_INTERVAL);
	connect(&m_view->document(), SIGNAL(textChanged()),
			this, SLOT(documentTextChanged()));
}

int DocumentViewInternal::documentOffsetX() const
{
	return m_startX;
}

int DocumentViewInternal::documentOffsetY() const
{
	return m_startY;
}

const DocumentPosition &DocumentViewInternal::caretPosition() const
{
	return *m_caret;
}

bool DocumentViewInternal::setCaretPosition(const DocumentPosition &pos)
{
	if(pos.line() >= m_view->document().lineCount())
		return false;
	if(pos.column() > m_view->document().lineLength(pos.line()) < pos.column())
		return false;
	
	m_caret->setLine(pos.line());
	m_caret->setColumn(pos.column());
	m_caret->timer.stop();
	m_caret->is_visible = false;
	m_caret->timer.start(CARET_INTERVAL);
	update();
	return true;
}

void DocumentViewInternal::setDocumentOffsetX(int x)
{
	if(x < 0)
		m_startX = 0;
	else
		m_startX = x;
	update();
	emit(documentOffsetXChanged(m_startX));
}

void DocumentViewInternal::setDocumentOffsetY(int y)
{
	if(y < 0)
		m_startY = 0;
	else if(y > (documentOffsetY() - height()))
	{
		m_startY = documentOffsetY() - height();
		if(m_startY < 0)
			m_startY = 0;
	}
	else
		m_startY = y;
	update();
	emit(documentOffsetYChanged(m_startY));
}

void DocumentViewInternal::paintEvent(QPaintEvent *event)
{
	QRect rect = event->rect();
	QPainter paint(this);
	
	unsigned int fontHeight = fontMetrics().height();
	int lineYStart = (documentOffsetY() % fontHeight);
	int lineNumStart = lineAt(documentOffsetY());
	int numLines = (height() / fontHeight) + 2;
	int i;
	// Paint the text
	Document *doc = &m_view->document();
	for(i = 0;i < numLines;i++,lineNumStart++)
	{
		QRect bound = QRect(0, (i*fontHeight) - lineYStart, rect.width(), fontHeight);
		paint.fillRect(bound, Qt::white);
		paint.drawText(bound, doc->text(DocumentRange(
			DocumentPosition(lineNumStart, 0),
			DocumentPosition(lineNumStart, -1))));
	}

	paintCaret(paint);
}

void DocumentViewInternal::resizeEvent(QResizeEvent *event)
{
	emit(sizeChanged(event->size().width(), event->size().height()));
}

void DocumentViewInternal::keyPressEvent(QKeyEvent *event)
{
	m_view->controller().keyPressEvent(event);
}

void DocumentViewInternal::keyReleaseEvent(QKeyEvent *event)
{
	m_view->controller().keyReleaseEvent(event);
}

void DocumentViewInternal::mousePressEvent(QMouseEvent *event)
{
	int pressLine = lineAt(event->y()+documentOffsetY());
	int pressColumn;
	if(pressLine >= m_view->document().lineCount())
	{
		pressLine = m_view->document().lineCount()-1;
	}
	
	QString line = m_view->document().text(
		DocumentRange(
			DocumentPosition(pressLine, 0),
			DocumentPosition(pressLine, -1)));
	// Find the column were at
	int i;
	for(i = 0;i < line.size();i++)
	{
		if(event->x() <= fontMetrics().width(line.left(i+1)))
			break;
	}
	pressColumn = i;
	m_caret->setLine(pressLine);
	m_caret->setColumn(pressColumn);
	m_caret->is_visible = true;
	update();
}

void DocumentViewInternal::wheelEvent(QWheelEvent *event)
{
	event->ignore();
	if(event->orientation() == Qt::Vertical)
	{
		setDocumentOffsetY(documentOffsetY() - (event->delta() / this->m_scrollWheelFactor));
	}
}

void DocumentViewInternal::paintCaret(QPainter &paint)
{
	int startLine = lineAt(documentOffsetY());
	if(!hasFocus())
		return;
	if(startLine > m_caret->line()
	   || lineAt(documentOffsetY()+height()) <= m_caret->line())
		return;
	// Text printed infront of cursor
	QString prevline = m_view->document().text(
		DocumentRange(
			DocumentPosition(m_caret->line(), 0),
			DocumentPosition(m_caret->line(), -1))).left(m_caret->column());
	int xStart = fontMetrics().width(prevline);
	QRect bound = QRect(xStart, (fontMetrics().height()*m_caret->line()) - documentOffsetY(), 1, fontMetrics().height());
	
	if(m_caret->is_visible)
	{
		if(m_caret->column() >= m_view->document().lineLength(m_caret->line()))
			paint.fillRect(bound, Qt::white);
		else
			paint.drawText(bound, QString(m_view->document().text(
				DocumentRange(
					DocumentPosition(m_caret->line(), 0),
					DocumentPosition(m_caret->line(), -1)))[m_caret->column()]));
	}
	else
	{
		paint.fillRect(bound, Qt::black);
	}
}

void DocumentViewInternal::toggleCaretVisibility()
{
	m_caret->is_visible = !m_caret->is_visible;
	update();
}

void DocumentViewInternal::documentTextChanged()
{
	update();
}

int DocumentViewInternal::lineAt(int x) const
{
	return x / fontMetrics().height();
}

}
