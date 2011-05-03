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

#include "documentview.h"
#include "keyboardhandler.h"

namespace QPlainText
{

class DocumentViewPrivate
{

	public:
		Document *document;
		KeyboardHandler *kbdHandler;

};

DocumentView::DocumentView(Document &document)
	: QWidget(0)
	, d(new DocumentViewPrivate)
{
	d->document = &document;
	d->kbdHandler = new KeyboardHandler(*this);
}

DocumentView::~DocumentView()
{
	delete d;
}

Document &DocumentView::document()
{
	return *d->document;
}

void DocumentView::setKeyboardHandler(KeyboardHandler *kbdHandler)
{
	if(!kbdHandler)
		kbdHandler = new KeyboardHandler(*this);
	else
		kbdHandler->setParent(this);
	d->kbdHandler = kbdHandler;
}

KeyboardHandler &DocumentView::keyboardHandler()
{
	return *d->kbdHandler;
}

}

