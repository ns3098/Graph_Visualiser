

#ifndef SLEEP_H
#define SLEEP_H

#include <QCoreApplication>
#include <QObject>
#include <QTimer>

class SLEEP_HELPER: public QObject{
	Q_OBJECT
public:
	SLEEP_HELPER(){
		connect(&SleepTimer, SIGNAL(timeout()),
				this, SLOT(SleepEnd()));
	}

private:
	QTimer SleepTimer;		// Sleep timer
	bool SleepFlag;			// Sleep flag
	friend void sleep(int);

private slots:
	void SleepEnd(){SleepFlag = false;}

};

static SLEEP_HELPER SL_HELPER;

inline void sleep(int t){
	SL_HELPER.SleepFlag = true;
	SL_HELPER.SleepTimer.setInterval(t);
	SL_HELPER.SleepTimer.start();
	while(SL_HELPER.SleepFlag){
		QCoreApplication::processEvents();
	}
}

#endif // SLEEP_H
