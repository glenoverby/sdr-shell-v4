
#include <cstdio>
#include "idbutton.h"

//IdPushButton::IdPushButton(QPushButton *parent)
IdPushButton::IdPushButton(QWidget *parent) : QPushButton (parent)
{
	id = -1;
	connect ( this, SIGNAL ( clicked() ), this, SLOT ( nclick() ) );
}

void IdPushButton::setID( int v )
{
	id = v;
}

int IdPushButton::getID( )
{
	return id;
}
void IdPushButton::nclick ( )
{
	emit selected ( id );
}
