#include "gps.h"
#include "gps_server.h"

#include <iostream>
#include <qcoreapplication.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	QCoreApplication gpsBle(argc, argv);

	GPS gps;
	GpsServer server;
	if (gps.open("COM24", 100))
	{
		std::cout << "Opened serial port COM24" << std::endl;
		server.start();

		QObject::connect(&gps, SIGNAL(receivedGpsPos(qint64, double, double, double)), &server, SLOT(onGpsData(qint64, double, double, double)));
	}
	else
	{
		std::cout << "Cannot open serial port COM24" << std::endl;
	}




	return gpsBle.exec();
}