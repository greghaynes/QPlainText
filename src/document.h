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

#include <stdexcept>

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
		virtual QString text(int line) const = 0;
		virtual int lineCount() const = 0;
		virtual int lineSize(int line) const = 0;
		virtual void clear() = 0;
		virtual DocumentPosition end() const = 0;

		void insertText(const DocumentPosition &position,
			QChar ch)
			throw (std::out_of_range, std::runtime_error);

		/**
		 * @brief Calls onInsertText and emits textInserted signal.
		 *
		 * This method throws std::out_of_range and std::runtime_error.
		 * the out_of_range error is used to specify invalid arguments,
		 * while runtime_error's should be used to signal the end of
		 * the world.
		 */
		void insertText(const DocumentPosition &position,
			const QString &text)
			throw(std::out_of_range, std::runtime_error);

		/**
		 * @brief Calls onRemoveText and emits textRemoved signal.
		 *
		 * See insertText for information on exceptions.
		 */
		void removeText(const DocumentRange &range)
			throw(std::out_of_range, std::runtime_error);
		virtual void appendText(const QString &text);
		bool isValidPosition(const DocumentPosition &pos);
	
	Q_SIGNALS:
		void textInserted(const DocumentPosition &pos,
			QChar ch);
		void textInserted(const DocumentPosition &pos,
			const QString &text);
		void textRemoved(const DocumentRange &range);
		void textChanged();
	
	protected:
		virtual void onInsertText(const DocumentPosition &position,
			QChar ch)
			throw(std::out_of_range, std::runtime_error) = 0;
		virtual void onInsertText(const DocumentPosition &position,
			const QString &text)
			throw(std::out_of_range, std::runtime_error) = 0;
		virtual void onRemoveText(const DocumentRange &range)
			throw(std::out_of_range, std::runtime_error) = 0;
	
	private:
		DocumentPrivate *d;

};

}

#endif
