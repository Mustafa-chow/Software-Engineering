// Mustafa
// irregular pentagon

#ifndef IrregularPentagonInstrument_H
#define IrregularPentagonInstrument_H

#include "abstractinstrument.h"

#include <QtCore/QObject>

/**
* @brief Concave Pentagon instrument class.
*
*/
class IrregularPentagonInstrument : public AbstractInstrument
{
	Q_OBJECT
public:
	explicit IrregularPentagonInstrument(QObject *parent = 0);

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

