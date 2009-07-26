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

#ifndef QSOURCEVIEW_DOCUMENT_VIEW_FACTORY_H
#define QSOURCEVIEW_DOCUMENT_VIEW_FACTORY_H

#include <QTextDocument>

namespace QSourceView
{

class Document;
class DocumentView;
class DocumentViewFactoryPrivate;

class DocumentViewFactory
	: public QObject
{
	Q_OBJECT

	public:
		DocumentViewFactory(QObject *parent = 0);
		virtual ~DocumentViewFactory();

		const QList<DocumentView*> views() const;
		virtual DocumentView *createView(Document &document) const;
		
		/**
		 * @brief Inserts view into view list
		 *
		 * Does not manage the inserted view, only appends it to
		 * the views() list.
		 */
		void insertView(DocumentView &view);
	
	private Q_SLOTS:
		void documentViewDestroyed(QObject *obj);
		
	private:
		DocumentViewFactoryPrivate *d;

};

}

#endif