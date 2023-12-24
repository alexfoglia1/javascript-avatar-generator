#ifndef GPS_SERVER_H
#define GPS_SERVER_H

#include <qobject.h>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

typedef struct
{
	qint64 millis;
	double lat;
	double lon;
	double height;
} GpsPositionInfo;

class GpsServer : public QObject
{
	Q_OBJECT
public:
	GpsServer();
	void start();
public slots:
	void onGpsData(qint64 millis, double lat, double lon, double height);

private:
	QWebSocketServer* wsServer;
	QWebSocket* client;
	GpsPositionInfo lastPosition;

    double haversine(double lat1, double lon1, double h1, double lat2, double lon2, double h2);

private slots:
	void onNewConnection();
	void onTextMessage();
	void onBinaryMessage();
	void onDisconnect();
};

#endif

