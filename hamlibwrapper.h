/***************************************************************************
                          hamlibwrapper.h  -  description
                             -------------------
    begin                : 8/12/2007
    copyright            : (C) 2007 by Rob Frohne
    email                : frohro@wallawalla.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef HAMLIBWRAPPER_H
#define HAMLIBWRAPPER_H

#include <hamlib/rig.h>
#include <qthread.h>
#include <qobject.h>
#include <qmutex.h>

#define SLOPE_TUNE_MAX 20;
#define SLOPE_TUNE_MAX_CW 20;

/** This is an simple wrapper class around hamlib to control our rigs for use with IF Softrock, etc.

  *@author Rob Frohne, with thanks to Luc Langehegermann for a look at his code for his ktrack hamlibwrapper.
  */

class hamlibWrapper : public QThread
{
        Q_OBJECT
	public:
                hamlibWrapper ( QObject *parent=0);
		~hamlibWrapper();
		int init ( rig_model_t, const char* port, int speed );
		void run();
		void setPause ( bool p );

	public slots:
		void setMode ( rmode_t, pbwidth_t );
		void pollSlopeTuning ( bool );
		void useMuteXmit ( bool );
		
	private:
		RIG* rRig;
		double freq, getfreq;
		bool shouldexit;
		ptt_t transmit;
		bool setmodes;
		QMutex mutex;
		freq_t getHardwareFrequency();
		rmode_t mode;
		rmode_t newMode;
		pbwidth_t width;
		pbwidth_t newWidth;
		value_t slopeLowVal;
		int slopeLow;
		value_t slopeHighVal;
		int slopeHigh;
		vfo_t vfo;
		bool useSlopeTuning;
		bool muteOnXmit;
		
	signals: // Signals
		void newFreq ( double );
		void nowTransmit ( int );
		void rigChangedMode ( rmode_t, bool );
		void slopeLowChangedByRig ( int );
		void slopeHighChangedByRig ( int );
		void rigPitch ( int );		
};

#endif
