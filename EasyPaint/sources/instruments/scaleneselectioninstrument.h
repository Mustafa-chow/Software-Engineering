
#ifndef SCALENESELECTIONINSTRUMENT_H
#define SCALENESELECTIONINSTRUMENT_H

#include "abstractselection.h"

QT_BEGIN_NAMESPACE
class QUndoStack;
QT_END_NAMESPACE

class ScaleneSelectionInstrument : public AbstractSelection
{
	Q_OBJECT

public:
	explicit ScaleneSelectionInstrument(QObject *parent = 0);

	/**
	* @brief Clears background image at selection area.
	*
	* @param imageArea ImageArea for applying changes.
	*/
	void clearSelectionBackground(ImageArea &imageArea);
	/**
	* @brief Copying image to the clipboard.
	*
	* @param imageArea ImageArea for applying changes.
	*/
	void copyImage(ImageArea &imageArea);
	/**
	* @brief Paste image from the clipboard.
	*
	* @param imageArea ImageArea for applying changes.
	*/
	void pasteImage(ImageArea &imageArea);
	/**
	* @brief Cut image to the clipboard.
	*
	* @param imageArea ImageArea for applying changes.
	*/
	void cutImage(ImageArea &imageArea);
	

private:
	void startAdjusting(ImageArea &imageArea);
	void startSelection(ImageArea &);
	void startResizing(ImageArea &imageArea);
	void startMoving(ImageArea &imageArea);
	void select(ImageArea &);
	void resize(ImageArea &);
	void move(ImageArea &);
	void completeSelection(ImageArea &imageArea);
	void completeResizing(ImageArea &imageArea);
	void completeMoving(ImageArea &);
	void clear();
	void paint(ImageArea &imageArea, bool = false, bool = false);
	void showMenu(ImageArea &);
	
	

	QImage mSelectedImage, /**< Copy of selected image. */
		mPasteImage; /**< Image to paste */
	QPolygon mScaleneSelect;

signals:
	void sendEnableCopyCutActions(bool enable);
	void sendEnableSelectionInstrument(bool enable);

};

#endif // SCALENESELECTIONINSTRUMENT_H
