#ifndef IDBUTTON_H
#define IDBUTTON_H

#include <QLabel>
#include <QPushButton>

class IdPushButton : public QPushButton
{
    Q_OBJECT
    
    public:
		IdPushButton(QWidget *parent = 0);
		//IdPushButton(QPushButton *parent = 0);
        void setID( int );
        int getID();

    private:
        int id;

	public slots:
		void nclick ();

    signals:
		void selected ( int );

};
#endif
