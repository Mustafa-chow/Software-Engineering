//JUSTIN: Concave pentagon implementation
#include "concavepentagoninstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"

#include <QPen>
#include <QPainter>
#include "../dialogs/textdialog.h"

ConcavePentagonInstrument::ConcavePentagonInstrument(QObject *parent) :
	AbstractInstrument(parent)
{
}
void ConcavePentagonInstrument::mousePressEvent(QMouseEvent * event, ImageArea & imageArea)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
	{
		mStartPoint = mEndPoint = event->pos();
		imageArea.setIsPaint(true);
		mImageCopy = *imageArea.getImage();
		makeUndoCommand(imageArea);
	}
}

void ConcavePentagonInstrument::mouseMoveEvent(QMouseEvent * event, ImageArea & imageArea)
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

void ConcavePentagonInstrument::mouseReleaseEvent(QMouseEvent * event, ImageArea & imageArea)
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

void ConcavePentagonInstrument::textDialog(ImageArea & imageArea)
{
	TextDialog *td = new TextDialog(mText, &imageArea);
	connect(td, SIGNAL(textChanged(ImageArea *, QString)), this, SLOT(updateText(ImageArea *, QString)));
	connect(this, SIGNAL(sendCloseTextDialog()), td, SLOT(accept()));
	connect(td, SIGNAL(canceled(ImageArea *)), this, SLOT(cancel(ImageArea *)));
	td->setAttribute(Qt::WA_DeleteOnClose);
	td->show();
}


void ConcavePentagonInstrument::paintText(ImageArea & imageArea)
{
	
	QPainter painter(imageArea.getImage());
	painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor()));
	painter.setFont(DataSingleton::Instance()->getTextFont());
	painter.drawText(QRect(mStartPoint/2, mEndPoint/2), mText);
	painter.end();
	imageArea.setEdited(true);
	imageArea.update();
}

//JUSTIN: Paints the pentagon on the screen
void ConcavePentagonInstrument::paint(ImageArea & imageArea, bool isSecondaryColor, bool additionalFlag)
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
		//JUSTIN: Creates a poly based on the rectangle given by mouse movement
		//with an added fifth point
		QPolygon *poly = new QPolygon(QRect(mStartPoint, mEndPoint), true);
		//JUSTIN: creates a fifth point and directs it to make a pentagon shape
		QPoint *fifthPoint = new QPoint(((int)mStartPoint.rx() + (int)mEndPoint.rx()) / 2,
			((int)mStartPoint.ry() * 1.75));
		//JUSTIN: sets the fifth point
		poly->setPoint(0, *fifthPoint);
		//JUSTIN: draws the pentagon on screen
		painter.drawConvexPolygon(*poly);
	}
	//JUSTIN: set image area as edited
	imageArea.setEdited(true);
	//JUSTIN: end painter
	painter.end();
	//JUSTIN: update the image area
	imageArea.update();
}

//JUSTIN: update text given by user in Text Dialog box and paints it to the screen
void ConcavePentagonInstrument::updateText(ImageArea *imageArea, QString textString)
{
	mText = textString;
	imageArea->setImage(mImageCopy);
	paintText(*imageArea);
	paint(*imageArea);
}

//JUSTIN: used in the even user cancels the text dialog
void ConcavePentagonInstrument::cancel(ImageArea * imageArea)
{
	mText = QString();
}
