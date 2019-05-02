// MUSTAFA

#include "ConvexPentagonSelectionInstrument.h"
#include "../imagearea.h"
#include "../undocommand.h"
#include "math.h"

#include <QPainter>
#include <QApplication>
#include <QClipboard>

ConvexPentagonSelectionInstrument::ConvexPentagonSelectionInstrument(QObject *parent) :
	AbstractSelection(parent)
{
}

void ConvexPentagonSelectionInstrument::copyImage(ImageArea &imageArea)
{
	if (mIsSelectionExists)
	{
		imageArea.setImage(mImageCopy);
		QClipboard *globalClipboard = QApplication::clipboard();
		QImage copyImage;
		if (mIsImageSelected)
		{
			copyImage = mSelectedImage;
		}
		else
		{
			copyImage = imageArea.getImage()->copy(mTopLeftPoint.x(), mTopLeftPoint.y(), mWidth, mHeight);
		}
		globalClipboard->setImage(copyImage, QClipboard::Clipboard);
	}
}

void ConvexPentagonSelectionInstrument::cutImage(ImageArea &imageArea)
{
	if (mIsSelectionExists)
	{
		copyImage(imageArea);
		if (mIsSelectionExists)
		{
			imageArea.setImage(mImageCopy);
			paint(imageArea);
		}
		makeUndoCommand(imageArea);
		if (/*mSelectedImage != mPasteImage || !*/mIsImageSelected)
		{
			imageArea.setImage(mImageCopy);
		}
		else
		{
			clearSelectionBackground(imageArea);
		}
		mTopLeftPoint = QPoint(0, 0);
		mBottomRightPoint = QPoint(0, 0);
		mImageCopy = *imageArea.getImage();
		imageArea.update();
		mIsSelectionExists = false;
		imageArea.restoreCursor();
		emit sendEnableCopyCutActions(false);
	}
}

void ConvexPentagonSelectionInstrument::pasteImage(ImageArea &imageArea)
{
	QClipboard *globalClipboard = QApplication::clipboard();
	if (mIsSelectionExists)
	{
		imageArea.setImage(mImageCopy);
		paint(imageArea);
		mImageCopy = *imageArea.getImage();
	}
	makeUndoCommand(imageArea);
	mPasteImage = globalClipboard->image();
	if (!mPasteImage.isNull())
	{
		mSelectedImage = mPasteImage;
		mImageCopy = *imageArea.getImage();
		mTopLeftPoint = QPoint(0, 0);
		mBottomRightPoint = QPoint(mPasteImage.width(), mPasteImage.height()) - QPoint(1, 1);
		mHeight = mPasteImage.height();
		mWidth = mPasteImage.width();
		mIsImageSelected = mIsSelectionExists = true;
		paint(imageArea);
		drawBorder(imageArea);
		imageArea.restoreCursor();
		emit sendEnableCopyCutActions(true);
	}
}


void ConvexPentagonSelectionInstrument::startAdjusting(ImageArea & imageArea)
{
	mImageCopy = *imageArea.getImage();
	mIsImageSelected = false;
}

void ConvexPentagonSelectionInstrument::startSelection(ImageArea &)
{
}

void ConvexPentagonSelectionInstrument::startResizing(ImageArea &imageArea)
{
	if (!mIsImageSelected)
	{
		clearSelectionBackground(imageArea);
	}
	
}

void ConvexPentagonSelectionInstrument::startMoving(ImageArea &imageArea)
{
	clearSelectionBackground(imageArea);
}

void ConvexPentagonSelectionInstrument::select(ImageArea &)
{
}

void ConvexPentagonSelectionInstrument::resize(ImageArea &)
{
}

void ConvexPentagonSelectionInstrument::move(ImageArea &)
{
}

void ConvexPentagonSelectionInstrument::completeSelection(ImageArea &imageArea)
{
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
		mTopLeftPoint.y(),
		mWidth, mHeight);
	emit sendEnableCopyCutActions(true);
}

void ConvexPentagonSelectionInstrument::completeResizing(ImageArea &imageArea)
{
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
		mTopLeftPoint.y(),
		mWidth, mHeight);
}

void ConvexPentagonSelectionInstrument::completeMoving(ImageArea &)
{
	
}

// add
void ConvexPentagonSelectionInstrument::clearSelectionBackground(ImageArea &imageArea)
{

	
	QPainter blankPainter(imageArea.getImage());
	blankPainter.setPen(Qt::white);
	blankPainter.setBrush(QBrush(Qt::white));
	blankPainter.setBackgroundMode(Qt::OpaqueMode);
	
	//copied justine code from CR#2 to draw convex oentagon
	QPolygon *poly = new QPolygon(QRect(mTopLeftPoint, mBottomRightPoint), true);
	QPoint *fifthPoint = new QPoint(((int)mTopLeftPoint.rx() + (int)mBottomRightPoint.rx()) / 2,
		((int)mTopLeftPoint.ry() / 4));
	poly->setPoint(0, *fifthPoint);
	
	blankPainter.drawPolygon(*poly);
	blankPainter.end();
	mImageCopy = *imageArea.getImage();
	
	
}

void ConvexPentagonSelectionInstrument::clear()
{
	mSelectedImage = QImage();
	emit sendEnableCopyCutActions(false);
}
//add
void ConvexPentagonSelectionInstrument::paint(ImageArea &imageArea, bool, bool)
{

	if (mIsSelectionExists)
	{
		if (mTopLeftPoint != mBottomRightPoint)
		{
			//mustafa

			//copied justine code from CR#2 to draw convex oentagon
			QPainter painter(imageArea.getImage());
			//QPolygon *poly = new QPolygon(QRect(mTopLeftPoint, mBottomRightPoint), true);
			//QPoint *fifthPoint = new QPoint(((int)mTopLeftPoint.rx() + (int)mBottomRightPoint.rx()) / 2,
			//	((int)mTopLeftPoint.ry() / 4));
			//poly->setPoint(0, *fifthPoint);
			
			//crop the image into a  region of it
			//painter.setClipRegion(QRegion(*poly));
			QRect source(0, 0, mSelectedImage.width(), mSelectedImage.height());
			QRect target(mTopLeftPoint, mBottomRightPoint);
			painter.drawImage(target, mSelectedImage, source);
			painter.end();
		}
		imageArea.setEdited(true);
		imageArea.update();
	}
}

void ConvexPentagonSelectionInstrument::showMenu(ImageArea &)
{
}

