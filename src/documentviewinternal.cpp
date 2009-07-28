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
#include "document.h"
#include "renderer.h"

#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QFontMetrics>
#include <QTimer>
#include <QCursor>
#include <QMouseEvent>
#include <QRect>

#include "documentviewinternal.moc"

namespace QSourceView
{

class TextCursor
	: public DocumentPosition
{

	public:
		TextCursor();
		
		bool is_visible;

};

TextCursor::TextCursor()
	: DocumentPosition(0, 0)
	, is_visible(false)
{
}

DocumentViewInternal::DocumentViewInternal(DocumentView &parentView,
	Renderer &renderer)
	: QWidget(&parentView)
	, m_view(&parentView)
	, m_renderer(&renderer)
	, m_startX(0)
	, m_startY(0)
	, m_caret(new TextCursor)
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setCursor(Qt::IBeamCursor);
	QTimer *caretTimer = new QTimer(this);
	caretTimer->setSingleShot(false);
	connect(caretTimer, SIGNAL(timeout()),
		this, SLOT(toggleCaretVisibility()));
	caretTimer->start(500);
}

int DocumentViewInternal::startX() const
{
	return m_startX;
}

int DocumentViewInternal::startY() const
{
	return m_startY;
}

int DocumentViewInternal::endY() const
{
	return m_view->document().lineCount() * fontMetrics().height();
}

void DocumentViewInternal::setStartX(int x)
{
	m_startX = x;
	update();
}

void DocumentViewInternal::setStartY(int y)
{
	m_startY = y;
	update();
}

void DocumentViewInternal::paintEvent(QPaintEvent *event)
{
	QRect rect = event->rect();
	QPainter paint(this);
	
	unsigned int fontHeight = fontMetrics().height();
	int lineYStart = (startY() % fontHeight);
	int lineNumStart = startY() / fontHeight;
	int numLines = (height() / fontHeight) + 2;
	int i;
	Document *doc = &m_view->document();
	for(i = 0;i < numLines;i++,lineNumStart++)
	{
		QRect bound = QRect(0, (i*fontHeight) - lineYStart, rect.width(), fontHeight);
		paint.fillRect(bound, Qt::white);
		paint.drawText(bound, doc->text(lineNumStart));
	}

	paintCaret(paint);
}

void DocumentViewInternal::resizeEvent(QResizeEvent *event)
{
	emit(sizeChanged(event->size().width(), event->size().height()));
}

void DocumentViewInternal::mousePressEvent(QMouseEvent *event)
{
	unsigned int pressLine = event->y() / fontMetrics().height();
	unsigned int pressColumn;
	if(pressLine >= m_view->document().lineCount())
	{
		pressLine = m_view->document().lineCount()-1;
	}
	
	QString line = m_view->document().text(pressLine);
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

void DocumentViewInternal::paintCaret(QPainter &paint)
{
	// Text printed infront of cursor
	QString prevline = m_view->document().text(m_caret->line()).left(m_caret->column());
	int xStart = fontMetrics().width(prevline);
	QRect bound = QRect(xStart, fontMetrics().height()*m_caret->line(), 1, fontMetrics().height());
	
	if(m_caret->is_visible)
	{
		if(m_caret->column() >= (unsigned int)m_view->document().text(m_caret->line()).size())
			paint.fillRect(bound, Qt::white);
		else
			paint.drawText(bound, QString(m_view->document().text(m_caret->line())[m_caret->column()]));
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

unsigned int DocumentViewInternal::lineAt(unsigned int x) const
{
	
}

}
