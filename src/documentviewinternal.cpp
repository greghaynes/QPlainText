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
#include <QFontMetrics>

namespace QSourceView
{

DocumentViewInternal::DocumentViewInternal(DocumentView &parentView,
	Renderer &renderer)
	: QWidget(&parentView)
	, m_view(&parentView)
	, m_renderer(&renderer)
	, m_startX(0)
{
	setAttribute(Qt::WA_OpaquePaintEvent);
}

void DocumentViewInternal::paintEvent(QPaintEvent *event)
{
	QRect rect = event->rect();
	QPainter paint(this);
	
	int xStart = startX() + rect.x();
	int xEnd = xStart + rect.width();
	unsigned int fontHeight = fontMetrics().height();
	int lineCount = rect.height() / fontHeight;
	int lineStart = rect.y() / fontHeight;
	int lineEnd = lineStart + lineCount;
	
	QRect bound;
	QString text;
	// Top excess
	bound = QRect(0, rect.y(), rect.width(), fontHeight - rect.y());
	text =  m_view->document().text(lineStart);
	paint.fillRect(bound, Qt::white);
	paint.drawText(bound, text);
	
	int line;
	for(line = lineStart; line < lineEnd; line++)
	{
		bound = QRect(0, (line+1)*fontHeight, rect.width(), fontHeight);
		text = m_view->document().text(line+1);
		paint.fillRect(bound, Qt::white);
		paint.drawText(bound, text);
	}

}

}
