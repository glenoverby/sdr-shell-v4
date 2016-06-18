/*
 * Server for hamlib's TCP rigctl protocol.
 * Copyright (C) 2010 Adam Sampson <ats@offog.org>
 *
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

#ifndef RIGCTL_H
#define RIGCTL_H

#include <QtCore/QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class Main_Widget;

class RigCtlSocket : public QObject {
	Q_OBJECT

	public:
		RigCtlSocket(QObject *parent = 0, Main_Widget *main = 0,
		             QTcpSocket *conn = 0);

	public slots:
		void disconnected(void);
		void readyRead(void);

	private:
		Main_Widget *main;
		QTcpSocket *conn;
};

class RigCtlServer : public QObject {
	Q_OBJECT

	public:
		RigCtlServer(QObject *parent = 0, Main_Widget *main = 0);

	public slots:
		void newConnection(void);

	private:
		Main_Widget *main;
		QTcpServer *server;
};

#endif
