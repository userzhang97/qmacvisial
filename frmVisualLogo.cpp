#include "frmVisualLogo.h"
#include "datavar.h"
#include "frmLog.h"
#include "mainwidget.h"
#include "frmToolsTree.h"
#include "cmysplashscreen.h"
#include <QPixmap>
#include <QTimer>
#include <QDesktopWidget>

frmVisualLogo::frmVisualLogo(QWidget *parent)
	: QWidget(parent)
{			
	connect(this, &frmVisualLogo::sig_FormStatues, this, &frmVisualLogo::slot_FormStatues, Qt::DirectConnection);	
	form_statues();
}

void frmVisualLogo::form_statues()
{
	SoftKey sk;
	dataVar::soft_key = sk.GetKey("MachineVision_LCL");	
	//����������ʾ������
	QPixmap pixmap(":/Bitmaps/pStart.png");
	pixmap.scaled(QApplication::desktop()->availableGeometry().size(), Qt::KeepAspectRatio);
	CMySplashScreen* splash = new CMySplashScreen(pixmap, 3000);
	splash->setDisabled(true); //�����û��������¼���Ӧ
	splash->show();		
	dataVar::app_state = true;
	emit sig_FormStatues();	
	dataVar::qtWidgetsSaveLoad = new QtWidgetsSaveLoad();	
	dataVar::fSystemSetUp = new frmSystemSetUp();
	dataVar::fImageLayout = new frmImageLayout();
	dataVar::fProcessSetUp = new frmProcessSetUp();
	dataVar::fGlobalVariable = new frmGlobalVariable();	
	dataVar::fInstrumentation = new frmInstrumentation();
	dataVar::fCameraSetUp = new frmCameraSetUp();	
	dataVar::fLog = new FrmLog();
	dataVar::fImageView = new FrmImageView();
	dataVar::fProItemTab = new frmProItemTab();		
	MainWidget *w = new MainWidget();
	dataVar::m_pWindow->slot_OpenProjectParam();  //��ʼ��ʱ����Ŀ
	w->setLogo(":/Bitmaps/pro.png", "QMacVisual");	
	w->setEnabelHeadMove(true);
	w->setLogDockWidget(dataVar::fLog);
	w->setCenterWidget(dataVar::fImageView);
	w->setToolDockWidget(new frmToolsTree);	
	w->setFlowDockWidget(dataVar::fProItemTab);	
	w->setWindowState(Qt::WindowMaximized);
    w->show();		
	dataVar::p_MainWidget = w;
	QTimer::singleShot(3000, w, SLOT(show()));			
	splash->finish(w);
	splash->deleteLater();	
	this->deleteLater();
}

void frmVisualLogo::slot_FormStatues()
{
	while (dataVar::app_state)
	{
		//��ͣ�Ĵ����¼����ó��򱣳���Ӧ
		QApplication::processEvents();
		QThread::msleep(1);
	}
}
