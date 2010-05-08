/*
 *   Copyright (C) 2009-2010 Gregory Haynes <greg@greghaynes.net>
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
	/*
	QRect rect = event->rect();
	
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
	*/
}

void StandardDocumentViewInternal::resizeEvent(QResizeEvent *event)
{
	emit(sizeChanged(event->size().width(), event->size().height()));
}

void StandardDocumentViewInternal::keyPressEvent(QKeyEvent *event)
{
	m_view->keyboardHandler().keyPressEvent(event);
}

void StandardDocumentViewInternal::keyReleaseEvent(QKeyEvent *event)
{
	m_view->keyboardHandler().keyReleaseEvent(event);
}

void StandardDocumentViewInternal::mousePressEvent(QMouseEvent *event)
{
	/*
	int pressLine = (event->y() + documentOffsetY()) / fontMetrics().height();
	int pressColumn = 0;
	int lineLength;
	int letterWidth;
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
		letterWidth = fontMetrics().width(line[i]);
		if(event->x() <= (pressColumn + letterWidth / 2))
		{
			pressColumn--;
			break;
		}
		else
			pressColumn += letterWidth;
	}
	pressColumn = i;
	
	// Set keyboard caret
	m_view->keyboardCaret()->setLine(pressLine);
	m_view->keyboardCaret()->setColumn(pressColumn);
	
	update();
	*/
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
}

void StandardDocumentViewInternal::setupSignals()
{
	connect(&m_view->document(), SIGNAL(textChanged()),
			this, SLOT(documentTextChanged()));
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

void StandardDocumentViewInternal::paintCaret(QPainter &paint,
	Caret *pos)
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

void StandardDocumentViewInternal::documentTextChanged()
{
	update();
}

int StandardDocumentViewInternal::lineAt(int x) const
{
	return x / fontMetrics().height();
}

}

#include "standarddocumentviewinternal.moc"