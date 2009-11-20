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
#include "documentcaret.h"

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

DocumentViewInternal::DocumentViewInternal(DocumentView &parentView)
	: QWidget(&parentView)
	, m_view(&parentView)
	, m_startX(0)
	, m_startY(0)
	, m_scrollWheelFactor(1) 
{
	setupUi();
	setupSignals();
}

int DocumentViewInternal::documentOffsetX() const
{
	return m_startX;
}

int DocumentViewInternal::documentOffsetY() const
{
	return m_startY;
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
	Document *doc = &m_view->document();
	// Where do we want to stop painting text
	int textLines = (numLines + lineYStart) <= doc->lineCount() ? numLines : doc->lineCount();
	
	// Paint the text
	for(i = 0;i < numLines;i++,lineNumStart++)
	{
		QRect bound = QRect(0, (i*fontHeight) - lineYStart, rect.width(), fontHeight);
		paint.fillRect(bound, Qt::white);
		if(textLines > i)
		{
			paint.drawText(bound, doc->text(DocumentRange(
				DocumentPosition(lineNumStart, 0),
				DocumentPosition(lineNumStart, -1))));
		}
	}

	DocumentCaret *pos;
	foreach(pos, m_view->carets())
		paintCaret(paint, pos);
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
	int pressLine = (event->y() + documentOffsetY()) / fontMetrics().height();
	int pressColumn = 0;
	int lineLength;
	QString line;
	
	if(pressLine >= m_view->document().lineCount())
	{
		pressLine = m_view->document().lineCount()-1;
	}
	
	line = m_view->document().text(
		DocumentRange(
			DocumentPosition(pressLine, 0),
			DocumentPosition(pressLine, -1)));
	lineLength = line.length();
	
	// Find the column were at
	int i;
	for(i = 0;i < lineLength;i++)
	{
		pressColumn += fontMetrics().width(line[i]);
		if(event->x() <= pressColumn)
		{
			pressColumn--;
			break;
		}
	}
	pressColumn = i;
	
	// Set keyboard caret
	m_view->keyboardCaret()->setLine(pressLine);
	m_view->keyboardCaret()->setColumn(pressColumn);
	
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

void DocumentViewInternal::setupUi()
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setFocusPolicy(Qt::ClickFocus);
	setCursor(Qt::IBeamCursor);
}

void DocumentViewInternal::setupSignals()
{
	connect(&m_view->document(), SIGNAL(textChanged()),
			this, SLOT(documentTextChanged()));
}

void DocumentViewInternal::paintCaret(QPainter &paint,
	DocumentCaret *pos)
{
	int fontHeight = fontMetrics().height();
	int caretYStart, caretXStart;
	QString prevText;
	QRect bound;
	
	if(pos->isVisible() && hasFocus())
	{
		prevText = m_view->document().text(
			DocumentRange(
				DocumentPosition(pos->line(), 0),
				DocumentPosition(pos->line(), pos->column())));
		caretYStart = (pos->line() * fontHeight) - documentOffsetY();
		caretXStart = fontMetrics().width(prevText);
		
		bound = QRect(caretXStart, caretYStart, 1, fontMetrics().height());
		paint.fillRect(bound, Qt::black);
	}
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
