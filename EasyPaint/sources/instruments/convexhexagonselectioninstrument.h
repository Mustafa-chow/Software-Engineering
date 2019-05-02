#pragma once
/*
Jacob
This is the header file for the convex hexagex selection shape.
This file contains the functions that all selection instruments utilize.

*/

#ifndef CONVEXHEXAGONSELECTIONINSTRUMENT_H
#define CONVEXHEXAGONSELECTIONINSTRUMENT_H

#include "abstractselection.h"

QT_BEGIN_NAMESPACE
class QUndoStack;
QT_END_NAMESPACE

class ConvexHexagonSelectionInstrument : public AbstractSelection
{
	Q_OBJECT

private:
	void startResizing(ImageArea &imageArea);
	void startMoving(ImageArea &imageArea);
	void startAdjusting(ImageArea &imageArea);
	void startSelection(ImageArea &);

	void select(ImageArea &);
	void resize(ImageArea &);
	void move(ImageArea &);
	void completeSelection(ImageArea &imageArea);
	void completeResizing(ImageArea &imageArea);
	void completeMoving(ImageArea &);
	void clear();
	void paint(ImageArea &imageArea, bool = false, bool = false);
	void showMenu(ImageArea &);

public:
	explicit ConvexHexagonSelectionInstrument(QObject *parent = 0);
	void copyImage(ImageArea &imageArea);
	void pasteImage(ImageArea &imageArea);
	void cutImage(ImageArea &imageArea);
	void clearSelectionBackground(ImageArea &imageArea);
	QImage mSelectedImage, mPasteImage; 

signals:
	void sendEnableCopyCutActions(bool enable);
	void sendEnableSelectionInstrument(bool enable);
};

#endif 