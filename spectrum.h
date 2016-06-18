#ifndef SDXCVR_SPECTRUM_H
#define SDXCVR_SPECTRUM_H

#include <qwidget.h>
#include <QMouseEvent>

class Spectrum : public QWidget
{
    Q_OBJECT
    
    public:
//        Spectrum(QWidget *parent = 0, const char *name = 0, WFlags f = 0 );
        Spectrum(QWidget *parent = 0);
    private:
        int mouseMoving;

    protected:
        void mouseReleaseEvent( QMouseEvent * );
        void mouseMoveEvent( QMouseEvent * );
	void wheelEvent(QWheelEvent *event);

    signals:
		void movement( int );
		void tune( int );
       	void tune2( int );
       	void tune3( int );
};
#endif
