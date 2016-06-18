#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <qframe.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qtimer.h>

#include <time.h>
#include <sys/timeb.h>
#include <sys/time.h>

#include "sunpropagator.h"
#include "moonpropagator.h"

class WorldMap : public QFrame
{
    Q_OBJECT
    
    public:
        WorldMap(QWidget *parent = 0, const char *name = 0);
        void setObserver( double, double );
        
    public slots:
        void compute();
        void plot();
        
    private:
        SunPropagator *sun;
        MoonPropagator *moon;
        QPointArray *sunPoints;
        QPixmap *world_pix;
        QWidget *map;
        QLabel *sunLabel;
        QLabel *moonLabel;
                
        int sun_x, sun_y, moon_x, moon_y;
        double my_lon, my_lat;
        int plotFlag;
        
        struct tm *tm_ptr;

        void drawMap();
        double arccos( double, double );
        void calculateSunFootprint();
        
    protected:

    signals:

};
#endif
