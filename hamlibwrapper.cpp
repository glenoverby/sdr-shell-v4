/***************************************************************************
                          hamlibwrapper.cpp  -  description
                             -------------------
    begin                : 8/12/07
    copyright            : (C) 2007 by Rob Frohne, KL7NA with thanks to Luc Langehegermann
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

#include <string.h>
#include <stdlib.h>
#include <qapplication.h>
#include <math.h>

#include "hamlibwrapper.h"

hamlibWrapper::hamlibWrapper ( QObject *parent) : QThread ( parent )
{
	rRig=NULL;
	shouldexit = FALSE;
	useSlopeTuning = FALSE;
	rig_set_debug ( RIG_DEBUG_TRACE ); // RIG_DEBUG_NONE for none.
        connect ( parent, SIGNAL ( changeRigMode ( rmode_t, pbwidth_t ) ), this, SLOT ( setMode ( rmode_t, pbwidth_t ) ) );
        connect ( parent, SIGNAL ( changeSlopeTune ( bool ) ), this, SLOT ( pollSlopeTuning ( bool ) ) );
        connect ( parent, SIGNAL ( tellMuteXmit ( bool ) ), this, SLOT ( useMuteXmit ( bool ) ) );
}

hamlibWrapper::~hamlibWrapper()
{
        if ( isRunning() )
	{
		shouldexit=true;
		wait();
	}
	if ( rRig )
	{
		rig_close ( rRig );
		rig_cleanup ( rRig );
	}
}



int hamlibWrapper::init ( rig_model_t rig, const char* port, int speed )
{
	int retval=0;
	value_t pitch;

	mutex.lock();
	if ( rRig ) // Close the rig if it is already open.
	{
		rig_close ( rRig );
		rig_cleanup ( rRig );
	}
	// first init the first radio.
	// if we have no rig... simply init dummy rig! So, we do not have to care if we have an
	// rig or not!
	if ( rig ) rRig=rig_init ( rig );
	// set the parameters
	rRig->state.rigport.parm.serial.rate=speed;
	strcpy ( rRig->state.rigport.pathname, port );
	// open the rig
	if ( rig_open ( rRig ) != RIG_OK )
	{
		fprintf ( stderr, "Error opening radio!\n" );
		retval--;
		mutex.unlock();
		return retval;
	}
	if ( rig_get_vfo ( rRig, &vfo ) != RIG_OK )
	{
		fprintf ( stderr, "Error getting vfo from radio!\n" );
		retval--;
	}
	if ( rig_get_mode ( rRig, vfo, &mode, &width ) != RIG_OK )
	{
		fprintf ( stderr, "Error getting mode from radio!\n" );
		retval--;
	}
	if ( rig_get_level ( rRig, RIG_VFO_CURR, RIG_LEVEL_CWPITCH, &pitch ) != RIG_OK )
	{
		fprintf ( stderr, "Error getting mode from radio!\n" );
		retval--;
	}
	emit rigPitch ( pitch.i );

        if ( !isRunning() && rRig )  start();
	mutex.unlock();
	return retval;
}

void hamlibWrapper::setMode ( rmode_t new_mode, pbwidth_t new_width )
{
	newMode = new_mode;
	newWidth = new_width;
	setmodes=TRUE;
}

void hamlibWrapper::run()
{
	// these are static (args) but we can only have one instance of this!
	static freq_t oldfreq = 0, fr = 0;
	int retval;
	ptt_t oldtrstate = transmit;
	rmode_t oldmode;
	int oldSlopeLow = 0, oldSlopeHigh = 0;

	for ( ;; )
	{
		if ( shouldexit || !rRig )
		{
			shouldexit=false;
			return;
		}
		
		if ( muteOnXmit )
		{
			//mutex.lock();
			if ( rig_get_ptt ( rRig, vfo, &transmit ) != RIG_OK )
			{
				fprintf ( stderr, "Error getting vfo from radio!\n" );
				retval--;
			}
			//mutex.unlock();
			if ( oldtrstate != transmit )
			{
				//mutex.lock();
				emit nowTransmit ( ( int ) transmit );
				//mutex.unlock();
				oldtrstate = transmit;
			}
		}

		//mutex.lock(); //Check vfo and mode
		if ( rig_get_vfo ( rRig, &vfo ) != RIG_OK )
		{
			fprintf ( stderr, "Error getting vfo from radio!\n" );
			retval--;
		}
		if ( rig_get_mode ( rRig, vfo, &mode, &width ) != RIG_OK )
		{
			fprintf ( stderr, "Error getting vfo from radio!\n" );
			retval--;
		}
		//mutex.unlock();
		if ( oldmode != mode )
		{
			//mutex.lock();
			emit rigChangedMode ( mode, TRUE );
			//mutex.unlock();
			oldmode = mode;
		}
		fr=getHardwareFrequency();  //Check frequency
		if ( fr!=oldfreq )
		{
			oldfreq=fr;
			getfreq=fr;
			//mutex.lock();
			emit newFreq ( ( double ) getfreq );
			//mutex.unlock();
		}

		if ( useSlopeTuning ) //Check slope tuning
		{
			//mutex.lock();  //Check SlopeLow
			retval = rig_get_level ( rRig, RIG_VFO_CURR, RIG_LEVEL_SLOPE_LOW, &slopeLowVal );
			// Note you have to put RIG_VFO_CURR here to make this call work.  (vfo won't).
			if ( retval != RIG_OK )
			{
				fprintf ( stderr, "Error getting slope tune low from radio!\n" );
				retval--;
			}
			//mutex.unlock();
			slopeLow = slopeLowVal.i;
			if ( oldSlopeLow != slopeLow )
			{
				//mutex.lock();
				emit slopeLowChangedByRig ( slopeLow );
				//mutex.unlock();
				oldSlopeLow = slopeLow;
			}

			//mutex.lock();  //Check SlopeHigh
			if ( rig_get_level ( rRig, RIG_VFO_CURR, RIG_LEVEL_SLOPE_HIGH, &slopeHighVal ) != RIG_OK )
			{
				fprintf ( stderr, "Error getting slope tune high from radio!\n" );
				retval--;
			}
			slopeHigh = slopeHighVal.i;
			//mutex.unlock();
			if ( oldSlopeHigh != slopeHigh )
			{
				//mutex.lock();
				emit slopeHighChangedByRig ( slopeHigh );
				//mutex.unlock();
				oldSlopeHigh = slopeHigh;
			}
		}

		if ( setmodes )
		{
			setmodes=false;
			if ( rRig )
			{
				//mutex.lock();
				rig_set_mode ( rRig, vfo, newMode, newWidth );
				//mutex.unlock();
			}
		}
	}
}

void hamlibWrapper::setPause ( bool p )
{
	if ( rRig->caps->rig_model==RIG_MODEL_DUMMY )
		return;

	shouldexit=p;
        if ( !p && !isRunning() )
		start();
}



freq_t hamlibWrapper::getHardwareFrequency()
{
	const char* errorstring=NULL;
	int errorcode;
	freq_t frequency=0;
	//mutex.lock();
	errorcode=rig_get_freq ( rRig, vfo, &frequency );
	if ( errorcode!=RIG_OK )
	{
		fprintf ( stderr, "WARNING: Error fetching frequency from Radio! \n" );
		errorstring=rigerror ( errorcode );
		if ( errorstring )
			fprintf ( stderr, "The reported error was: %s\n", errorstring );
	}
	//mutex.unlock();
	return frequency;
}

void hamlibWrapper::pollSlopeTuning ( bool checktuning )
{
	useSlopeTuning = checktuning;
}

void hamlibWrapper::useMuteXmit ( bool muteonxmit )
{
	muteOnXmit = muteonxmit;
}
