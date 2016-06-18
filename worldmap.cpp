#include "worldmap.h"
#include "world.xpm"

WorldMap::WorldMap(QWidget *parent, const char *name) : QFrame(parent, name)
{
    world_pix = new QPixmap( world_xpm );

    map = new QWidget( this );
    map->setGeometry( 2, 2, world_pix->width(), world_pix->height() );
    map->setPaletteBackgroundPixmap( *world_pix );

    sun = new SunPropagator();
    moon = new MoonPropagator();
    sunPoints = new QPointArray( 360 );

    QFrame *line = new QFrame( this );
    line->setGeometry ( 2, map->height() + 2, 240, 1 );
    line->setPaletteBackgroundColor( QColor( 150, 150, 150 ) );
    
    sunLabel = new QLabel( this );
    sunLabel->setFont( QFont( "Courier", 8, QFont::Black, FALSE) );
    sunLabel->setPaletteForegroundColor( QColor( 255, 200, 0 ) );
    sunLabel->setPaletteBackgroundColor( QColor( 0, 20, 60 ) );
    sunLabel->setGeometry( 2, map->height() + 3, 120, 14 );
    sunLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    moonLabel = new QLabel( this );
    moonLabel->setFont( QFont( "Courier", 8, QFont::Black, FALSE) );
    moonLabel->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    moonLabel->setPaletteBackgroundColor( QColor( 0, 20, 60 ) );
    moonLabel->setGeometry( 122, map->height() + 3, 120, 14 );
    moonLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    plot();
    compute();
    
    QTimer *mapTimer1 = new QTimer( this );
    connect( mapTimer1, SIGNAL(timeout()), this, SLOT(compute()) );
    mapTimer1->start( 10000, FALSE );

    QTimer *mapTimer2 = new QTimer( this );
    connect( mapTimer2, SIGNAL(timeout()), this, SLOT(plot()) );
    mapTimer2->start( 60000, FALSE );
    
}

void WorldMap::setObserver( double lat, double lon )
{
    sun->setObserver( lat, lon );
    moon->setObserver( lat, lon );

    //printf("> setObserver( %f, %f )\n", lat, lon );
}

void WorldMap::compute()
{
    struct timeb t_ptr;
    struct timeval tv;
    double daynum;
    char tmp[25];

    ftime( &t_ptr );
    gettimeofday( &tv, NULL );
    tm_ptr = gmtime( &t_ptr.time );

    daynum =  ((((double)tv.tv_sec + 0.001 * (tv.tv_usec/1000.0)) / 86400.0 ) - 3651.0);
    //    strftime( timeStamp, 25, "%a %Y-%m-%d %H:%M UTC", tm_ptr );

    sun->propagate( daynum );
    moon->propagate( daynum );

    //printf("> 1 %d %d %f\n", tv.tv_sec, tv.tv_usec, daynum);
    
    if ( plotFlag ) {
        calculateSunFootprint();
        drawMap();
        plotFlag = 0;
    }
    
    sprintf( tmp, "%3.1f : %2.1f\n", sun->getAz(), sun->getEl() );
    sunLabel->setText( tmp );
    sprintf( tmp, "%3.1f : %2.1f\n", moon->getAz(), moon->getEl() );
    moonLabel->setText( tmp );
}

void WorldMap::plot()
{
    plotFlag = 1;
}

void WorldMap::drawMap()
{
    QPainter p;
    QPen pen1, pen2, pen3;
    QPixmap tmpMap = *world_pix;

    pen1.setColor( QColor( 255, 200, 0 ) );
    pen2.setColor( QColor( 255, 255, 255 ) );
    pen3.setColor( QColor( 150, 150, 150 ) );

    p.begin( &tmpMap );

    // Draw the moon
    p.setPen( pen2 );
    p.drawRect( moon_x - 1, moon_y - 1, 3, 3 );

    // Draw the sun terminator
    p.setPen( pen3 );
    //for ( int i = 0; i < 360 - 1; i++ ) {
    //    if ( sunPoints->point( i ).x() <= sunPoints->point( i + 1 ).x() ) {
    //        p.drawLine( sunPoints->point( i ), sunPoints->point( i + 1 ) );
    //    }
    //    //printf( "%d %d %d\n", i, sunPoints->point( i ).x(), sunPoints->point( i + 1 ).x() );
    //}
    p.drawPoints( *sunPoints );

    // Draw the sun
    p.setPen( pen1 );
    p.drawRect( sun_x - 1, sun_y - 1, 3, 3 );

    p.end();

    map->setBackgroundPixmap( tmpMap );
}

// The code for the functions below were borowed from Predict
double WorldMap::arccos( double x, double y )
{
   double result = 0, fraction;

   fraction=x/y;

   if ((x>0.0) && (y>0.0))
      result=acos(fraction);

   if ((x<0.0) && (y>0.0))
      result=acos(fraction);

   if ((x<0.0) && (y<0.0))
      result = 3.14159 +  acos(fraction);

   if ((x>0.0) && (y<0.0))
      result = 3.14159 + acos(fraction);

   return result;
}

void WorldMap::calculateSunFootprint()
{
    int azi, xx, yy;
    float ssplat, ssplong, azimuth;
    float num, dem, beta, spiderlat, spiderlong;
    float height = (float)world_pix->height();
    float width = (float)world_pix->width();
    float TWOPI = 2.0 * M_PI;
    float R0=6378.16;

    ssplat = sun->getLat() * deg2rad;
    ssplong = sun->getLon() * deg2rad;
    beta = 0.5 * 20000 / R0;

    /* Compute the visibility circle */
    for ( azi = 0; azi < 360; azi++)
    {
        azimuth = deg2rad * (double)azi;
        spiderlat = asin( sin( ssplat ) * cos( beta ) +
                    cos( azimuth ) * sin( beta ) * cos( ssplat ));
        num = cos( beta ) - ( sin( ssplat ) * sin( spiderlat ));
        dem = cos( ssplat ) * cos( spiderlat );

        if ( azi == 0 && ( beta > M_PI_2 - ssplat ))
            spiderlong = ssplong + M_PI;
        else if ( azi == 180 && ( beta > M_PI_2 + ssplat ))
            spiderlong = ssplong + M_PI;
        else if ( fabs( num / dem) > 1.0 )
            spiderlong = ssplong;
        else {
            if (( 180 - azi ) >= 0)
                spiderlong = ssplong - arccos( num, dem );
            else
                spiderlong = ssplong + arccos( num, dem );
        }

        while ( spiderlong < 0.0 )
            spiderlong += TWOPI;

        while ( spiderlong > ( 2.0 * M_PI ))
            spiderlong -= TWOPI;

        spiderlat = spiderlat / deg2rad;
        spiderlong = spiderlong / deg2rad;

        /* Convert spiderweb data to map-based coordinates */
        if ( spiderlong <= 180.0 )
            xx = (int)( height - ( width * ( spiderlong / 360.0 )));
        else {
            spiderlong = spiderlong - 180.0;
            xx = (int)( width - ( width * ( spiderlong / 360.0 )));
        }

        yy = (int)( height / 2.0 - ( height / 2.0 * ( spiderlat / 90.0 )));
        sunPoints->setPoint( azi, xx, yy );
    }

    // The sun map coordinates
    if ( sun->getLon() <= 180.0)
        sun_x = (int)( height - (width * ( sun->getLon() / 360.0 )));
    else
        sun_x = (int)( width - ( width * ( (sun->getLon() - 180.0) / 360.0 )));

    sun_y = (int)( height / 2.0 - ( height / 2.0 * ( sun->getLat() / 90.0 )));

    // The moon map corrdinates
    if ( moon->getLon() <= 180.0)
        moon_x = (int)( height - (width * ( moon->getLon() / 360.0 )) );
    else moon_x = (int)( width - ( width * ( (moon->getLon() - 180.0) / 360.0 )));
   
   moon_y = (int)( height / 2.0 - ( height / 2.0 * ( moon->getLat() / 90.0 )));
}

