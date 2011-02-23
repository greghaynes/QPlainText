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

#ifndef QPLAINTEXT_DOCUMENT_CARET_H
#define QPLAINTEXT_DOCUMENT_CARET_H

#include "documentposition.h"

#include <QObject>

class QTimer;

namespace QPlainText
{

/**
  * @brief Represents a caret in a document view.
  */
class Caret
	: public QObject
	, public DocumentPosition
{
	Q_OBJECT

	public:
		/**
		  * @brief Create a Caret.
		  */
		Caret(QObject *parent = 0);
		
		/**
		  * @brief Create a Caret.
		  */
		Caret(int line,
			int column,
			QObject *parent);
		
		/**
		  * @brief Is the caret visible.
		  */
		bool isVisible() const;
		
		/**
		  * @brief Make caret invisible.
		  */
		void setVisible(bool val);

		/**
		  * @brief Is the caret currently blinking.
		  */
		bool isBlinking(void) const;

		/**
		  * @brief Turn blinking on/off.
		  */
		void setBlinking(bool val);

		/**
		  * @brief Miliseconds for complete blink interval.
		  */
		int blinkMsecs(void) const;

	Q_SIGNALS:
		/**
		  * @brief The position of the caret has changed.
		  */
		void positionChanged(Caret *self);
		
		/**
		  * @brief The visibility of the caret has changed.
		  */
		void visibilityChanged(Caret *self,
			bool visible);
	
	protected:
		void onSetLine(int line);
		void onSetColumn(int line);

	private Q_SLOTS:
		void blinkTimeout(void);
	
	private:
		QTimer *m_blinkTimer;
		bool m_visible;
		bool m_blinking;
		int m_blink_msecs;

};

}

#endif
