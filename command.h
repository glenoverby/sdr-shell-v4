#ifndef COMMAND_H
#define COMMAND_H

#include <qlabel.h>
#include <qstring.h>
#include <QMouseEvent>
#include <dttsp.h>

class Command : public QLabel
{
    Q_OBJECT
    
    public:
        Command(QWidget *parent = 0 );

		void setCmd (DttSPcmd *, DttSPcmd *);
        void setID( int );
        void setName( QString );
        void setCommand( QString, QString );
        void setOnCommand( QString );
        void setOffCommand( QString );
        void setTargets( bool, bool );
        void setTargets( QString );
 
        int getID();
        QString getName();
		QString getOnCommand();
		QString getOffCommand();
		int getTargets();
		QString getTo();
		bool getToRX();
		bool getToTX();
		bool toggle();
		bool sendCmd(DttSPcmd *, QString *);

    private:
		DttSPcmd *rxCmd, *txCmd;
                bool state;
        int id;
        QString name;
		QString OnCommand;
		QString OffCommand;
        bool sendRx, sendTx;

    protected:
        void mouseReleaseEvent ( QMouseEvent * );

    signals:
        void mouseReleaseL ( Command * );
        void mouseReleaseR ( Command * );
};
#endif
