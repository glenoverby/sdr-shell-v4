#include "freqlabel.h"

Freqlabel::Freqlabel(QWidget *parent) : QLabel(parent)
{
    p = this->palette();
    q = this->palette();
    p.setColor( QPalette::Window, QColor(0, 0, 0, 0) );
    this->setPalette(p);
    // set label background
    q.setColor( QPalette::Window, QColor(255, 0, 0, 127) );   //transparent red
//    q.setColor( QPalette::Window, QColor(0, 0, 255, 127) );   //transparent blue
//    q.setColor( QPalette::Window, QColor(255, 255, 0, 127) ); //transparent yellow
    this->setAutoFillBackground(true);
}

bool Freqlabel::event ( QEvent *e )
{
  if (e->type() == QEvent::Wheel) {
     QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(e);
     if ( wheelEvent->delta() < 0 ) 
        emit mouseWheel( increment );
       else
         emit mouseWheel( -increment );
     return 0;
  }

  switch ( e->type() ) {

  case QEvent::Enter:
       this->setPalette(q);
       break;
  case QEvent::Leave:
       this->setPalette(p);
       break;
  default:
       break;
  }
}  

void Freqlabel::setIncrement( int i )
{
  increment = i;
}
