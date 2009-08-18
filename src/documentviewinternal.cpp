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
#include "renderer.h"

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
	setFocusPolicy(Qt::ClickFocus);
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

const DocumentPosition &DocumentViewInternal::caretPosition() const
{
	return *m_caret;
}

void DocumentViewInternal::setCaretPosition(const DocumentPosition &pos)
{
	if(pos.line() < m_view->document().lineCount() || 
	   !m_view->document().text(m_view->document().lineCount()-1).isEmpty() && 
		m_view->document().lineCount() == pos.line())
		m_caret->setLine(pos.line());
	if(pos.column() <= m_view->document().text(caretPosition().line()).size())
		m_caret->setColumn(pos.column());
}

void DocumentViewInternal::setStartX(int x)
{
	if(x < 0)
		m_startX = 0;
	else
		m_startX = x;
	update();
}

void DocumentViewInternal::setStartY(int y)
{
	qDebug() << y;
	if(y < 0)
		m_startY = 0;
	else if(y > (endY() - height()))
	{
		m_startY = endY() - height();
		if(m_startY < 0)
			m_startY = 0;
	}
	else
		m_startY = y;
	update();
	emit(startYChanged(m_startY));
}

void DocumentViewInternal::paintEvent(QPaintEvent *event)
{
	QRect rect = event->rect();
	QPainter paint(this);
	
	unsigned int fontHeight = fontMetrics().height();
	int lineYStart = (startY() % fontHeight);
	int lineNumStart = lineAt(startY());
	int numLines = (height() / fontHeight) + 2;
	int i;
	// Paint the text
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

void DocumentViewInternal::keyPressEvent(QKeyEvent *event)
{
	m_view->controller().keyPressEvent(event);
}

void DocumentViewInternal::mousePressEvent(QMouseEvent *event)
{
	unsigned int pressLine = lineAt(event->y()+startY());
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

void DocumentViewInternal::wheelEvent(QWheelEvent *event)
{
	event->ignore();
	if(event->orientation() == Qt::Vertical)
	{
		setStartY(startY() - (event->delta() / 30));
	}
}

void DocumentViewInternal::paintCaret(QPainter &paint)
{
	int startLine = lineAt(startY());
	if(!hasFocus())
		return;
	if(startLine > m_caret->line()
	   || lineAt(startY()+height()) <= m_caret->line())
		return;
	// Text printed infront of cursor
	QString prevline = m_view->document().text(m_caret->line()).left(m_caret->column());
	int xStart = fontMetrics().width(prevline);
	QRect bound = QRect(xStart, (fontMetrics().height()*m_caret->line()) - startY(), 1, fontMetrics().height());
	
	if(m_caret->is_visible)
	{
		if(m_caret->column() >= m_view->document().text(m_caret->line()).size())
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

int DocumentViewInternal::lineAt(int x) const
{
	return x / fontMetrics().height();
}

}
