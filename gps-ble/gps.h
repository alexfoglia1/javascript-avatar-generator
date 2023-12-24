#ifndef GPS_H
#define GPS_H

#include <qobject.h>
#include <qtimer.h>
#include <qserialport.h>

class GPS : public QObject
{
	Q_OBJECT

public:
	GPS();
	bool open(QString txPortName, int txDelayMillis);

signals:
	void receivedGpsPos(qint64 millis, double lat, double lon, double height);

private:
	QSerialPort _serialPort;

	bool initSerialPort(QSerialPort& serialPort, const QString& serialPortName, QSerialPort::OpenMode openMode);

private slots:
	void onRxNmea();
};


#endif