// mustafa
// irregular pentagon
#include "IrregularPentagonInstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"

#include <QPen>
#include <QPainter>
#include "../dialogs/textdialog.h"

IrregularPentagonInstrument::IrregularPentagonInstrument(QObject *parent) :
	AbstractInstrument(parent)
{
}
void IrregularPentagonInstrument::mousePressEvent(QMouseEvent * event, ImageArea & imageArea)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
	{
		mStartPoint = mEndPoint = event->pos();
		imageArea.setIsPaint(true);
		mImageCopy = *imageArea.getImage();
		makeUndoCommand(imageArea);
	}
}

void IrregularPentagonInstrument::mouseMoveEvent(QMouseEvent * event, ImageArea & imageArea)
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

void IrregularPentagonInstrument::mouseReleaseEvent(QMouseEvent * event, ImageArea & imageArea)
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
void IrregularPentagonInstrument::textDialog(ImageArea & imageArea)
{
	TextDialog *td = new TextDialog(mText, &imageArea);
	connect(td, SIGNAL(textChanged(ImageArea *, QString)), this, SLOT(updateText(ImageArea *, QString)));
	connect(this, SIGNAL(sendCloseTextDialog()), td, SLOT(accept()));
	connect(td, SIGNAL(canceled(ImageArea *)), this, SLOT(cancel(ImageArea *)));
	td->setAttribute(Qt::WA_DeleteOnClose);
	td->show();
}

//: Paints text on the screen
void IrregularPentagonInstrument::paintText(ImageArea & imageArea)
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
void IrregularPentagonInstrument::paint(ImageArea & imageArea, bool isSecondaryColor, bool additionalFlag)
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
		// pentagon have six point
		QPointF point[6];
		// get the size
		int size = mEndPoint.rx() - mStartPoint.rx();
	
		// 1st point from where drawing will start
		point[0].setX(mStartPoint.x());
		point[0].setY(mStartPoint.y());

		// 2nd point x goes right from point 0 and the y goes down
		// therefore, angle shape created
		point[1].setX(mStartPoint.x() + size);
		point[1].setY(mStartPoint.y() + size);

		//3rd point x stay as same point and y goes down straight
		point[2].setX(point[1].x());
		point[2].setY(point[1].y() + size);

		//4t point x moves from point 2 to left and y point to same as x
		// therefore straight line will be created from point 3 to 4
		point[3].setX(point[2].x() - (size+size));
		point[3].setY(point[2].x());

		// 5th point x doesn't updated and y goes up from 3
		// there a straigh vertical line created
		point[4].setX(point[3].x());
		point[4].setY(point[1].y());

		//6th point is same as point 1
		point[5].setX(point[0].x());
		point[5].setY(point[0].y());
	

		// create a QPolygon object
		QPolygon poly;
		poly << point[0].toPoint();
		poly << point[1].toPoint();
		poly << point[2].toPoint();
		poly << point[3].toPoint();
		poly << point[4].toPoint();
		poly << point[5].toPoint();

		// setup the set point
		poly.setPoint(5, point[0].toPoint());

		//draw the polygo
		QPainterPath path;
		path.addPolygon(QPolygon(poly));
		painter.drawPath(path);
	}

	imageArea.setEdited(true);
	
	painter.end();
	
	imageArea.update();
}

// set text for shape by showing textDialoug
void IrregularPentagonInstrument::updateText(ImageArea *imageArea, QString textString)
{
	mText = textString;
	imageArea->setImage(mImageCopy);
	paintText(*imageArea);
	paint(*imageArea);
}

// if user cancel text dialoug
void IrregularPentagonInstrument::cancel(ImageArea * imageArea)
{
	mText = QString();
}
