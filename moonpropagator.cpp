/* Copyright (C) 2002 Edson Pereira, PU1JTE, N1VTN, 7N4NCL
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, download a copy
 * from http://www.fsf.org
 *
 *
 * Propagator algorithm derived from John A. Magliacane,
 * KD2BD's Predict.
 *
 */

#include "moonpropagator.h"
#include <stdio.h>

MoonPropagator::MoonPropagator()
{

}

MoonPropagator::~MoonPropagator()
{

}

void MoonPropagator::setObserver( double lat, double lon )
{
    stnlat = lat;
    stnlon = lon;
}

void MoonPropagator::propagate( double Daynum )
{
   daynum = Daynum;
   gm=24.0*(daynum-floor(daynum));
   ew=fixAngle(1.134193+daynum*0.229971506);
   mm=fixAngle(1.319238+daynum*0.228027135);
   t1=fixAngle(6.217512+daynum*0.01720196977);
   t2=2.0*fixAngle(2.550677+daynum*0.212768711);
   t3=fixAngle(4.7652214+daynum*0.230895723);
   ew=ew+0.01148*sin(t2)+0.10976*sin(mm);
   ew=ew-0.022235*sin(mm-t2)-0.003246*sin(t1);
   ew=ew+0.003735*sin(2.0*mm)-0.0019897*sin(2.0*t3);
   ew=ew-0.0010297*sin(2.0*mm-t2)-0.0009948*sin(mm+t1-t2);
   en=t3+0.0115070*sin(t2)+0.10873924*sin(mm);
   en=en-0.0222006*sin(mm-t2);
   en=0.0897797*sin(en)-0.002548*sin(t3-t2);
   
   si=C1*sin(en)+S1*cos(en)*sin(ew);
   co=sqrt(1.0-(si*si));
   dc=atan2(si,co);
   si=sin(ew)*C1-tan(en)*S1;
   co=cos(ew);
   ra=atan2(si,co);
   
   if (ra<0.0)
       ra+=TP;
   
   t1=(double)(long)(daynum-39410.0);
   t2=floor((t1+32044.75)/36524.25);
   t2+=t1-t2/4.0+1486.0;
   yr=2084.0+floor((t2-122.1)/365.25);
   
   t1=yr-1.0;
   de=floor(365.25*(t1-1980.0))-floor(t1/100.0)+floor(t1/400.0)+381.0;
   t1=(de+29218.5)/36525.0;
   t1=6.6460656+t1*(2400.051262+t1*2.581e-5);
   se=t1-24.0*(yr-1900.0);
   t1=(se+0.0657098*(daynum-de)+gm*1.002738)/24.0;
   gs=24.0*(t1-floor(t1));
      
   t1=gs/24.0-ra/TP;
   gh=TP*(t1-floor(t1));
     
   n=stnlat*deg2rad;    /* North latitude of tracking station */
   e=stnlon*deg2rad;   /* East longitude of tracking station */
   
   lh=gh+e;
   
   si=cos(lh)*cos(dc)*cos(n)+sin(dc)*sin(n);
   co=sqrt(1.0-(si*si));
   el=atan2(si,co);
   
   si=-sin(lh)*cos(dc)*cos(n);
   co=sin(dc)-sin(n)*sin(el);
   az=atan2(si,co);
   
   if (az<0.0)
       az+=TP;
   
   ro=0.996986/(1.0+0.0549*cos(mm+0.10976*sin(mm)));
   el=el-0.0166*cos(el)/ro;

}

double MoonPropagator::fixAngle( double x )
{
   /* This function reduces angles greater than
    two pi by subtracting two pi from the angle */
   
   while (x>TP)
       x-=TP;
   
   return x;
}

double MoonPropagator::getAz()
{
   return az /deg2rad;
}

double MoonPropagator::getEl()
{
   return el / deg2rad;
}

double MoonPropagator::getLat()
{
   return dc / deg2rad;;
}

double MoonPropagator::getLon()
{
   return gh / deg2rad;;
}
