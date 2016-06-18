/***************************************************************************
                          trxwidget.h  -  description
                             -------------------
    begin                : Sat May 18 2002
    copyright            : (C) 2002 by Luc Langehegermann
    email                : lx2gt@qsl.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TRXWIDGET_H
#define TRXWIDGET_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <qwidget.h>
#include "trxwidgetbase.h"
#include "rigctrl.h"
#include "dcopinterface.h"


/**Transceiver widget - displays frequencies and let you chose the transponder
  *@author Luc Langehegermann
  */

typedef struct
{
  long mtype;                   /* mtype should always be 88 */
  char mtext[1024];             /* mtext holds the message */
} msgtype;

class trxWidget : public trxWidgetBase, virtual public DCOPInterface {
   Q_OBJECT
public: 
	trxWidget(QWidget *parent=0, const char *name=0);
	~trxWidget();
  /** sets the radio control device we should use */
  void setDevice(rigctrl* dev);
private:
  rigctrl* trxctrl;
  satellite* sat;
  // these values are available for the DCOP interface
  double uplinkfrequency, downlinkfrequency;
  QString mode,satname;
  int msgid;
  msgtype msgbuf;
  transponder* currenttransponder;
  QString lastmsg;
public slots:
  void slotNewHardwareDownlinkFrequency(double freq);
  void slotNewHardwareUplinkFrequency(double freq);
private slots: // Private slots
  void slotNewUplinkFrequency(double freq);
  void slotNewDownlinkFrequency(double freq);
  void newTransponder(int id);
  void newCorrection(int c);
  void sendToXlog();
public:
  /** sets the satellite we use to get informations about */
  void setSatellite(satellite* s);
  /** call this, if the transponderlist changed */
  void updateTransponderList();
  // DCOP Interface functions
  double getUplinkFrequency();
  double getDownlinkFrequency();
  QString getMode();
  QString getSatellite();
private: // Private methods
  /** No descriptions */
  QString dottedFrequency(double freq);
};

#endif
