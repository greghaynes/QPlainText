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

#include "caret.h"

#include <QTimer>

namespace QPlainText
{

Caret::Caret(QObject *parent)
	: QObject(parent)
	, DocumentPosition()
	, m_blinkTimer(0)
	, m_visible(true)
	, m_blinking(false)
	, m_blink_msecs(400)
{
}

Caret::Caret(int line,
	int column,
	QObject *parent)
	: QObject(parent)
	, DocumentPosition(line, column)
	, m_blinkTimer(0)
	, m_visible(true)
	, m_blinking(false)
	, m_blink_msecs(400)
{
}

bool Caret::isVisible() const
{
	return m_visible;
}

void Caret::setVisible(bool value)
{
	if(m_visible != value)
	{
		//emit(visibilityChanged(this, value));
		m_visible = value;
	}
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
	m_visible = !m_visible;
	emit(visibilityChanged(this, m_visible));
}

}

#include "caret.moc"
