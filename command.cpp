
#include <cstdio>
#include "command.h"

Command::Command(QWidget *parent) : QLabel(parent)
{
        state = FALSE;
        rxCmd = txCmd = NULL;

}

void Command::setCmd(DttSPcmd *rx, DttSPcmd *tx)
{
	rxCmd = rx;
	if(txCmd != NULL) txCmd = tx;
    else fprintf(stderr, "TX not enabled.\n");
}

void Command::setID(int v)
{
	id = v;
}

void Command::setName(QString n)
{
    // fprintf(stderr, "setName '%s'\n", qPrintable(n));
	name = n;
}
void Command::setTargets(bool r, bool t)
{
	sendRx = r;
	sendTx = t;
}
void Command::setCommand(QString On, QString Off)
{
	OnCommand = On;
	OffCommand = Off;
}
void Command::setOnCommand(QString On)
{
	//fprintf(stderr, "setONCommand: %s\n", qPrintable(On));
	OnCommand = On;
}
void Command::setOffCommand(QString Off)
{
	//fprintf(stderr, "setOffCommand: %s\n", qPrintable(Off));
	OffCommand = Off;
}
int Command::getID()
{
	return id;
}
void Command::setTargets(QString T)
{
	if(T.contains("RX", Qt::CaseInsensitive)) {
		printf("setTarget: RX\n");
		sendRx = TRUE;
	}
	if(T.contains("TX", Qt::CaseInsensitive)) {
		printf("setTarget: TX\n");
		sendTx = TRUE;
	}
}

QString Command::getOnCommand()
{
	return OnCommand;
}
QString Command::getOffCommand()
{
	return OffCommand;
}
QString Command::getName()
{
	return name;
}
QString Command::getTo()
{
	return tr(sendRx? "RX":"") + tr(sendTx? "TX":"");
}
bool Command::getToRX()
{
	return sendRx;
}
bool Command::getToTX()
{
	return sendTx;
}

void Command::mouseReleaseEvent ( QMouseEvent *e )
{
    if ( e->button() == Qt::LeftButton )
        emit mouseReleaseL( this );
    else if ( e->button() == Qt::RightButton )
        emit mouseReleaseR( this );
}

bool Command::sendCmd(DttSPcmd *p, QString *str)
{
	QStringList list;
	int n;
	char *s;
	bool error = FALSE;

    if (p == NULL)
        return 1;

	//printf("Send: '%s'\n", qPrintable(*str));
	list = str->split(tr(";"), QString::KeepEmptyParts);
	//printf("list is %d items\n", list.size());
	n = list.size();
	for(int i = 0; i < n; i++) {
		s = (char*)qPrintable(list.at(i));
		printf("item %d = '%s'\n", i, s);
		if (p->sendCommand (s))
			error = TRUE;
	}
	//p->sendCommand (qPrintable(*str));
	return error;
}

// Click on the command icon
// If there is one command to send, it should be in the "On" field.
// If there is no command in the "Off" field, then the icon is left plain.
bool Command::toggle()
{
	bool error = FALSE;
	QString *cmd;

	if (OffCommand.isEmpty())
		state = TRUE;
	else
		state = !state;

	fprintf(stderr, "Command::toggle %s\n", state ? "on" : "off");
	if (state) {
		cmd = &OnCommand;
		//txCmd->sendCommand (qPrintable(OnCommand));
		if (!OffCommand.isEmpty())
			this->setPalette(QPalette(QColor(200, 0, 0)));
	} else {
		cmd = &OffCommand;
		//txCmd->sendCommand (qPrintable(OffCommand));
		this->setPalette(QPalette(QColor(0, 0, 0)));
	}
	if (sendRx) {
		if (sendCmd(rxCmd, cmd)) {
			error = TRUE;
		}
	}
	if (sendTx) {
		if (sendCmd(txCmd, cmd)) {
			error = TRUE;
		}
	}
	return TRUE;
}

