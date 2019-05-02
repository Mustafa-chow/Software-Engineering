/*JUSTIN:
Added convex pentagon class. Mostly the same as other instrument classes
except added signals and slots for user defined text stamping.
*/
#ifndef CONVEXPENTAGONINSTRUMENT_H
#define CONVEXPENTAGONINSTRUMENT_H

#include "abstractinstrument.h"

#include <QtCore/QObject>

/**
* @brief Convex Pentagon instrument class.
*
*/
class ConvexPentagonInstrument : public AbstractInstrument
{
	Q_OBJECT
public:
	explicit ConvexPentagonInstrument(QObject *parent = 0);

	void mousePressEvent(QMouseEvent *event, ImageArea &imageArea);
	void mouseMoveEvent(QMouseEvent *event, ImageArea &imageArea);
	void mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea);
	void textDialog(ImageArea &imageArea);
	void paintText(ImageArea &imageArea);

protected:
	void paint(ImageArea &imageArea, bool isSecondaryColor = false, bool additionalFlag = false);
private:
	QString mText;
signals:
	void sendCloseTextDialog();
private slots:
	void updateText(ImageArea *imageArea, QString textString);
	void cancel(ImageArea *imageArea);
};

#endif
