#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QMap>
#include <QVector>
#include <QObject>
#include <QString>
#include "types.h"

class Server : public QObject{
	Q_OBJECT
public:
	void startServer(int port);

private slots:
	void readData();
	void incomingConnection();
	void socketDisconnected();

private:
	template<class T>
	void writeData(QTcpSocket* client, qint8 num, T data);
	QDataStream in;
	QTcpServer* _tcpServer;
	QMap<int, QString> waitingPlayers;
	QVector<QPair<int, int>> 
};
