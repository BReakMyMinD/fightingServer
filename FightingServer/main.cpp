#include <QtCore/QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	Server server;
	server.startServer(1234);

	return a.exec();
}
