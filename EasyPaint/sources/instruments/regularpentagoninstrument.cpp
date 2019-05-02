// Mustafa
// regular pentagon
#include "RegularPentagonInstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"

#include <QPen>
#include <QPainter>
#include <QPolygon>
#include <QVector>
#include <QPointF>
#include "../dialogs/textdialog.h"

RegularPentagonInstrument::RegularPentagonInstrument(QObject *parent) :
	AbstractInstrument(parent)
{
}
void RegularPentagonInstrument::mousePressEvent(QMouseEvent * event, ImageArea & imageArea)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
	{
		mStartPoint = mEndPoint = event->pos();
		imageArea.setIsPaint(true);
		mImageCopy = *imageArea.getImage();
		makeUndoCommand(imageArea);
	}
}

void RegularPentagonInstrument::mouseMoveEvent(QMouseEvent * event, ImageArea & imageArea)
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

void RegularPentagonInstrument::mouseReleaseEvent(QMouseEvent * event, ImageArea & imageArea)
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

//: Text Dialog pop up
void RegularPentagonInstrument::textDialog(ImageArea & imageArea)
{
	TextDialog *td = new TextDialog(mText, &imageArea);
	connect(td, SIGNAL(textChanged(ImageArea *, QString)), this, SLOT(updateText(ImageArea *, QString)));
	connect(this, SIGNAL(sendCloseTextDialog()), td, SLOT(accept()));
	connect(td, SIGNAL(canceled(ImageArea *)), this, SLOT(cancel(ImageArea *)));
	td->setAttribute(Qt::WA_DeleteOnClose);
	td->show();
}

//: Paints text on the screen
void RegularPentagonInstrument::paintText(ImageArea & imageArea)
{
	QPainter painter(imageArea.getImage());
	painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor()));
	painter.setFont(DataSingleton::Instance()->getTextFont());
	painter.drawText(QRect(mStartPoint, mEndPoint), mText);
	painter.end();
	imageArea.setEdited(true);
	imageArea.update();
}

//: Paints the pentagon on the screen
void RegularPentagonInstrument::paint(ImageArea & imageArea, bool isSecondaryColor, bool additionalFlag)
{
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
		// get the sizw
		int size = mEndPoint.x() - mStartPoint.x();
		//get half size
		int hSize = (size / 2);
	
		// pentagon have six point
		QPointF point[6];

		//for 1st point is not updated
		point[0].setX(mStartPoint.x());
		point[0].setY(mStartPoint.y());

		//for 2nd point x move from left and y goes down
		//therefore, an angle shape c
		point[1].setX(point[0].x() + size);
		point[1].setY(point[0].y() + size);
		
		//for 3rd point x moves half size to left and the y goes down 
		point[2].setX(point[1].x() - hSize);
		point[2].setY(point[1].y() + size);
		
		//for 4th point x moves left from point 3 and the y doest not move
		point[3].setX(point[2].x() - size);
		point[3].setY(point[2].y());

		// for 5th point x moves half size from point 4 and the y goes up
		point[4].setX(point[3].x() - hSize);
		point[4].setY(point[1].y());

		//for 6th upadted sames point 1
		point[5].setX(point[0].x());
		point[5].setY(point[0].y());

		// create polygon
		QPolygon poly;
		poly << point[0].toPoint();
		poly << point[1].toPoint();
		poly << point[2].toPoint();
		poly << point[3].toPoint();
		poly << point[4].toPoint();
		poly << point[5].toPoint();

		// set point for polygon
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

//get user text after shape drew
void RegularPentagonInstrument::updateText(ImageArea *imageArea, QString textString)
{
	mText = textString;
	imageArea->setImage(mImageCopy);
	paintText(*imageArea);
	paint(*imageArea);
}

//if user cancel the dialog box
void RegularPentagonInstrument::cancel(ImageArea * imageArea)
{
	mText = QString();
}
