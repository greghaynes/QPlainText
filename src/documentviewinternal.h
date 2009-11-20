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
class QKeyEvent;
class QMouseEvent;
class QPainter;
class QResizeEvent;

namespace QSourceEdit
{

class DocumentView;
class TextCursor;
class DocumentPosition;
class DocumentCaret;

/**
  * @brief Internal text widget of document view.
  */
class DocumentViewInternal
	: public QWidget
{
	Q_OBJECT

	public:
		/**
		  * @brief Create internal view.
		  */
		DocumentViewInternal(DocumentView &parentView);
		
		/**
		  * @brief Offset in document in Y direction.
		  *
		  * Offset in terms of document's pixels to top of view.
		  */ 
		int documentOffsetY() const;

		/**
		  * @brief Offset in document in X direction.
		  *
		  * Offset in terms of document's pixels to leftmost pont of view.
		  */ 
		int documentOffsetX() const;

	Q_SIGNALS:
		/**
		  * @brief The size of the view has changed.
		  */
		void sizeChanged(int width = 0, int height = 0);

		/**
		  * @brief The Y offset of the view has changed.
		  */
		void documentOffsetYChanged(int y = 0);

		/**
		  * @brief The X offset of the view has changed.
		  */
		void documentOffsetXChanged(int x = 0);
	
	public Q_SLOTS:
		/**
		  * @brief Set offset from the document's start.
		  */
		void setDocumentOffsetY(int);

		/**
		  * @brief Set offset from the document's line's start.
		  */
		void setDocumentOffsetX(int);
		
	protected:
		/**
		  * @brief Paint event occoured.
		  */
		void paintEvent(QPaintEvent *event);

		/**
		  * @brief A key has been pressed.
		  */
		void keyPressEvent(QKeyEvent *event);
		
		/**
		  * @brief A key has been released.
		  */
		void keyReleaseEvent(QKeyEvent *event);

		/**
		  * @brief A mouse button has been pressed.
		  */
		void mousePressEvent(QMouseEvent *event);

		/**
		  * @brief The mouse wheel has been moved.
		  */
		void wheelEvent(QWheelEvent *event);

		/**
		  * @brief The widget has been resized.
		  */
		void resizeEvent(QResizeEvent *event);
	
	private Q_SLOTS:
		void documentTextChanged();
	
	private:
		void setupUi();
		void setupSignals();
		void paintCaret(QPainter &paint, DocumentCaret *pos);
		int lineAt(int x) const;
	
		// This is a hidden class so private members are ok
		DocumentView *m_view;
		int m_startX;
		int m_startY;
		int m_scrollWheelFactor;

};

}

#endif
