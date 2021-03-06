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

#ifndef QPLAINTEXT_DOCUMENT_VIEW_INTERNAL_H
#define QPLAINTEXT_DOCUMENT_VIEW_INTERNAL_H

#include <QWidget>

class QPaintEvent;
class QKeyEvent;
class QMouseEvent;
class QPainter;
class QResizeEvent;

namespace QPlainText
{

class Document;
class DocumentView;
class TextCursor;
class DocumentPosition;
class Caret;

/**
  * @brief Internal text widget of document view.
  */
class StandardDocumentViewInternal
	: public QWidget
{
	Q_OBJECT

	public:
		/**
		  * @brief Create internal view.
		  */
		StandardDocumentViewInternal(DocumentView &parentView);
		
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

		Document &document();
	
	private Q_SLOTS:
		void documentTextChanged();
		void caretVisibilityChanged(bool visible);
	
	private:
		void setupUi();
		void setupSignals();
		void paintLines(QPainter &paint, QPaintEvent &event);
		void paintCaret(QPainter &paint);
		int lineAt(int x) const;
	
		// This is a hidden class so private members are ok
		DocumentView *m_view;
		int m_startX;
		int m_startY;
		int m_scrollWheelFactor;

};

}

#endif
