/*
 * Server for hamlib's TCP rigctl protocol.
 * Copyright (C) 2010 Adam Sampson <ats@offog.org>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Modified for setFreq and SetMode by Glenn VE9GJ Jan 22 2010

#include <cstdio>
#include <hamlib/rig.h>

#include "main_widget.h"
#include "rigctl.h"
#include <string>
#include <vector>


RigCtlSocket::RigCtlSocket(QObject *parent, Main_Widget *main, QTcpSocket *conn)
	: QObject(parent),
	  main(main) {
	this->conn = conn;
}

void RigCtlSocket::disconnected() {
	deleteLater();
}


void RigCtlSocket::readyRead() {
	if (!conn->canReadLine()) {
		return;
	}

	QByteArray command(conn->readLine());
	command.chop(1);
	if (command.size() == 0) {
		command.append("?");
	}

        QString cmdstr = command.constData();
        QStringList cmdlist = cmdstr.split(QRegExp("\\s+"));
        int cmdlistcnt = cmdlist.count();
        bool output = false;
	int retcode = RIG_OK;
	QTextStream out(conn);

	/* This isn't a full implementation of the rigctl protocol; it's
	   essentially just enough to keep fldigi happy.  (And not very happy
	   at that; you will need to up the delays to stop it sending a
	   command, ignoring the reply owing to a timeout, then getting
	   confused the next time it sends a command because the old reply is
	   already in the buffer.)

	   Not implemented but used by fldigi:
             v           get_vfo  -fixed
             F 1.234     set_freq -fixed
	 */

	int space = command.indexOf(' ');
	if (command[0] == 'f') { // get_freq
		out << main->rigGetFrequency() << "\n";
		output = true;
        }else if(cmdlist[0].compare("F") == 0 && cmdlistcnt == 2) { // set_freq
                main->rigSetFrequency(atol(cmdlist[1].toAscii().data()));
	} else if (command[0] == 'm') { // get_mode
		out << rig_strrmode(main->rigGetMode()) << "\n";
		out << main->rigGetFilterWidth() << "\n";
		output = true;
	} else if (command[0] == 'v') { // get_vfo
		out << rig_strvfo(RIG_VFO_A) << "\n";
		output = true;
        } else if (command[0] == 'j') { // get_rit
                    out << "0" << "\n";
                    output = true;
        } else if (command[0] == 's') { // get_split_vfo
                            out << "0" << "\n";
                            out << "VFOA" << "\n";
                            output = true;
	} else if (command[0] == 'T') { // set_ptt
		int enabled = command.mid(space + 1).toInt();
		main->rigSetPTT(enabled);
	} else if (command[0] == 't') { // get PTT
		out << main->rigGetPTT() << "\n";
	} else if (command[0] == 'q') { // quit
		conn->close();
		return;
        } else if (cmdlist[0].compare("M") == 0 && cmdlistcnt == 3) { // set_mode
                if (cmdlist[1].compare("USB") == 0 ) {
                   main->setMode (  RIG_MODE_USB, FALSE, FALSE );
                } else if (cmdlist[1].compare("LSB") == 0) {
                   main->setMode (  RIG_MODE_LSB, FALSE, FALSE );
                } else if (cmdlist[1].compare("AM") == 0) {
                  main->setMode (  RIG_MODE_AM, FALSE, FALSE );
                } else if (cmdlist[1].compare("FM") == 0) {
                  main->setMode (  RIG_MODE_FM, FALSE, FALSE );
                } else if (cmdlist[1].compare("SAM") == 0) {
                  main->setMode (  RIG_MODE_SAM, FALSE, FALSE );
                } else if (cmdlist[1].compare("CW") == 0) {
                  main->setMode (  RIG_MODE_CW, FALSE, FALSE );
                } else if (cmdlist[1].compare("CWR") == 0) {
                  main->setMode (  RIG_MODE_CWR, FALSE, FALSE );
                }
                retcode =  RIG_OK;

        } else if (command == "\\dump_state" || command[0] == '1') {
		// See dump_state in rigctl_parse.c for what this means.
		out << "0\n"; // protocol version
		out << RIG_MODEL_NETRIGCTL << "\n";
		out << RIG_ITU_REGION2 << "\n";
                // Not sure exactly what to send here but this seems to work
                out << "150000.000000 30000000.000000  0x900af -1 -1 0x10 000003 0x3\n"; //("%"FREQFMT" %"FREQFMT" 0x%x %d %d 0x%x 0x%x\n",start,end,modes,low_power,high_power,vfo,ant)
                out << "0 0 0 0 0 0 0\n";
                out << "150000.000000 30000000.000000  0x900af -1 -1 0x10 000003 0x3\n";
		out << "0 0 0 0 0 0 0\n";
		out << "0 0\n";
		out << "0 0\n";
		out << "0\n";
		out << "0\n";
		out << "0\n";
		out << "0\n";
		out << "\n";
		out << "\n";
		out << "0x0\n";
		out << "0x0\n";
		out << "0x0\n";
		out << "0x0\n";
		out << "0x0\n";
		out << "0\n";
		output = true;
    } else if (command == "\\chk_vfo") {
        out << "CHKVFO 0\n";
	} else {
		fprintf(stderr, "rigctl: unknown command \"%s\"\n",
		        command.constData());
		retcode = -RIG_ENAVAIL;
	}
        //fprintf(stderr, "rigctl:  command \"%s\"\n", command.constData());
	if (!output) {
		out << "RPRT " << retcode << "\n";
	}
}

RigCtlServer::RigCtlServer(QObject *parent, Main_Widget *main)
	: QObject(parent),
	  main(main) {
	server = new QTcpServer(this);
//	if (!server->listen(QHostAddress::LocalHost, 19090)) {
	if (!server->listen(QHostAddress::Any, 19090)) {
		fprintf(stderr, "rigctl: failed to bind socket\n");
		return;
	}
	connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void RigCtlServer::newConnection() {
	QTcpSocket *conn = server->nextPendingConnection();
	RigCtlSocket *sock = new RigCtlSocket(this, main, conn);
	connect(conn, SIGNAL(disconnected()), conn, SLOT(deleteLater()));
	connect(conn, SIGNAL(disconnected()), sock, SLOT(disconnected()));
	connect(conn, SIGNAL(readyRead()), sock, SLOT(readyRead()));
}
