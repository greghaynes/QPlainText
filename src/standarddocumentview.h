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

#ifndef QPLAINTEXT_STANDARD_DOCUMENT_VIEW_H
#define QPLAINTEXT_STANDARD_DOCUMENT_VIEW_H

#include "documentview.h"

class QFont;

namespace QPlainText
{

class Caret;
class SelectedRange;
class Document;
class Renderer;
class DocumentPosition;
class DocumentRange;
class DocumentController;
class StandardDocumentViewPrivate;

/**
  * @brief QWidget view of a document.
  */
class StandardDocumentView
	: public DocumentView
{
	Q_OBJECT

	public:
		/**
		  * @brief Create a document view.
		  *
		  * This view is parented by the document.
		  */
		StandardDocumentView(Document &document);

		/**
		  * @brief Destroy a document view.
		  */
		~StandardDocumentView();
		
		Caret &keyboardCaret();

		SelectedRange &selectedRange();
		
	Q_SIGNALS:
		/**
		  * @brief Internal font of the view changed
		  */
		void internalFontChanged(StandardDocumentView *view,
			const QFont &font);
	
	public Q_SLOTS:
		/**
		  * @brief Set font for internal text.
		  */
		void setInternalFont(const QFont &font);

		/**
		  * @brief Enable vertical scroll bar.
		  */
		void setScrollBarEnabled(bool enable,
			Qt::Orientation orientation=Qt::Vertical);
	
	private Q_SLOTS:
		void documentTextInserted(const DocumentPosition &position,
			const QString &text);
		void documentTextRemoved(const DocumentRange &range);
		void slotInternalViewResize(int width, int height);
	
	private:
		void setupSignals();
		void setupScrollBars();
		unsigned int longestLine();
		void setupUi();
		void resizeScrollbar();
		void documentChanged();
	
		StandardDocumentViewPrivate *d;

};

}

#endif
