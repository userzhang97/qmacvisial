#include "frmRegister.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QSharedMemory>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//����ֻ��������һ��
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QSharedMemory memory("QMacVisual");
	if (!memory.create(1))
	{
		return 0;
	}	
	//���ĳ���������������ͼ��
	a.setWindowIcon(QIcon(":/Bitmaps/app.ico"));
	QApplication::setAttribute(Qt::AA_Use96Dpi);
	//Opengl��Ⱦ��ʽ
	QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QApplication::setAttribute(Qt::AA_UseOpenGLES);
	QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
	QFile fi(":/res/default.qss");
	fi.open(QIODevice::ReadOnly);
	QTextStream stream(&fi);
	a.setStyleSheet(stream.readAll());
	fi.close();
	frmRegister w;		
	return a.exec();
}
