#include "freeformselectioninstrument.h"
#include "../datasingleton.h"
#include "../imagearea.h"
#include "../undocommand.h"
#include "math.h"

#include <QPainter>
#include <QPen>
#include <QApplication>
#include <QClipboard>

FreeformSelectionInstrument::FreeformSelectionInstrument(QObject *parent) :
	AbstractSelection(parent)
{
}

void FreeformSelectionInstrument::copyImage(ImageArea &imageArea)
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

void FreeformSelectionInstrument::cutImage(ImageArea &imageArea)
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

void FreeformSelectionInstrument::pasteImage(ImageArea &imageArea)
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


void FreeformSelectionInstrument::startAdjusting(ImageArea & imageArea)
{
	mImageCopy = *imageArea.getImage();
	mIsImageSelected = false;
}

void FreeformSelectionInstrument::startSelection(ImageArea &)
{
}

void FreeformSelectionInstrument::startResizing(ImageArea &imageArea)
{
	if (!mIsImageSelected)
	{
		clearSelectionBackground(imageArea);
	}
}

void FreeformSelectionInstrument::startMoving(ImageArea &imageArea)
{
	clearSelectionBackground(imageArea);
}

void FreeformSelectionInstrument::select(ImageArea &)
{
}

void FreeformSelectionInstrument::resize(ImageArea &)
{
}

void FreeformSelectionInstrument::move(ImageArea &)
{
}

void FreeformSelectionInstrument::completeSelection(ImageArea &imageArea)
{
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
		mTopLeftPoint.y(),
		mWidth, mHeight);
	emit sendEnableCopyCutActions(true);
}

void FreeformSelectionInstrument::completeResizing(ImageArea &imageArea)
{
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
		mTopLeftPoint.y(),
		mWidth, mHeight);
}

void FreeformSelectionInstrument::completeMoving(ImageArea &)
{
}

void FreeformSelectionInstrument::clearSelectionBackground(ImageArea &imageArea)
{
	QPainter blankPainter(imageArea.getImage());
	blankPainter.setPen(Qt::white);
	blankPainter.setBrush(QBrush(Qt::white));
	blankPainter.setBackgroundMode(Qt::OpaqueMode);

	QVector<QPoint> points;
	QPoint *thirdPoint = new QPoint(((int)mTopLeftPoint.rx() + (int)mBottomRightPoint.rx()) / 2,
		((int)mTopLeftPoint.ry() - 4));
	points.append(mTopLeftPoint);
	points.append(mBottomRightPoint);
	points.append(*thirdPoint);
	QPolygon *poly = new QPolygon(points);
	blankPainter.drawPolygon(*poly);
	blankPainter.end();
	mImageCopy = *imageArea.getImage();
}

void FreeformSelectionInstrument::clear()
{
	mSelectedImage = QImage();
	emit sendEnableCopyCutActions(false);
}

void FreeformSelectionInstrument::paint(ImageArea &imageArea, bool isSecondaryColor, bool)
{
	if (mIsSelectionExists)
	{
		QPainter painter(imageArea.getImage());
		if (isSecondaryColor)
		{
			painter.setPen(QPen(DataSingleton::Instance()->getSecondaryColor(),
				DataSingleton::Instance()->getPenSize() * imageArea.getZoomFactor(),
				Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		}
		else
		{
			painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor(),
				DataSingleton::Instance()->getPenSize() * imageArea.getZoomFactor(),
				Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		}

		if (mStartPoint != mEndPoint)
		{
			painter.drawLine(mStartPoint, mEndPoint);
		}

		if (mStartPoint == mEndPoint)
		{
			painter.drawPoint(mStartPoint);
		}
		imageArea.setEdited(true);
		painter.end();
		imageArea.update();
	}
}

void FreeformSelectionInstrument::showMenu(ImageArea &)
{
}
