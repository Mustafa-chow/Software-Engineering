

#include "scaleneselectioninstrument.h"
#include "../imagearea.h"
#include "../undocommand.h"
#include "math.h"

#include <QPainter>
#include <QApplication>
#include <QClipboard>

ScaleneSelectionInstrument::ScaleneSelectionInstrument(QObject *parent) :
	AbstractSelection(parent)
{
}

void ScaleneSelectionInstrument::copyImage(ImageArea &imageArea)
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

void ScaleneSelectionInstrument::cutImage(ImageArea &imageArea)
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

void ScaleneSelectionInstrument::pasteImage(ImageArea &imageArea)
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


void ScaleneSelectionInstrument::startAdjusting(ImageArea & imageArea)
{
	mImageCopy = *imageArea.getImage();
	mIsImageSelected = false;
}

void ScaleneSelectionInstrument::startSelection(ImageArea &)
{
}

void ScaleneSelectionInstrument::startResizing(ImageArea &imageArea)
{
	if (!mIsImageSelected)
	{
		clearSelectionBackground(imageArea);
	}
}

void ScaleneSelectionInstrument::startMoving(ImageArea &imageArea)
{
	clearSelectionBackground(imageArea);
}

void ScaleneSelectionInstrument::select(ImageArea &)
{
}

void ScaleneSelectionInstrument::resize(ImageArea &)
{
}

void ScaleneSelectionInstrument::move(ImageArea &)
{
}

void ScaleneSelectionInstrument::completeSelection(ImageArea &imageArea)
{
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
		mTopLeftPoint.y(),
		mWidth, mHeight);
	emit sendEnableCopyCutActions(true);
}

void ScaleneSelectionInstrument::completeResizing(ImageArea &imageArea)
{
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
		mTopLeftPoint.y(),
		mWidth, mHeight);
}

void ScaleneSelectionInstrument::completeMoving(ImageArea &)
{
}

void ScaleneSelectionInstrument::clearSelectionBackground(ImageArea &imageArea)
{
	QPainter blankPainter(imageArea.getImage());
	blankPainter.setPen(Qt::white);
	blankPainter.setBrush(QBrush(Qt::white));
	blankPainter.setBackgroundMode(Qt::OpaqueMode);
	//create a vector for points
	QVector<QPoint> points;
	QPoint *thirdPoint = new QPoint(((int)mTopLeftPoint.rx() + (int)mBottomRightPoint.rx()) / 2,
		((int)mTopLeftPoint.ry() - 4));
	points.append(mTopLeftPoint);
	points.append(mBottomRightPoint);
	points.append(*thirdPoint);
	//create a scalene triangle from start point, end point and a third point
	QPolygon *poly = new QPolygon(points);
	blankPainter.drawPolygon(*poly);
	blankPainter.end();
	mImageCopy = *imageArea.getImage();
}

void ScaleneSelectionInstrument::clear()
{
	mSelectedImage = QImage();
	emit sendEnableCopyCutActions(false);
}

void ScaleneSelectionInstrument::paint(ImageArea &imageArea, bool, bool)
{
	if (mIsSelectionExists)
	{
		if (mTopLeftPoint != mBottomRightPoint)
		{	
			QPainter painter(imageArea.getImage());
			//create the same triangle as selected
			QVector<QPoint> points;
			QPoint *thirdPoint = new QPoint(((int)mTopLeftPoint.rx() + (int)mBottomRightPoint.rx()) / 2,
				((int)mTopLeftPoint.ry() - 4));
			points.append(mTopLeftPoint);
			points.append(mBottomRightPoint);
			points.append(*thirdPoint);
			QPolygon *poly = new QPolygon(points);
			//crop the image into a triangle region of it
			painter.setClipRegion(QRegion(*poly));
			QRect source(0, 0, mSelectedImage.width(), mSelectedImage.height());
			QRect target(mTopLeftPoint, mBottomRightPoint);
			painter.drawImage(target, mSelectedImage, source);
			painter.end();
		}
		imageArea.setEdited(true);
		imageArea.update();
	}
}

void ScaleneSelectionInstrument::showMenu(ImageArea &)
{
}

