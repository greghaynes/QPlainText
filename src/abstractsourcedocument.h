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

#ifndef QSOURCEVIEW_ABSTRACT_SOURCE_DOCUMENT_H
#define QSOURCEVIEW_ABSTRACT_SOURCE_DOCUMENT_H

#include <QObject>

class QString;

namespace QSourceView
{

class DocumentPosition;
class DocumentRange;
class AbstractSourceDocumentPrivate;

class AbstractSourceDocument
	: public QObject
{
	Q_OBJECT

	public:
		AbstractSourceDocument(QObject *parent = 0);
		AbstractSourceDocument(const QString &text,
			QObject *parent = 0);
		virtual ~AbstractSourceDocument();
	
		QString encoding() const;
		void setEncoding(const QString &encoding);
		
		virtual void insertText(const DocumentPosition &pos,
			const QString &text) = 0;
		virtual void removeText(const DocumentRange &pos) = 0;
	
	Q_SIGNALS:
		void textInserted(const DocumentPosition &position,
			const QString &text);
	
	private:
		AbstractSourceDocumentPrivate *d;
	
}; // AbstractSourceDocument

} // QSourceView

#endif