#include "gps_server.h"
#include <cmath>
#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

#include <qhostaddress.h>


GpsServer::GpsServer()
{
	wsServer = new QWebSocketServer("WebSocket Server", QWebSocketServer::NonSecureMode, this);
	client = nullptr;

	lastPosition = { -1, 0, 0, 0 };
}


void GpsServer::start()
{
	if (wsServer->listen(QHostAddress::Any, 8080)) {
		qDebug() << "Connect handlers";
		connect(wsServer, &QWebSocketServer::newConnection, this, &GpsServer::onNewConnection);
	}
	if (wsServer->isListening()) {
		qDebug() << "Still Listening";
	}
}


void GpsServer::onNewConnection()
{
	qDebug() << "New Connection";
	client = wsServer->nextPendingConnection();

	connect(client, &QWebSocket::textMessageReceived, this, &GpsServer::onTextMessage);
	connect(client, &QWebSocket::binaryMessageReceived, this, &GpsServer::onBinaryMessage);
	connect(client, &QWebSocket::disconnected, this, &GpsServer::onDisconnect);

	lastPosition = { -1, 0, 0, 0 };
}


void GpsServer::onDisconnect()
{
	qDebug() << "Client disconnected\r\n";

	disconnect(client, &QWebSocket::textMessageReceived, this, &GpsServer::onTextMessage);
	disconnect(client, &QWebSocket::binaryMessageReceived, this, &GpsServer::onBinaryMessage);
	disconnect(client, &QWebSocket::disconnected, this, &GpsServer::onDisconnect);

	client->deleteLater();
	client = nullptr;

	lastPosition = { -1, 0, 0, 0 };
}


void GpsServer::onBinaryMessage()
{
	qDebug() << "binaryMessage";
}

void GpsServer::onTextMessage()
{
	qDebug() << "textMessage";
}


void GpsServer::onGpsData(qint64 millis, double lat, double lon, double height)
{
	GpsPositionInfo newPosition = { millis, lat, lon, height };
	double speed = 0;
	if (lastPosition.millis == -1)
	{
		lastPosition = newPosition;
	}
	else
	{
		double distance_km = haversine(lastPosition.lat, lastPosition.lon, lastPosition.height, newPosition.lat, newPosition.lon, newPosition.height);
		double dt_hours = (newPosition.millis - lastPosition.millis) / (1000.0 * 3600.0);

		speed = (distance_km / dt_hours);

		lastPosition = newPosition;
	}

	QString response = QString("%1,%2,%3,%4 : %5 KM/H").arg(millis).arg(lat).arg(lon).arg(height).arg(speed);
    qDebug() << response;
	if (client)
	{
		client->sendTextMessage(response);
	}
}


double GpsServer::haversine(double lat1, double lon1, double h1, double lat2, double lon2, double h2)
{
	// distance between latitudes
	// and longitudes
	double dLat = (lat2 - lat1) *
		M_PI / 180.0;
	double dLon = (lon2 - lon1) *
		M_PI / 180.0;

	// convert to radians
	lat1 = (lat1)*M_PI / 180.0;
	lat2 = (lat2)*M_PI / 180.0;

	// apply formulae
	double a = pow(sin(dLat / 2), 2) +
		pow(sin(dLon / 2), 2) *
		cos(lat1) * cos(lat2);
	double rad = 6371;
	double c = 2 * asin(sqrt(a));

	double llDistance_km = rad * c;
	double hDistance_km = (h2 - h1) / 1000.0;

	return llDistance_km + hDistance_km;
}
