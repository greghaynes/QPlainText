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

#ifndef QSOUCEVIEW_DOCUMENT_VIEW_INTERNAL_H
#define QSOUCEVIEW_DOCUMENT_VIEW_INTERNAL_H

#include <QWidget>

class QPaintEvent;
class QMouseEvent;
class QPainter;
class QResizeEvent;

namespace QSourceView
{

class DocumentView;
class TextCursor;
class Renderer;

class DocumentViewInternal
	: public QWidget
{
	Q_OBJECT

	public:
		DocumentViewInternal(DocumentView &parentView,
			Renderer &renderer);
		
		int startY() const;
		int startX() const;
		int endY() const;

	Q_SIGNALS:
		void sizeChanged(int width, int height);
	
	public Q_SLOTS:
		void setStartY(int x);
		void setStartX(int x);
		
	protected:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent *event);
	
	private Q_SLOTS:
		void toggleCaretVisibility();
	
	private:
		void paintCaret(QPainter &paint);
		Renderer &renderer() { return *m_renderer; }
		unsigned int lineAt(unsigned int x) const;
	
		// This is a hidden class so private members are ok
		DocumentView *m_view;
		Renderer *m_renderer;
		int m_startX;
		int m_startY;
		TextCursor *m_caret;

};

}

#endif
