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

#include "caret.h"

#include <QTimer>

namespace QPlainText
{

Caret::Caret(QObject *parent)
	: PositionalMarker(parent)
	, DocumentPosition()
	, m_blinkTimer(0)
	, m_blinking(false)
	, m_blink_msecs(400)
{
}

Caret::Caret(int line,
	int column,
	QObject *parent)
	: PositionalMarker(parent)
	, DocumentPosition(line, column)
	, m_blinkTimer(0)
	, m_blinking(false)
	, m_blink_msecs(400)
{
}

bool Caret::isBlinking(void) const
{
	return m_blinking;
}

void Caret::setBlinking(bool val)
{
	if(isBlinking() == val)
		return;

	if(!m_blinkTimer)
	{
		m_blinkTimer = new QTimer(this);
		m_blinkTimer->setSingleShot(false);
		connect(m_blinkTimer, SIGNAL(timeout()),
			this, SLOT(blinkTimeout(void)));
	}

	m_blinking = val;
	if(m_blinking)
		m_blinkTimer->start(m_blink_msecs);
	else
		m_blinkTimer->stop();
}

int Caret::blinkMsecs(void) const
{
	return m_blink_msecs;
}

void Caret::resetBlinking(void)
{
	if(!isBlinking())
		return;

	setBlinking(false);
	setVisible(true);
	setBlinking(true);
}

void Caret::onSetLine(int line)
{
	emit(positionChanged(this));
}

void Caret::onSetColumn(int column)
{
	emit(positionChanged(this));
}

void Caret::blinkTimeout(void)
{
	setVisible(!isVisible());
}

}

#include "caret.moc"
