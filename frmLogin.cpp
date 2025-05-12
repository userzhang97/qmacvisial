#include "frmLogin.h"
#include <QMessageBox>
#include <QTime>
#include "datavar.h"
using namespace std;

frmLogin::frmLogin(QWidget*parent)
	: BaseWindow(parent)
{
	ui.setupUi(this);	
	this->setWindowIcon(QIcon(":/res/ico/user.png"));	
	connect(this, &frmLogin::sendInStateSignal, dataVar::m_pWindow, &MainWindow::slot_LoginBtnState);
	connect(this, &frmLogin::sendOutStateSignal, dataVar::m_pWindow, &MainWindow::slot_LogoutBtnState);
	//��ʼ��������
	initTitleBar();	
	//����Tab˳��
	ui.txtUserName->setFocus();
	setTabOrder(ui.txtUserName, ui.txtUserPW);
	setTabOrder(ui.txtUserPW, ui.btnLogout);
	setTabOrder(ui.btnLogout, ui.btnLogin);	
	form_load();		
	ui.txtUserPW->setEchoMode(QLineEdit::Password);
}

frmLogin::~frmLogin()	
{
	this->deleteLater();
}

void frmLogin::initTitleBar()
{			
	m_titleBar->setBackgroundColor(3, 110, 95);
	m_titleBar->setStyleSheet("background-color: rgba(0, 0, 0,0);color:white;font-size:16px");
	m_titleBar->setTitleIcon(":/res/ico/user.png");
	m_titleBar->setTitleContent("�û���¼");
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}

void frmLogin::paintEvent(QPaintEvent* event)
{
	//���ñ���ɫ
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.fillPath(pathBack, QBrush(QColor("#bcc2bc")));
	return QWidget::paintEvent(event);
}

void frmLogin::form_load()
{	
	//SQLite���ݿ����	
	QFileInfo file(dataVar::path_DB + "MyDataBase.db");
	if (!file.exists())
	{		
		dataVar::bol_connect = dataVar::sqlite.CreateConnectDB(dataVar::soft_key, dataVar::path_DB + "MyDataBase.db", dataVar::database);
		if (dataVar::bol_connect == true)
		{			
			dataVar::sqlite.CreateTable(dataVar::soft_key, "LoginTable");
			dataVar::sqlite.InsertData(dataVar::soft_key, "LoginTable", "fz_lcl", "666666");
		}		
	}
	if (dataVar::login_state == 1)
	{
		ui.txtUserName->setText(dataVar::user_name);
	}	
}

void frmLogin::on_btnLogout_clicked()
{
	if (dataVar::login_state == 1)
	{			
		dataVar::startTime = QDateTime();
		emit sendOutStateSignal();
		dataVar::login_state = 0;		
		dataVar::admin_login_state = 0;
		emit dataVar::fProItemTab->sig_InfoClick();
		emit dataVar::fProItemTab->sig_Log("�û���: " + dataVar::user_name + "��ע����");
		dataVar::user_name = QString();
		QMessageBox msgBox(QMessageBox::Icon::NoIcon, "��ʾ", "��ע����");
		msgBox.setWindowIcon(QIcon(":/res/ico/info.png"));
		msgBox.exec();				
	}
	else
	{
		emit dataVar::fProItemTab->sig_WarnClick();
		emit dataVar::fProItemTab->sig_Log("���¼����ע����");
		QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "���¼����ע����");
		msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));		
		msgBox.exec();		
	}
}

void frmLogin::on_btnLogout_pressed()
{
	ui.btnLogout->setStyleSheet("QPushButton{background-color: rgb(255, 207, 130);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmLogin::on_btnLogout_released()
{
	ui.btnLogout->setStyleSheet("QPushButton{background-color: rgb(255, 163, 97);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmLogin::on_btnLogin_clicked()
{
	if (dataVar::login_state == 1)
	{
		emit dataVar::fProItemTab->sig_WarnClick();
		emit dataVar::fProItemTab->sig_Log("�û���: " + dataVar::user_name + "�ѵ�¼��");
		QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�û���: " + dataVar::user_name + "�ѵ�¼��");
		msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));
		msgBox.exec();
		return;
	}
	if (dataVar::bol_connect == false)
	{
		dataVar::bol_connect = dataVar::sqlite.CreateConnectDB(dataVar::soft_key, dataVar::path_DB + "/MyDataBase.db", dataVar::database);
	}	
	if (dataVar::bol_connect == true)
	{
		if (ui.txtUserName->text() == "" || ui.txtUserPW->text() == "")
		{
			emit dataVar::fProItemTab->sig_WarnClick();
			emit dataVar::fProItemTab->sig_Log("�û������û����벻��Ϊ�գ�");
			QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�û������û����벻��Ϊ�գ�");
			msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));			
			msgBox.exec();		
			return;
		}
		else
		{
			QString password;
			bol_query = dataVar::sqlite.QueryData(dataVar::soft_key, ui.txtUserName->text(), password);
			if (bol_query == true && password == ui.txtUserPW->text() && password != "")
			{					
				emit sendInStateSignal();
				dataVar::login_state = 1;
				if (ui.txtUserName->text() == "fz_lcl")
				{
					dataVar::admin_login_state = 1;
				}
				ui.txtUserPW->setText("");
				dataVar::user_name = ui.txtUserName->text();	
				dataVar::startTime = QDateTime::currentDateTime();
				emit dataVar::fProItemTab->sig_InfoClick();
				emit dataVar::fProItemTab->sig_Log("�û���: " + dataVar::user_name + "��¼�ɹ���");
				QMessageBox msgBox(QMessageBox::Icon::NoIcon, "��ʾ", "��¼�ɹ���");
				msgBox.setWindowIcon(QIcon(":/res/ico/info.png"));				
				msgBox.exec();			
			}
			else
			{
				emit dataVar::fProItemTab->sig_ErrorClick();
				emit dataVar::fProItemTab->sig_Log("�û������û��������");
				QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�û������û��������");
				msgBox.setWindowIcon(QIcon(":/res/ico/error.png"));				
				msgBox.exec();				
			}
		}		
	}
	else
	{
		emit dataVar::fProItemTab->sig_ErrorClick();
		emit dataVar::fProItemTab->sig_Log("���ݿ�����ʧ�ܣ�");
		QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "���ݿ�����ʧ�ܣ�");
		msgBox.setWindowIcon(QIcon(":/res/ico/error.png"));		
		msgBox.exec();		
	}
}

void frmLogin::on_btnLogin_pressed()
{
	ui.btnLogin->setStyleSheet("QPushButton{background-color: rgb(146, 217, 108);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmLogin::on_btnLogin_released()
{
	ui.btnLogin->setStyleSheet("QPushButton{background-color: rgb(85, 170, 127);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}
