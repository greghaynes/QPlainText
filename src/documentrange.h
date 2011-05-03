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

#ifndef QPLAINTEXT_DOCUMENT_RANGE_H
#define QPLAINTEXT_DOCUMENT_RANGE_H

#include "documentposition.h"

namespace QPlainText
{

class DocumentRange
{

	public:
		DocumentRange();
		DocumentRange(const DocumentPosition &start,
			const DocumentPosition &end);
		
		DocumentPosition &start();
		DocumentPosition &end();
		const DocumentPosition &start() const;
		const DocumentPosition &end() const;
		void setStart(const DocumentPosition &start);
		void setEnd(const DocumentPosition &end);
		bool isEmpty(void) const;
		bool isValid(void) const;
		
	private:
		DocumentPosition m_start;
		DocumentPosition m_end;

};

}

#endif
