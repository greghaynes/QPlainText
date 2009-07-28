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

namespace QSourceView 
{

class Document;
class Renderer;
class DocumentPosition;
class DocumentViewPrivate;

class DocumentView
	: public QWidget
{
	Q_OBJECT

	public:
		DocumentView(Document &document);
		~DocumentView();
		
		Document &document();
	
	public Q_SLOTS:
		void enableHorizontalNumberWidget();
		void enableVerticalNumberWidget();
	
	private Q_SLOTS:
		void slotDocumentTextInserted(const DocumentPosition &position,
			const QString &text);
	
	private:
		unsigned int longestLine();
		void setupUi();
		Renderer &renderer();
	
		DocumentViewPrivate *d;

};

}

#endif