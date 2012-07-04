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

#ifndef QPLAINEXT_SELECTED_RANGE_H
#define QPLAINEXT_SELECTED_RANGE_H

#include "positionalmarker.h"

#include <QObject>

namespace QPlainText
{

class SelectedRange
	: public PositionalMarker
{
	public:
		SelectedRange(QObject *parent = 0);

		SelectedRange(const DocumentPosition &start,
			const DocumentRange &end,
			QObject *parent = 0);

};

}

#endif
