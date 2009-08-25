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

#include <stdexcept>

class QFont;

namespace QSourceView 
{

class Document;
class Renderer;
class DocumentPosition;
class DocumentController;
class DocumentViewPrivate;

class DocumentView
	: public QWidget
{
	Q_OBJECT

	public:
		DocumentView(Document &document);
		~DocumentView();
		
		Document &document();
		DocumentController &controller();
		void setController(DocumentController *controller);
		const DocumentPosition &caretPosition() const;
		void setCaretPosition(const DocumentPosition &pos)
			throw(std::out_of_range);
	
	public Q_SLOTS:
		void enableHorizontalNumberWidget();
		void enableVerticalNumberWidget();
		void setInternalFont(const QFont &font);
	
	private Q_SLOTS:
		void slotDocumentTextInserted(const DocumentPosition &position,
			const QString &text);
		void slotInternalViewResize(int width, int height);
	
	private:
		unsigned int longestLine();
		void setupUi();
		Renderer &renderer();
		void resizeScrollbar();
	
		DocumentViewPrivate *d;

};

}

#endif
