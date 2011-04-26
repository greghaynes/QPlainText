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

#ifndef QPLAINTEXT_STANDARD_DOCUMENT_H
#define QPLAINTEXT_STANDARD_DOCUMENT_H

#include "document.h"

class QString;

namespace QPlainText
{

class DocumentRange;
class StandardDocumentPrivate;

class StandardDocument
	: public Document
{

	public:
		StandardDocument(QObject *parent = 0);
		~StandardDocument();
		
		QString text(const DocumentRange &range) const;
		int lineCount() const;
		int lineLength(int line) const; 
		void clear();
		DocumentPosition end() const;
		bool insert(const DocumentPosition &position,
			const QString &text);
		bool remove(const DocumentRange &range);
	
	private:
		QString line(int line) const;
		bool tryInsert(const DocumentPosition &position,
			const QString &text);
		bool tryRemove(const DocumentRange &range);
		void insertSingleLine(const DocumentPosition &position,
			const QString &text);
		bool isNewline(QChar ch) const;

		/* These check the validity of a position for specific
		   operations */
		bool isRetrievableRange(const DocumentRange &range) const;
		bool isInsertablePosition(const DocumentPosition &pos) const;
		bool isRemovableRange(const DocumentRange &range) const;

		/* These expand -1's to their actual value
		   do not use them for error checking */
		void autoexpandRange(DocumentRange &range) const;
		void autoexpandPosition(DocumentPosition &pos) const;
		int lineAutoexpandVal(void) const;
		int columnAutoexpandVal(int line) const;

		StandardDocumentPrivate *d;

};

}

#endif
