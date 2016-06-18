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

#include <stdio.h>
#include "sunpropagator.h"

SunPropagator::SunPropagator()
{
   ssplat = 0;
   ssplon = 0;
}

SunPropagator::~SunPropagator()
{
   
}

void SunPropagator::setObserver( double lat, double lon )
{
    stnlat = lat;
    stnlon = lon;
}

void SunPropagator::propagate( double Daynum )
{
   daynum = Daynum;
   /* Calculate the year from "daynum" */
   
   t1=(double)(long)(daynum-39410.0);
   t2=floor((t1+32044.75)/36524.25);
   t2+=t1-t2/4.0+1486.0;
   yr=2084.0+floor((t2-122.1)/365.25);
   
   gm=24.0*(daynum-floor(daynum));
   c=((daynum+gm/24.0)/365.2422-0.010452395);
   ms=TP*(c-floor(c));
   t1=ms;
   t2=0.016718;
   
   do
   {
      
      t3=t1-t2*sin(t1)-ms;
      t1=t1-t3/(1.0-t2*cos(t1));
   }
   
   while (fabs(t3)>1.0e-6);
   
   t1=1.01686*tan(t1/2.0);
   ew=2.0*atan(t1)+4.932237686;
   si=S1*sin(ew);
   co=sqrt(1.0-(si*si));
   dc=atan2(si,co);
   si=sin(ew)*C1;
   co=cos(ew);
   ra=atan2(si,co);

   if (ra<0.0)
       ra+=TP;
   
   t1=yr-1.0;
   de=floor(365.25*(t1-1980.0))-floor(t1/100.0)+floor(t1/400.0)+381.0;
   t1=(de+29218.5)/36525.0;
   t1=6.6460656+t1*(2400.051262+t1*2.581e-5);
   se=t1-24.0*(yr-1900.0);
   t1=(se+0.0657098*(daynum-de)+gm*1.002738)/24.0;
   gs=24.0*(t1-floor(t1));
   t1=gs/24.0-ra/TP;
   gh=TP*(t1-floor(t1));
   
   lat[0] = stnlat;
   lon[0] = -stnlon;
   
   lat[1] = dc;
   lon[1] = gh;
   
   for (int x = 0; x < 2; x++)
   {
      
      n = lat[x] * deg2rad;  /* North latitude of tracking position */
      e = -lon[x] * deg2rad; /* East longitude of tracking position */
      
      lh = gh + e;
      si = cos(lh) * cos(dc) * cos(n) + sin(dc) * sin(n);
      co = sqrt(1.0 - (si * si));
      el[x] = atan2( si, co );
      
      si = -sin(lh) * cos(dc) * cos(n);
      co = sin(dc) -sin(n) * sin(el[x]);
      az[x] = atan2( si, co );
      
      if (az[x]<0.0)
	  az[x]+=TP;
   }
}

double SunPropagator::getAz()
{
   return  az[0] / deg2rad;
}                                                                               

double SunPropagator::getEl()
{
   return  el[0] / deg2rad;
}
                                                                               
double SunPropagator::getLat()
{
   return  lat[1] / deg2rad;
}
                                                                               
double SunPropagator::getLon()
{
   return  lon[1] / deg2rad;
}

