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

#ifndef QSOURCEVIEW_STANDARD_DOCUMENT_H
#define QSOURCEVIEW_STANDARD_DOCUMENT_H

#include "document.h"

class QString;

namespace QSourceView
{

class DocumentRange;
class StandardDocumentPrivate;

class StandardDocument
	: public Document
{

	public:
		StandardDocument(QObject *parent = 0);
		~StandardDocument();
		
		QString text(int line) const;
		int lineCount() const;
		int lineSize(int line) const; 
		void clear();
		DocumentPosition end() const;
	
	protected:
		void onInsertText(const DocumentPosition &position,
			QChar ch)
			throw(std::out_of_range, std::runtime_error);
		void onInsertText(const DocumentPosition &position,
			const QString &text)
			throw(std::out_of_range, std::runtime_error);
		void onRemoveText(const DocumentRange &range)
			throw(std::out_of_range, std::runtime_error);
	
	private:
		bool isNewline(QChar ch) const;

		StandardDocumentPrivate *d;

};

}

#endif
