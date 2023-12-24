#include "gps.h"
#include <qdatetime.h>
#include <qdebug.h>


GPS::GPS()
{

}


bool GPS::open(QString txPortName, int txDelayMillis)
{
	bool success = initSerialPort(_serialPort, txPortName, QSerialPort::OpenModeFlag::ReadWrite);

	if (success)
	{
		connect(&_serialPort, SIGNAL(readyRead()), this, SLOT(onRxNmea()));

		return true;
	}
	else
	{
		return false;
	}
}


void GPS::onRxNmea()
{
	qint64 millis = QDateTime::currentMSecsSinceEpoch();
	QByteArray qba = _serialPort.readAll();

	if (qba.startsWith("$GPGGA"))
	{
		QString nmeaSentence(qba.toStdString().c_str());
		QStringList nmeaValues = nmeaSentence.split(",");
		
		QString ddmmsssssLat = nmeaValues[2];
		QString dddmmsssssLon = nmeaValues[4];

		double height = nmeaValues[9].toDouble();

		double degLat = QString("%1%2").arg(ddmmsssssLat[0]).arg(ddmmsssssLat[1]).toDouble();
		double minLat = QString("%1%2").arg(ddmmsssssLat[2]).arg(ddmmsssssLat[3]).toDouble() +
					    QString("0.%1%2%3%4%5").arg(ddmmsssssLat[5]).arg(ddmmsssssLat[6]).arg(ddmmsssssLat[7]).arg(ddmmsssssLat[8]).arg(ddmmsssssLat[9]).toDouble();

		double degLon = QString("%1%2%3").arg(dddmmsssssLon[0]).arg(dddmmsssssLon[1]).arg(dddmmsssssLon[2]).toDouble();
		double minLon = QString("%1%2").arg(dddmmsssssLon[3]).arg(dddmmsssssLon[4]).toDouble() +
			            QString("0.%1%2%3%4%5").arg(dddmmsssssLon[6]).arg(dddmmsssssLon[7]).arg(dddmmsssssLon[8]).arg(dddmmsssssLon[9]).arg(dddmmsssssLon[10]).toDouble();

		double lat = degLat + (minLat / 60.0);
		double lon = degLon + (minLon / 60.0);

		emit receivedGpsPos(millis, lat, lon, height);
	}
	else
	{
		qDebug() << "Unsupported NMEA sentence";
	}
}



bool GPS::initSerialPort(QSerialPort& serialPort, const QString& serialPortName, QSerialPort::OpenMode openMode)
{
	serialPort.setPortName(serialPortName);
	serialPort.setBaudRate(QSerialPort::Baud4800);
	serialPort.setParity(QSerialPort::NoParity);
	serialPort.setDataBits(QSerialPort::Data8);
	serialPort.setStopBits(QSerialPort::OneStop);
	serialPort.setFlowControl(QSerialPort::NoFlowControl);

	return serialPort.open(openMode);
}