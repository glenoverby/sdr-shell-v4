#ifndef FREQLABEL_H
#define FREQLABEL_H

#include <QLabel>
#include <QWheelEvent>

class Freqlabel : public QLabel
{
    Q_OBJECT
    
    public:
    	Freqlabel(QWidget *parent = 0);    
        void setIncrement( int );

    private:
        int increment;
        QPalette p, q;

    protected:
//        void wheelEvent ( QWheelEvent * );
        bool event ( QEvent * );

    signals:
        void mouseWheel( int );

};
#endif
