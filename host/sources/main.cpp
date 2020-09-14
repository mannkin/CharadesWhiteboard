#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QGuiApplication>
#include <QPluginLoader>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

#include "network_device.h"
#include "network_interface.h"

int main(int argc, char* argv[]) {
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::addLibraryPath(QDir::currentPath());
    QCoreApplication::addLibraryPath(QDir::currentPath() + "/Whiteboard");
    qDebug() << QCoreApplication::libraryPaths();
	QGuiApplication app(argc, argv);
	QCommandLineParser parser;

    qputenv("QT_OPENGL", "software");

	parser.addHelpOption();
	parser.addOptions({
		{{"c", "connect"}, "Connects to host specified by host and port switches"},
		{{"a", "host"}, "Sets destination host address.", "host address", "127.0.0.1"},
		{{"p", "port"}, "Sets destination port number.", "host port", "55555"},
	});

	parser.process(app);

	const bool connect = parser.isSet("connect");
	const QString ipAddress = parser.value("host");
	const QString portNumber = parser.value("port");

	qDebug() << "Creating network device...";

	NetworkDevice defaultNetworkDevice(QHostAddress::Any, portNumber.toInt());
	if (connect == true) {
		defaultNetworkDevice.connect(QHostAddress(ipAddress), portNumber.toInt());
	}

	QQmlApplicationEngine engine;

    engine.addImportPath("qrc:/");
    qDebug() << engine.importPathList();

	qDebug() << "Registering \"NetworkInterface\"...";
	if (qmlRegisterType<NetworkDeviceInterface>() < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"NetworkInterface\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	qDebug() << "Registering \"NetworkDevice\"...";
	if (qmlRegisterType<NetworkDevice>() < 0) {
		qFatal("FILE %s FUNCTION %s LINE %d\nERROR: Registering of \"NetworkDevice\" failed!", __FILE__, __PRETTY_FUNCTION__, __LINE__);
	}

	engine.rootContext()->setContextProperty("networkDevice", &defaultNetworkDevice);
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
