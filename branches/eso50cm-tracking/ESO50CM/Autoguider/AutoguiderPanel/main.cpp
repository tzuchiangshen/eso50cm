#include "autoguiderpanel.h"
#include <QtWidgets/QApplication>
#include <QDebug>

#include "ImageProcessor.h"
//#include "Q_DebugStream.h"
#include <iostream>


void msgHandler( QtMsgType type, const char* msg )
{
    const char symbols[] = { 'I', 'E', '!', 'X' };
    QString output = QString("[%1] %2").arg( symbols[type] ).arg( msg );
    std::cout << output.toStdString() << std::endl;
    if( type == QtFatalMsg ) abort();
}

int main(int argc, char *argv[])
{
	//qInstallMsgHandler( msgHandler );
	qDebug() << "Starting Autoguider Panel";
	QApplication a(argc, argv);
	//Q_DebugStream cout(std::cout);
	//Q_DebugStream cerr(std::cerr);
	std::cout << "Probando redireccionamiento to qDebug" << std::endl;
	AutoguiderPanel w;
	w.show();
	return a.exec();
}
