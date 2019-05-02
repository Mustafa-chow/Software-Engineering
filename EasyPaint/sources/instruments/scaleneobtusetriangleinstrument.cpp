#pragma once

/*
Jacob McGivern
In this file the class to create the scalene obtuse triangle is created. The drawing of it is based on the movement
in the left and right direction of the cursor when dragging.
*/

#include "scaleneobtusetriangleinstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"
#include <QPen>
#include <QPainter>
#include "../dialogs/textdialog.h"


ScaleneObtuseTriangleInstrument::ScaleneObtuseTriangleInstrument(QObject *parent) : AbstractInstrument(parent)
{

}

void ScaleneObtuseTriangleInstrument::mousePressEvent(QMouseEvent * event, ImageArea & imageArea)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
	{
		mStartPoint = mEndPoint = event->pos();
		imageArea.setIsPaint(true);
		mImageCopy = *imageArea.getImage();
		makeUndoCommand(imageArea);
	}
}


void ScaleneObtuseTriangleInstrument::mouseMoveEvent(QMouseEvent * event, ImageArea & imageArea)
{
	if (imageArea.isPaint())
	{
		mEndPoint = event->pos();
		imageArea.setImage(mImageCopy);
		if (event->buttons() & Qt::LeftButton)
		{
			paint(imageArea, false);
		}
		else if (event->buttons() & Qt::RightButton)
		{
			paint(imageArea, true);
		}
	}
}

void ScaleneObtuseTriangleInstrument::mouseReleaseEvent(QMouseEvent * event, ImageArea & imageArea)
{
	if (imageArea.isPaint())
	{
		textDialog(imageArea);
		imageArea.setImage(mImageCopy);
		if (event->button() == Qt::LeftButton)
		{
			paint(imageArea, false);
		}
		else if (event->button() == Qt::RightButton)
		{
			paint(imageArea, true);
		}
		imageArea.setIsPaint(false);
	}
}
void ScaleneObtuseTriangleInstrument::paint(ImageArea & imageArea, bool isSecondaryColor, bool additionalFlag)
{
	//set the parameters of current image, color selection, and zoom for painting.
	QPainter painter(imageArea.getImage());
	painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor(),
		DataSingleton::Instance()->getPenSize() * imageArea.getZoomFactor(),
		Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	if (isSecondaryColor)
	{
		painter.setBrush(QBrush(DataSingleton::Instance()->getSecondaryColor()));
	}
	if (mStartPoint != mEndPoint)
	{


		int size = mEndPoint.x() - mStartPoint.x();
		//use size/ 3 to make the non-obtuse angles be different angles (multiply one by 2)
	
		int endSize = mEndPoint.y() - mEndPoint.y();

		// triangle will have 4 points, 3 points, and then 4th points back to first.
		QPointF point[4];

		//first point is original
		point[0].setX(mStartPoint.x());
		point[0].setY(mStartPoint.y());
		//second point
		point[1].setX(point[0].x() + 2 * size);
		point[1].setY(point[0].y() + 2 * size);

		//third point
		point[2].setX(point[1].x() - endSize);
		point[2].setY(point[1].y() +  size);

		//for 4th upadted sames point 1
		point[3].setX(point[0].x());
		point[3].setY(point[0].y());

		//readying each point to be painted.
		QPolygon poly;
		poly << point[0].toPoint();
		poly << point[1].toPoint();
		poly << point[2].toPoint();
		poly << point[3].toPoint();

		poly.setPoint(0, point[0].toPoint());
		// draw the polygon
		QPainterPath path;
		path.addPolygon(QPolygon(poly));
		painter.drawPath(path);
	}
	imageArea.setEdited(true);
	painter.end();
	imageArea.update();
}


//drawing text items below

void ScaleneObtuseTriangleInstrument::textDialog(ImageArea & imageArea)
{
	TextDialog *td = new TextDialog(mText, &imageArea);
	connect(td, SIGNAL(textChanged(ImageArea *, QString)), this, SLOT(updateText(ImageArea *, QString)));
	connect(this, SIGNAL(sendCloseTextDialog()), td, SLOT(accept()));
	connect(td, SIGNAL(canceled(ImageArea *)), this, SLOT(cancel(ImageArea *)));
	td->setAttribute(Qt::WA_DeleteOnClose);
	td->show();
}

void ScaleneObtuseTriangleInstrument::paintText(ImageArea & imageArea)
{
	QPainter painter(imageArea.getImage());
	painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor()));
	painter.setFont(DataSingleton::Instance()->getTextFont());
	painter.drawText(QRect(mStartPoint, mEndPoint), mText);
	painter.end();
	imageArea.setEdited(true);
	imageArea.update();
}

//update text after interface pops up
void ScaleneObtuseTriangleInstrument::updateText(ImageArea *imageArea, QString textString)
{
	mText = textString;
	imageArea->setImage(mImageCopy);
	paintText(*imageArea);
	paint(*imageArea);
}

void ScaleneObtuseTriangleInstrument::cancel(ImageArea * imageArea)
{
	mText = QString(); //emptys string
}



