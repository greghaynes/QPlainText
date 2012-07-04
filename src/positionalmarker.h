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

#ifndef QPLAINTEXT_POSISIONAL_MARKER_H
#define QPLAINTEXT_POSISIONAL_MARKER_H

#include <QObject>

namespace QPlainText
{

class PositionalMarker
	: public QObject
{
	Q_OBJECT

	public:
		PositionalMarker(QObject *parent = 0);

		/**
		  * @brief Is the marker visible.
		  */
		bool isVisible(void) const;

		/**
		  * @brief Make marker invisible.
		  */
		void setVisible(bool val);

	Q_SIGNALS:
		/**
		  * @brief The visibility of the caret has changed.
		  */
		void visibilityChanged(bool visible);

	private:
		bool m_visible;

};

}

#endif

