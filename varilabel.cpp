#include "varilabel.h"

Varilabel::Varilabel(QWidget *parent) : QLabel(parent)
{
}

void Varilabel::mouseMoveEvent( QMouseEvent *e )
{
    static int previous_y;
    
    // 1 pulse every 10 pixels
    if ( e->y() > previous_y + 10 || e->y() < previous_y - 10) {
        if ( e->y() > previous_y ) emit pulse( -1 );
        else emit pulse( 1 );
        previous_y = e->y();
    }

    // 1 pulse every pixel
    emit y( e->y() );
}

void Varilabel::mouseReleaseEvent ( QMouseEvent *e )
{
	if (e->button() & Qt::LeftButton)
    	emit mouseRelease( label );
	if (e->button() & Qt::RightButton)
    	emit mouseRelease2( label );
	if (e->button() & Qt::MidButton)
    	emit mouseRelease3( label );
}

void Varilabel::setLabel( int l )
{
  label = l;
}


VariModelabel::VariModelabel(QWidget *parent) : Varilabel (parent)
{
}

void VariModelabel::mouseReleaseEvent ( QMouseEvent *e )
{
    bool force = 0;
#if 0
    if (e->state() & ShiftButton) {
	fprintf( stderr, "VariModelabel mouseReleaseEvent: ShiftButton\n");
	force = 1;
    }
#endif
    emit mouseRelease( label, FALSE, force );
}

void VariModelabel::setLabel( rmode_t l )
{
	label = l;
}
