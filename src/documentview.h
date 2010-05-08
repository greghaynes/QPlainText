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

#ifndef QPLAINTEXT_DOCUMENT_VIEW_H
#define QPLAINTEXT_DOCUMENT_VIEW_H

#include "caret.h"

#include <QWidget>

namespace QPlainText
{

class Document;
class Caret;
class KeyboardHandler;
class DocumentViewPrivate;

/**
  * @brief QWidget view of a document.
  */
class DocumentView
	: public QWidget
{

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
		virtual ~DocumentView();
		
		/**
		  * @brief Get document represented by this view.
		  */
		Document &document();

		/**
		  * @brief Position of caret for keyboard.
		  */
		virtual Caret &keyboardCaret() = 0;

		/**
		  * 0 creates default handler.
		  */
		void setKeyboardHandler(KeyboardHandler *kdbHandler);

		KeyboardHandler &keyboardHandler();
	
	private:
		DocumentViewPrivate *d;

};

}

#endif
