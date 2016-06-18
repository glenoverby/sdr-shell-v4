#ifndef SDXCVR_PBSCALE_H
#define SDXCVR_PBSCALE_H

#include <qwidget.h>
#include <QMouseEvent>

class PassBandScale : public QWidget
{
    Q_OBJECT
    
    public:
//    PassBandScale(QWidget *parent = 0, const char *name = 0, WFlags f = 0 );
//    PassBandScale(QWidget *parent = 0, const char *name = 0);
    PassBandScale(QWidget *parent = 0);
    
    private:
        int x0;

    protected:
        void mousePressEvent( QMouseEvent * );
        void mouseMoveEvent( QMouseEvent * );

    signals:
		void set_lower_pb( int );
		void set_upper_pb( int );
		void movement( int );
};
#endif
