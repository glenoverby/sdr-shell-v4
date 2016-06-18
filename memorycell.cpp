#include "memorycell.h"

MemoryCell::MemoryCell(QWidget *parent) : QLabel(parent)
{
    id = 0;
    frequency = 0;
    mode = 0;
    filter_l = -2400;
    filter_h = -20;

	setMouseTracking( true );
}

void MemoryCell::setID( int i )
{
    id = i;
}

void MemoryCell::setMode( rmode_t m )
{
    mode = m;
}

void MemoryCell::setFilter( int l, int h )
{
    filter_l = l;
    filter_h = h;
}

void MemoryCell::setFrequency( long long int f )
{
    frequency = f;
}

void MemoryCell::setTxFrequency( long long int f )
{
    txFrequency = f;
}

int MemoryCell::getID()
{
    return id;
}

int MemoryCell::getMode()
{
    return mode;
}

int MemoryCell::getFilter_l()
{
    return filter_l;
}

int MemoryCell::getFilter_h()
{
    return filter_h;
}

long long int MemoryCell::getFrequency()
{
    return frequency;
}

long long int MemoryCell::getTxFrequency()
{
    return txFrequency;
}

void MemoryCell::mouseReleaseEvent ( QMouseEvent *e )
{
    if ( e->button() == Qt::LeftButton )
        emit read( this );
    else if ( e->button() == Qt::MidButton )
        emit write( this );
}

void MemoryCell::enterEvent( QEvent * )
{
	//fprintf( stderr, "memory cell mouse enter\n" );
	emit display( this );
}

void MemoryCell::leaveEvent( QEvent * )
{
	//fprintf( stderr, "memory cell mouse leave\n" );
	//emit display( this );
}
