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

#ifndef MOONPROPAGATOR_H
#define MOONPROPAGATOR_H

#include <math.h>
#include <constants.h>

class MoonPropagator
{
public:
   MoonPropagator();
   ~MoonPropagator();
   
   void propagate( double daynum );
   double getAz();
   double getEl();
   double getLat();
   double getLon();
   void setObserver( double, double );
private:
   double t1, t2, t3, ew, en, e, n, si, co, ra, gh,
          lh, gm, de, se, gs, dc, el, az, mm, yr, ro, mr;
   double stnlat, stnlon;
   double daynum;
  
   double riseTime, setTime;
   
   double fixAngle( double x );
                            
};

#endif
