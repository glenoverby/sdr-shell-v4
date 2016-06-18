#ifndef SDXCVR_SPECTROGRAM_H
#define SDXCVR_SPECTROGRAM_H

#include <qwidget.h>
#include <QMouseEvent>

class Spectrogram : public QWidget
{
    Q_OBJECT
    
    public:
//        Spectrogram(QWidget *parent = 0, const char *name = 0);
        Spectrogram(QWidget *parent = 0);
    private:
        int mouseMoving;

    protected:
        void mouseReleaseEvent( QMouseEvent * );
        void mouseMoveEvent( QMouseEvent * );
	void wheelEvent(QWheelEvent *event);

    signals:
        void tune1( int );
        void tune2( int );
        void tune3( int );
        void plot( int );
	void movement( int );
};
#endif
