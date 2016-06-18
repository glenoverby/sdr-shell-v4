#ifndef LCDFREQ_H
#define LCDFREQ_H

#include <QLabel>
#include <QLCDNumber>
#include <QMouseEvent>

class LCDFreq : public QLCDNumber
{
	Q_OBJECT

	public:
		explicit LCDFreq(QWidget *parent = 0);
		explicit LCDFreq(uint numDigits, QWidget *parent = 0);

    protected:
        void mouseReleaseEvent ( QMouseEvent * );
		void wheelEvent ( QWheelEvent * );

    signals:
        void tuneStep( int );
		void tune3 (int );

	private:
		int digit ( int );
};
#endif
