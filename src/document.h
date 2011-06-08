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

#ifndef QPLAINTEXT_DOCUMENT_H
#define QPLAINTEXT_DOCUMENT_H

#include "documentposition.h"

#include <QObject>

namespace QPlainText
{

class DocumentViewFactory;
class DocumentRange;
class DocumentPrivate;

/**
  * @brief Abstract document interface.
  */
class Document
	: public QObject
{
	Q_OBJECT

	public:
		/**
		  * @brief Create a new Document.
		  */
		Document(QObject *parent = 0);

		/**
		  * @brief Destroy a document.
		  */
		virtual ~Document();
		
		/**
		  * @brief Obtain text from the document.
		  * @return QString instance containing text, or NULL on error
		  *
		  * Creates a new QString instance contaning text or NULL.
		  * You should delete the returned instance when it is no
		  * longer needed.
		  */
		virtual QString text(const DocumentRange &range) const = 0;

		/**
		  * @brief Number of lines in document.
		  */
		virtual int lineCount() const = 0;

		/**
		  * @brief Number of characters in specified line.
		  */
		virtual int lineLength(int line) const = 0;

		/**
		  * @brief Clear all text from document.
		  */
		virtual void clear() = 0;

		/**
		  * @brief Position pointed to last character in document.
		  */
		virtual DocumentPosition end() const = 0;

		/**
		 * @brief Calls onInsertText and emits textInserted signal.
		 */
		virtual bool insert(const DocumentPosition &position,
			const QString &text) = 0;
		
		/**
		 * @brief Append text to end of document.
		 *
		 * Does not insert a newline before appending text.
		 */
		bool append(const QString &text);

		/**
		 * @brief Calls onRemoveText and emits textRemoved signal.
		 */
		virtual bool remove(const DocumentRange &range) = 0;

	Q_SIGNALS:
		/**
		  * @brief Text has been inserted into document.
		  *
		  * Text insertion begins at character pointed to by pos.
		  */
		bool textInserted(const DocumentPosition &pos,
			const QString &text);

		/**
		  * @brief Text has been removed from document.
		  *
		  * Text removal was betwen characters pointed to by range,
		  * including endpoints.
		  */
		bool textRemoved(const DocumentRange &range);

		void textChanged(void);
};

}

#endif
