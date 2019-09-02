/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "tpvsvgview.h"

#include <QFile>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QtSvg/QSvgRenderer>
#include <QPaintEvent>
#include <qmath.h>

namespace gong {
namespace tpv {

SvgView::SvgView( QWidget *parent )
	: QGraphicsView(parent)
	, pSvgItem(0), pRenderer(0)
{
	setScene(new QGraphicsScene(this));
	setTransformationAnchor(AnchorUnderMouse);
//	setDragMode(ScrollHandDrag);
	setViewportUpdateMode(FullViewportUpdate);

	// Prepare background check-board pattern
	QPixmap tilePixmap(64, 64);
	tilePixmap.fill(Qt::white);
	QPainter tilePainter(&tilePixmap);
	QColor color(220, 220, 220);
	tilePainter.fillRect(0, 0, 32, 32, color);
	tilePainter.fillRect(32, 32, 32, 32, color);
	tilePainter.end();

	setBackgroundBrush(tilePixmap);
}

void SvgView::openFile( const char *fname )
{
	if( strlen(fname) == 0 )
		return;

	QGraphicsScene *s = scene();
	s->clear();
	resetTransform();

	pRenderer = new QSvgRenderer( QString(fname) );
	pSvgItem = new TpvSvgItem( 0 );
	pSvgItem->setSharedRenderer( pRenderer );
	pSvgItem->setFlags(QGraphicsItem::ItemClipsToShape);
	pSvgItem->setCacheMode(QGraphicsItem::NoCache);
	pSvgItem->setZValue(0);

	pBackground = new QGraphicsRectItem(pSvgItem->boundingRect());
	pBackground->setBrush(Qt::white);
	pBackground->setPen(Qt::NoPen);
	pBackground->setVisible(true);
	pBackground->setZValue(-1);

	s->addItem(pBackground);
	s->addItem(pSvgItem);
	s->setSceneRect(pSvgItem->boundingRect().adjusted(0, 0, 0, 0));
}

void SvgView::drawBackground(QPainter *p, const QRectF &)
{
	p->save();
	p->resetTransform();
	p->drawTiledPixmap(viewport()->rect(), backgroundBrush().texture());
	p->restore();
}

void SvgView::wheelEvent(QWheelEvent *event)
{
	qreal factor = qPow(1.2, event->delta() / 240.0);
	scale(factor, factor);
	event->accept();
}

} // namespace gong
} // namespace tpv
