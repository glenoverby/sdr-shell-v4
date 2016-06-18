
#include <cstdio>
#include "lcdfreq.h"
#include "cmath"

LCDFreq::LCDFreq (QWidget *parent) : QLCDNumber (parent)
{
}

LCDFreq::LCDFreq (uint numDigits, QWidget *parent) : QLCDNumber (numDigits, parent)
{
}

int LCDFreq::digit(int x)
{
	int index[] = { 34, 47, 60, 86, 99, 112, 125, 138, 151, 0 };
	int digit;
	// map the x offset to a digit on the LCD type display.  The LCD is fixed with, making this
	// easy.  The index[] array, above, is the left edge of the digit.
	for (digit=0; index[digit] != 0; digit++) {
		if (x >= index[digit] && x < index[digit]+13) {
			break;
		}
	}
	if (index[digit] == 0)
		digit = -1;
	digit = 8 - digit;
	return digit;
}

void LCDFreq::mouseReleaseEvent( QMouseEvent *e )
{
	int d;
#if 0
	int index[] = { 34, 47, 60, 86, 99, 112, 125, 138, 151, 0 };
	int x;
	int digit;
	// map the x offset to a digit on the LCD type display.  The LCD is fixed with, making this
	// easy.  The index[] array, above, is the left edge of the digit.
	x = e->x();
	for (digit=0; index[digit] != 0; digit++) {
		if (x >= index[digit] && x < index[digit]+13) {
			break;
		}
	}
	if (index[digit] == 0)
		digit = -1;
	digit = 8 - digit;
#endif

	d = digit(e->x());
	if (d != -1) {
		if (e->button() & Qt::LeftButton) {
			printf("left button x %d y %d digit %d\n", e->x(), e->y(), d);
			emit tuneStep( d );
		}
	}
}

void LCDFreq::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    int orient = 0;
	int d;

    if (event->orientation() == Qt::Horizontal) {
		orient = 0;
    } else {
		orient = 1;
    }
	d = digit((int)event->x());
	printf("wheel turn steps %d orientation %d digit %d %f\n", numSteps, orient, d, pow(10, d));
	emit tune3 ( numSteps * (int)pow(10, d));
}

