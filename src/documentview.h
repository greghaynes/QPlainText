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

#ifndef QSOURCEVIEW_SOURCE_DOCUMENT_VIEW_H
#define QSOURCEVIEW_SOURCE_DOCUMENT_VIEW_H

#include <QWidget>

class QFont;

namespace QSourceEdit
{

class Document;
class Renderer;
class DocumentPosition;
class DocumentController;
class DocumentViewPrivate;

/**
  * @brief QWidget view of a document.
  */
class DocumentView
	: public QWidget
{
	Q_OBJECT

	public:
		/**
		  * @brief Create a document view.
		  *
		  * This view is parented by the document.
		  */
		DocumentView(Document &document);

		/**
		  * @brief Destroy a document view.
		  */
		~DocumentView();
		
		/**
		  * @brief Get document represented by this view.
		  */
		Document &document();

		/**
		  * @brief Controller for this view.
		  */
		DocumentController &controller();

		/**
		  * @brief Set controller for this view.
		  */
		void setController(DocumentController *controller);

		/**
		  * @brief Current caret position.
		  */
		const DocumentPosition &caretPosition() const;

		/**
		  * @brief Set the caret position.
		  */
		bool setCaretPosition(const DocumentPosition &pos);
	
	public Q_SLOTS:
		/**
		  * @brief Set font for internal text.
		  */
		void setInternalFont(const QFont &font);

		/**
		  * @brief Enable vertical scroll bar.
		  */
		void enableVerticalScrollBar(bool enable);
	
	private Q_SLOTS:
		void documentTextInserted(const DocumentPosition &position,
			const QString &text);
		void slotInternalViewResize(int width, int height);
	
	private:
		void setupSignals();
		void setupScrollBars();
		unsigned int longestLine();
		void setupUi();
		void resizeScrollbar();
	
		DocumentViewPrivate *d;

};

}

#endif
