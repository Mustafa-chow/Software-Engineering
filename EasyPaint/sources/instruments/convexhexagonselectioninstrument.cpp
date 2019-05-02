/*
Jacob
This is the source file for the convex hexagex selection shape.
This file contains the functions that all selection instruments utilize and their definitions.

*/

#include "convexhexagonselectioninstrument.h"
#include "../imagearea.h"
#include "../undocommand.h"
#include "math.h"
#include <QPainter>
#include <QApplication>
#include <QClipboard>

//this instrument inherits from the AbstractSelection class. These functions redefine their definitions
ConvexHexagonSelectionInstrument::ConvexHexagonSelectionInstrument(QObject *parent) :AbstractSelection(parent){ 
}


void ConvexHexagonSelectionInstrument::startAdjusting(ImageArea & imageArea){
	mImageCopy = *imageArea.getImage();
	mIsImageSelected = false;
}

void ConvexHexagonSelectionInstrument::startSelection(ImageArea &){
}


void ConvexHexagonSelectionInstrument::select(ImageArea &){
}

void ConvexHexagonSelectionInstrument::resize(ImageArea &){
}

void ConvexHexagonSelectionInstrument::move(ImageArea &){
}


void ConvexHexagonSelectionInstrument::showMenu(ImageArea &){
}

void ConvexHexagonSelectionInstrument::startResizing(ImageArea &imageArea){
	if (!mIsImageSelected)
	{
		clearSelectionBackground(imageArea);
	}
}

void ConvexHexagonSelectionInstrument::startMoving(ImageArea &imageArea){
	clearSelectionBackground(imageArea);
}


void ConvexHexagonSelectionInstrument::completeSelection(ImageArea &imageArea){
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
	mTopLeftPoint.y(),mWidth, mHeight); 
	emit sendEnableCopyCutActions(true);
}

void ConvexHexagonSelectionInstrument::completeResizing(ImageArea &imageArea){
	mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),mTopLeftPoint.y(),mWidth, mHeight);
}

void ConvexHexagonSelectionInstrument::completeMoving(ImageArea &){
}

void ConvexHexagonSelectionInstrument::copyImage(ImageArea &imageArea)
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

void ConvexHexagonSelectionInstrument::cutImage(ImageArea &imageArea)
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
		if (mIsImageSelected)
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

void ConvexHexagonSelectionInstrument::pasteImage(ImageArea &imageArea)
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

void ConvexHexagonSelectionInstrument::clearSelectionBackground(ImageArea &imageArea)
{
	QPainter Painter(imageArea.getImage());
	//These are the attributes of clearing the area.
	Painter.setPen(Qt::white);
	Painter.setBrush(QBrush(Qt::white));
	Painter.setBackgroundMode(Qt::OpaqueMode); 

	//find the hexagons points and create a QPolygon shape. Then remove the inside of that polygon.
	QPainter painter(imageArea.getImage());
	//creating the polygon
	QPointF point[7];
	int size = mEndPoint.rx() - mStartPoint.rx();

	point[0].setX(mStartPoint.x());
	point[0].setY(mStartPoint.y());

	point[1].setX(mStartPoint.x() - (size / 5));
	point[1].setY(mStartPoint.y() + (size / 15));

	point[2].setX(point[1].x() - (size / 5));
	point[2].setY(point[1].y() - (size / 15));


	point[3].setX(point[2].x());
	point[3].setY(point[2].y() - (size / 15));

	point[4].setX(point[3].x() + (size / 5));
	point[4].setY(point[3].y() - (size / 15));

	point[5].setX(point[4].x() + (size / 5));
	point[5].setY(point[4].y() + (size / 25));

	point[6].setX(point[0].x());
	point[6].setY(point[0].y());

	//paint the 'clear' polygon, effectively removing that area.
	QPolygon *poly = new QPolygon(QRect(mTopLeftPoint, mBottomRightPoint), true);
	Painter.drawPolygon(*poly);
	Painter.end();
	mImageCopy = *imageArea.getImage();
}

void ConvexHexagonSelectionInstrument::clear()
{
	mSelectedImage = QImage();
	emit sendEnableCopyCutActions(false);
}

void ConvexHexagonSelectionInstrument::paint(ImageArea &imageArea, bool, bool)
{
	if (mIsSelectionExists)
	{
		if (mTopLeftPoint != mBottomRightPoint)
		{
			//draw the dotted polygon shape based on points of the cursor dragging location.
			QPainter painter(imageArea.getImage());

			QPointF point[7];
			int size = mEndPoint.rx() - mStartPoint.rx();

			point[0].setX(mStartPoint.x());
			point[0].setY(mStartPoint.y());

			point[1].setX(mStartPoint.x() - (size / 5));
			point[1].setY(mStartPoint.y() + (size / 15));

			point[2].setX(point[1].x() - (size / 5));
			point[2].setY(point[1].y() - (size / 15));


			point[3].setX(point[2].x());
			point[3].setY(point[2].y() - (size / 15));

			point[4].setX(point[3].x() + (size / 5));
			point[4].setY(point[3].y() - (size / 15));

			point[5].setX(point[4].x() + (size / 5));
			point[5].setY(point[4].y() + (size / 25));

			point[6].setX(point[0].x());
			point[6].setY(point[0].y());

			// create the QPolygon object
			QPolygon poly;
			poly << point[0].toPoint();
			poly << point[1].toPoint();
			poly << point[2].toPoint();
			poly << point[3].toPoint();
			poly << point[4].toPoint();
			poly << point[5].toPoint();
			poly << point[6].toPoint();

			poly.setPoint(6, point[0].toPoint());

			//draw the dotted figure hexagon
			QPainterPath path;
			path.addPolygon(QPolygon(poly));
			painter.drawPath(path);
			//can be customizable/cut
			imageArea.setEdited(true);
			painter.end();
			imageArea.update();
		}
	}
}



