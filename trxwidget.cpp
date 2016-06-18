/***************************************************************************
                          trxwidget.cpp  -  description
                             -------------------
    begin                : Sat May 18 2002
    copyright            : (C) 2002 by Luc Langehegermann
    email                : lx2gt@qmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <errno.h>

#include <qlcdnumber.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qtimer.h>
#include <qlabel.h>
#include <kapplication.h>
#include <stdio.h>


#include "globals.h"
#include "trxwidget.h"

trxWidget::trxWidget(QWidget *parent, const char *name ) : trxWidgetBase(parent,name), DCOPObject("DCOPInterface") {
  uplinkLCD->setSmallDecimalPoint(true);
  downlinkLCD->setSmallDecimalPoint(true);
  QObject::connect(correctionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(newCorrection(int)));
  sat=NULL;
  currenttransponder=NULL;
  // init the message queue to xlog

  /* create the message queue, you need to use 1238 for the key to talk to xlog */
  msgid = msgget ((key_t) 1238, 0666 | IPC_CREAT);
  if (msgid == -1)
    {
      fprintf (stderr, "msgget failed: %s\n", strerror (errno));
    }
  /* you need to set mtype to 88 */
  msgbuf.mtype = 88;

  mode="";
  downlinkfrequency=0;
  uplinkfrequency=0;

  QTimer* timer = new QTimer (this);
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(sendToXlog()));
  timer->start(1000);
}
trxWidget::~trxWidget(){
}
/** sets the radio control device we should use */
void trxWidget::setDevice(rigctrl* dev){
  trxctrl=dev;
  if (trxctrl) {
    QObject::connect(trxctrl, SIGNAL(newDownlinkFreq(double)), this,
      SLOT(slotNewDownlinkFrequency(double)));
    QObject::connect(trxctrl, SIGNAL(newUplinkFreq(double)), this,
      SLOT(slotNewUplinkFrequency(double)));
    QObject::connect(trxctrl->Wrapper(), SIGNAL(newHardwareDownlinkFreq(double)), this,
      SLOT(slotNewHardwareDownlinkFrequency(double)));
    QObject::connect(trxctrl->Wrapper(), SIGNAL(newHardwareUplinkFreq(double)), this,
      SLOT(slotNewHardwareUplinkFrequency(double)));
  }
}
/** No descriptions */
void trxWidget::slotNewDownlinkFrequency(double freq){
  downlinkLCD->display(dottedFrequency(freq));
  downlinkfrequency=freq/1000.0;
}
/** No descriptions */
void trxWidget::slotNewUplinkFrequency(double freq){
  uplinkLCD->display(dottedFrequency(freq));
  uplinkfrequency=freq/1000.0;
}

void trxWidget::slotNewHardwareDownlinkFrequency(double freq) {
  downlinkHardwareFrequencyLabel->setText(dottedFrequency(freq));
}
  
void trxWidget::slotNewHardwareUplinkFrequency(double freq) {
  uplinkHardwareFrequencyLabel->setText(dottedFrequency(freq));
}


void trxWidget::setSatellite(satellite* s) {
  sat=s;
  // fill in the combo
  QString str;
  transponder* t;
  QList<transponder> list=*sat->translist();
  transponderCombo->clear();
  for (t=list.first(); t!=0; t=list.next()) {
    str = QString::number(t->uplink()/1000.0,'f',0) + "/" + QString::number(t->downlink()/1000.0,'f',0);
    transponderCombo->insertItem(str);
  }
  QObject::connect(transponderCombo, SIGNAL(activated(int)), this, SLOT(newTransponder(int)));
  newTransponder(0);
}

void trxWidget::updateTransponderList() {
  // for now: do as if we had a new satellite - but with the old one
  if (sat)
    setSatellite(sat);
}

/** No descriptions */
void trxWidget::newTransponder(int id){
  QList<transponder> list = *sat->translist();
  currenttransponder=list.at(id);
  if (!currenttransponder) return;
  if(trxctrl) {
    trxctrl->setTransponder(currenttransponder);
    trxctrl->setCorrection(currenttransponder->getLastOffset());
  }
  if (currenttransponder->mode() == MODE_SSB)
    mode="SSB";
  if (currenttransponder->mode() == MODE_FM)
    mode="FM";
  if (currenttransponder->mode() == MODE_CW)
    mode="CW";
//  currenttransponder->setLastOffset(correctionSpinBox->value());
  correctionSpinBox->setValue(currenttransponder->getLastOffset());
  satname=sat->name();
}
/** No descriptions */
void trxWidget::newCorrection(int c){
  if(trxctrl)
    trxctrl->setCorrection(c);
  if (currenttransponder)
    currenttransponder->setLastOffset(c);
}

double trxWidget::getUplinkFrequency() {
  return uplinkfrequency;
}

double trxWidget::getDownlinkFrequency() {
  return downlinkfrequency;
}

QString trxWidget::getMode() {
  return mode;
}

QString trxWidget::getSatellite() {
  if (sat)
    return sat->name();
  else
    return "";
}

/** No descriptions */
void trxWidget::sendToXlog(){
  QString xlogstr;
  xlogstr="program:ktrack\1version:1\1mhz:";
  xlogstr+=QString::number(downlinkfrequency/1000.0, 'f', 3);
  xlogstr+="\1free1:" + satname; 
  xlogstr+="\1free2:" + QString::number(uplinkfrequency/1000.0, 'f', 3);
  xlogstr+="\1mode:" + mode + "\1";
  strcpy (msgbuf.mtext, xlogstr.latin1());
  msgsnd (msgid, (void *) &msgbuf, 1024, IPC_NOWAIT);
  lastmsg = xlogstr;
}
/** No descriptions */
QString trxWidget::dottedFrequency(double freq){
  QString str=QString::number(freq,'f',0);
  str.insert(str.length()-3, '.');
  str.insert(str.length()-7, '.');
  if(freq>999999999.0)
    str.insert(str.length()-11, '.');
  return str;
}
