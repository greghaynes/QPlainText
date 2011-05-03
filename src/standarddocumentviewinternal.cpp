/*
 * Copyright (C) 2009-2011 Gregory Haynes <greg@greghaynes.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "standarddocumentviewinternal.h"
#include "standarddocumentview.h"
#include "keyboardhandler.h"
#include "document.h"
#include "documentrange.h"
#include "caret.h"

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


#define CARET_INTERVAL 500

namespace QPlainText
{

StandardDocumentViewInternal::StandardDocumentViewInternal(DocumentView &parentView)
	: QWidget(&parentView)
	, m_view(&parentView)
	, m_startX(0)
	, m_startY(0)
	, m_scrollWheelFactor(1) 
{
	setupUi();
	setupSignals();
}

int StandardDocumentViewInternal::documentOffsetX() const
{
	return m_startX;
}

int StandardDocumentViewInternal::documentOffsetY() const
{
	return m_startY;
}

void StandardDocumentViewInternal::setDocumentOffsetX(int x)
{
	if(x < 0)
		m_startX = 0;
	else
		m_startX = x;
	update();
	emit(documentOffsetXChanged(m_startX));
}

void StandardDocumentViewInternal::setDocumentOffsetY(int y)
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

void StandardDocumentViewInternal::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	paintLines(painter, *event);
	paintCaret(painter);
}

void StandardDocumentViewInternal::resizeEvent(QResizeEvent *event)
{
	emit(sizeChanged(event->size().width(), event->size().height()));
}

Document &StandardDocumentViewInternal::document()
{
	return m_view->document();
}

void StandardDocumentViewInternal::keyPressEvent(QKeyEvent *event)
{
	m_view->keyboardHandler().keyPressEvent(event);
	m_view->keyboardCaret().resetBlinking();
	update();
}

void StandardDocumentViewInternal::keyReleaseEvent(QKeyEvent *event)
{
	m_view->keyboardHandler().keyReleaseEvent(event);
}

void StandardDocumentViewInternal::mousePressEvent(QMouseEvent *event)
{
	int lineNum, selectedCaretColumn;
	int lineHeight = fontMetrics().height();
	Caret *caret;
	
	if(!event)
		return;

	// Determine selected line
	lineNum = (documentOffsetY() + event->y()) / lineHeight;
	if(lineNum >= (document().lineCount() - 1))
		lineNum = document().lineCount() - 1;
	if(lineNum < 0)
		lineNum = 0;

	qDebug() << "selected line " << lineNum;
	
	/* Determine selected caret column
	   Caret columns occour inbetween characters, starting at 0 infront of the
	   first charater on the line, and ending at (numchars + 1) after all the
	   characters on the line. */
	QString line = document().text(DocumentRange(
		DocumentPosition(lineNum, 0),
		DocumentPosition(lineNum, -1)));
	int lineWidth = 0;
	int lineCharNum = 0;
	while(true)
	{
		if(lineCharNum >= line.size())
			break;
		int charWidth = fontMetrics().width(line[lineCharNum]);
		if((lineWidth + (charWidth / 2)) > event->x())
			break;
		lineWidth += charWidth;
		++lineCharNum;
	}

	caret = &m_view->keyboardCaret();
	caret->setLine(lineNum);
	caret->setColumn(lineCharNum);
	update();
}

void StandardDocumentViewInternal::wheelEvent(QWheelEvent *event)
{
	event->ignore();
	if(event->orientation() == Qt::Vertical)
	{
		setDocumentOffsetY(documentOffsetY() - (event->delta() / this->m_scrollWheelFactor));
	}
}

void StandardDocumentViewInternal::setupUi()
{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setFocusPolicy(Qt::ClickFocus);
	setCursor(Qt::IBeamCursor);

	m_view->keyboardCaret().setBlinking(true);
}

void StandardDocumentViewInternal::setupSignals()
{
	connect(&m_view->document(), SIGNAL(textChanged()),
			this, SLOT(documentTextChanged()));
	connect(&m_view->keyboardCaret(), SIGNAL(visibilityChanged(Caret*, bool)),
		this, SLOT(caretVisibilityChanged(Caret*, bool)));
}

void StandardDocumentViewInternal::paintLines(QPainter &paint,
	QPaintEvent &event)
{
	int lineHeight = fontMetrics().height();
	int curOffset = lineHeight - (documentOffsetY() % (lineHeight));
	int startLine = documentOffsetY() / lineHeight;
	int endLine = ((documentOffsetY() + height()) / lineHeight) + 1;
	int curLine, curLineLen, charNdx;
	Document *doc = &m_view->document();
	QString line;

	if(doc->lineCount() < endLine)
		endLine = doc->lineCount();

	paint.fillRect(0, 0, width(), height(), Qt::white);

	for(curLine = startLine;curLine < endLine;++curLine,curOffset+=lineHeight)
	{
		curLineLen = 0;
		charNdx = 0;
		line = doc->text(DocumentRange(
			DocumentPosition(curLine, 0), DocumentPosition(curLine, -1)));
		while(charNdx < line.size() && curLineLen < width())
		{
			charNdx++;
			curLineLen += fontMetrics().width(line[charNdx]);
		}
		paint.drawText(0, curOffset, line.left(charNdx));
	}
}

void StandardDocumentViewInternal::paintCaret(QPainter &paint)
{
	int fontHeight = fontMetrics().height();
	int caretYStart, caretXStart;
	QString prevText;
	QRect bound;
	Caret *pos = &m_view->keyboardCaret();
	
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
		//qDebug() << "Painting caret";
	}
}

void StandardDocumentViewInternal::documentTextChanged()
{
	update();
}

void StandardDocumentViewInternal::caretVisibilityChanged(Caret *self,
	bool visible)
{
	update();
}

int StandardDocumentViewInternal::lineAt(int x) const
{
	return x / fontMetrics().height();
}

}

#include "standarddocumentviewinternal.moc"
