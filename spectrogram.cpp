#include <cstdio>
#include "spectrogram.h"

//Spectrogram::Spectrogram(QWidget *parent, const char *name) : QWidget(parent, name)
Spectrogram::Spectrogram(QWidget *parent) : QWidget(parent)
{
    setMouseTracking( true );

    mouseMoving = 0;
}

void Spectrogram::mouseReleaseEvent( QMouseEvent *e )
{
//printf("spectrogram mouseReleaseEvent \n");
    if ( !mouseMoving && e->button() == Qt::LeftButton )
    {
//     printf("e->x() = %d\n", e->x());
//     printf("e->gloabalX() = %d\n", e->globalX());     
//     printf("e->y() = %d\n", e->y());
//     printf("e->globalY() = %d\n", e->globalY());     
        emit tune1( e->x() );
        }

    if ( !mouseMoving && e->button() == Qt::RightButton )
        emit plot( e->y() );    

    mouseMoving = false;
}

void Spectrogram::mouseMoveEvent( QMouseEvent *e )
{
//printf("spectrogram mouseMoveEvent \n");
    static int x0 = 0;
    int output;
	
    mouseMoving = true;

    if ( x0 - e->x() >= 0 )
        output = 1;
    else
        output = -1;

    if ( e->buttons() == Qt::LeftButton ) 
        emit tune2( output );
    else if ( e->buttons() == Qt::RightButton )
        emit tune2( output * 10 );
    else if ( e->buttons() == Qt::MidButton )
        emit tune2( output * 100 );
    else
        emit movement( e->x() );


    x0 = e->x();
}

void Spectrogram::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    char orientation = '?';
    int orient = 0, shift = 0, ctl = 0, alt = 0;

    if (event->orientation() == Qt::Horizontal) {
		orientation = 'h';
		orient = 100;
    } else {
		orientation = 'v';
		orient = 1000;
    }

#if 0
   if (event->state() & Qt::ShiftButton)
	shift = 1;
   if (event->state() & Qt::AltButton)
	alt = 1;
   if (event->state() & Qt::ControlButton)
	ctl = 1;
#endif

    printf("wheelEvent degrees %d steps %d orientation %c %c %c %c\n",
		numDegrees, numSteps, orientation, shift, ctl, alt);

    if (event->orientation() == Qt::Horizontal) {
    	emit tune2( orient * -numSteps +  10000 * shift + 100000 * alt);
	} else {
    	emit tune3( -numSteps );
	}
}

