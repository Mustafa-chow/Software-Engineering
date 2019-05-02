#include "convexhexagoninstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"

#include <QPen>
#include <QPainter>
#include "../dialogs/textdialog.h"
#include "../instruments/textinstrument.h"


ConvexHexagonInstrument::ConvexHexagonInstrument(QObject *parent) :
	AbstractInstrument(parent)
{
}
void ConvexHexagonInstrument::mousePressEvent(QMouseEvent * event, ImageArea & imageArea)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
	{
		mStartPoint = mEndPoint = event->pos();
		imageArea.setIsPaint(true);
		mImageCopy = *imageArea.getImage();
		makeUndoCommand(imageArea);
	}
}

void ConvexHexagonInstrument::mouseMoveEvent(QMouseEvent * event, ImageArea & imageArea)
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

void ConvexHexagonInstrument::mouseReleaseEvent(QMouseEvent * event, ImageArea & imageArea)
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

void ConvexHexagonInstrument::textDialog(ImageArea &imageArea)
{
	TextDialog *td = new TextDialog(mText, &imageArea);
	connect(td, SIGNAL(textChanged(ImageArea *, QString)), this, SLOT(updateText(ImageArea *, QString)));
	connect(this, SIGNAL(sendCloseTextDialog()), td, SLOT(accept()));
	connect(td, SIGNAL(canceled(ImageArea *)), this, SLOT(cancel(ImageArea *)));
	td->setAttribute(Qt::WA_DeleteOnClose);
	td->show();
}

void ConvexHexagonInstrument::paintText(ImageArea &imageArea)
{
	QPainter painter(imageArea.getImage());
	painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor()));
	painter.setFont(DataSingleton::Instance()->getTextFont());
	painter.drawText(QRect(mStartPoint, mEndPoint), mText);
	painter.end();
	imageArea.setEdited(true);
	imageArea.update();
}

void ConvexHexagonInstrument::paint(ImageArea & imageArea, bool isSecondaryColor, bool additionalFlag)
{
	mText = "";
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
		QPointF point[7];
		int size = mEndPoint.rx() - mStartPoint.rx();

		// 1st point starting
		point[0].setX(mStartPoint.x());
		point[0].setY(mStartPoint.y());

		point[1].setX(mStartPoint.x()-(size/5));
		point[1].setY(mStartPoint.y()+ (size/15));

		// 3nd point
		point[2].setX(point[1].x()-(size/5));
		point[2].setY(point[1].y()-(size/15));


		point[3].setX(point[2].x());
		point[3].setY(point[2].y()-(size/15));

		//5
		point[4].setX(point[3].x()+ (size/5));
		point[4].setY(point[3].y() - (size/15));

		//6
		point[5].setX(point[4].x()+(size/5));
		point[5].setY(point[4].y()+ (size/25));

		//7th point is same as point 1
		point[6].setX(point[0].x());
		point[6].setY(point[0].y());


		// create a QPolygon object
		QPolygon poly;
		poly << point[0].toPoint();
		poly << point[1].toPoint();
		poly << point[2].toPoint();
		poly << point[3].toPoint();
		poly << point[4].toPoint();
		poly << point[5].toPoint();
		poly << point[6].toPoint();

		poly.setPoint(6, point[0].toPoint());

		//draw 
		QPainterPath path;
		path.addPolygon(QPolygon(poly));
		painter.drawPath(path);
	}
	imageArea.setEdited(true);
	painter.end();
	imageArea.update();

}


void ConvexHexagonInstrument::updateText(ImageArea *imageArea, QString textString)
{
	mText = textString;
	imageArea->setImage(mImageCopy);
	paintText(*imageArea);
	paint(*imageArea);
}


void ConvexHexagonInstrument::cancel(ImageArea *imageArea)
{
	mText = QString();
}
