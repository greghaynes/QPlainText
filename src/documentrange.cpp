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

#include "documentrange.h"

namespace QPlainText
{

DocumentRange::DocumentRange()
{
}

DocumentRange::DocumentRange(const DocumentPosition &start,
	const DocumentPosition &end)
	: m_start(start)
	, m_end(end)
{
}

DocumentPosition &DocumentRange::start()
{
	return m_start;
}

DocumentPosition &DocumentRange::end()
{
	return m_end;
}

const DocumentPosition &DocumentRange::start() const
{
	return m_start;
}

const DocumentPosition &DocumentRange::end() const
{
	return m_end;
}

void DocumentRange::setStart(const DocumentPosition &start)
{
	m_start = start;
	onSetStart();
}

void DocumentRange::setEnd(const DocumentPosition &end)
{
	m_end = end;
	onSetEnd();
}

bool DocumentRange::isEmpty(void) const
{
	return m_start >= m_end;
}

bool DocumentRange::isValid(void) const
{
	return !isEmpty() || (m_start == m_end);
}

}
