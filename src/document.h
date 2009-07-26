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

#ifndef QSOURCEVIEW_DOCUMENT_H
#define QSOURCEVIEW_DOCUMENT_H

#include "documentposition.h"

#include <QObject>

namespace QSourceView
{

class DocumentViewFactory;
class DocumentRange;
class DocumentPrivate;

class Document
	: public QObject
{
	Q_OBJECT

	public:
		Document(QObject *parent = 0);
		virtual ~Document();
		
		DocumentViewFactory &viewFactory();
		void setViewFactory(DocumentViewFactory &factory);
		virtual QString text(unsigned int line) const = 0;
		virtual unsigned int lineCount() const = 0;
		virtual unsigned int lineSize(unsigned int line) const = 0;
		virtual void clear() = 0;
		virtual DocumentPosition end() const = 0;
		/**
		 * @brief Calls onInsertText and emits textInserted signal.
		 */
		void insertText(const DocumentPosition &position,
			const QString &text);
		/**
		 * @brief Calls onRemoveText and emits textRemoved signal.
		 */
		void removeText(const DocumentRange &range);
		virtual void appendText(const QString &text);
	
	Q_SIGNALS:
		void textInserted(const DocumentPosition &pos,
			const QString &text);
		void textRemoved(const DocumentRange &range);
	
	protected:
		virtual void onInsertText(const DocumentPosition &position,
			const QString &text) = 0;
		virtual void onRemoveText(const DocumentRange &range) = 0;
	
	private:
		DocumentPrivate *d;

};

}

#endif