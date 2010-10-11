#include <QApplication>
#include <Mainwindow.h>
#include<qfile.h>
#include<qtextstream.h>

void debugOutput(QtMsgType type, const char *msg)
	{
	QFile logFile("c://data//TestAppLog.txt");
	Q_ASSERT(logFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ));
	QTextStream stream(&logFile);
	
	switch (type)
		{
		case QtDebugMsg:
			stream<<msg<<"\n";
			break;

		case QtWarningMsg:
			stream<<"Warning: ";
			stream<<msg<<"\n";
			break;
			
		case QtCriticalMsg:
			stream<<"Critical: ";
			stream<<msg<<"\n";
			break;
			
		case QtFatalMsg:
			stream<<"Fatal: ";
			stream<<msg<<"\n";
			break;
			
		default:;
		}
	}

int main(int argc,char *argv[])
{
qInstallMsgHandler(debugOutput);	
QApplication app(argc,argv);
MainWindow *mainWnd = new MainWindow();
mainWnd->showMaximized();
return app.exec();
}
