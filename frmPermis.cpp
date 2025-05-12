#include "frmPermis.h"
#include <QMessageBox>
#include "datavar.h"
using namespace std;

frmPermis::frmPermis(QWidget*parent)
	: BaseWindow(parent)
{
	ui.setupUi(this);	
	this->setWindowIcon(QIcon(":/Bitmaps/permis.png"));		
	//��ʼ��������
	initTitleBar();	
	//����Tab˳��
	ui.txtUserName->setFocus();
	setTabOrder(ui.txtUserName, ui.txtUserPW);
	setTabOrder(ui.txtUserPW, ui.btnAdd);
	setTabOrder(ui.btnAdd, ui.btnDelete);
	setTabOrder(ui.btnDelete, ui.btnQuery);	
}

frmPermis::~frmPermis()	
{
	this->deleteLater();
}

void frmPermis::initTitleBar()
{	
	m_titleBar->setBackgroundColor(3, 110, 95);
	m_titleBar->setStyleSheet("background-color: rgba(0, 0, 0,0);color:white;font-size:16px");
	m_titleBar->setTitleIcon(":/Bitmaps/permis.png");
	m_titleBar->setTitleContent("Ȩ�޹���");
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}

void frmPermis::paintEvent(QPaintEvent* event)
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

void frmPermis::on_btnAdd_clicked()
{	
	if (dataVar::login_state == 1)
	{		
		if (dataVar::bol_connect == true)
		{
			if (ui.txtUserName->text() == "" || ui.txtUserPW->text() == "")
			{
				emit dataVar::fProItemTab->sig_WarnClick();
				emit dataVar::fProItemTab->sig_Log("�û������û����벻��Ϊ�գ�");
				QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�û������û����벻��Ϊ�գ�");
				msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));
				msgBox.exec();				
			}
			else
			{
				//���ǰ�Ȳ�ѯ�Ƿ��Ѵ��ڸ��û�				
				QSqlQuery query;
				query.exec("SELECT * FROM LoginTable");
				while (query.next())
				{
					QString user_name = query.value(0).toString();	
					if (user_name == ui.txtUserName->text())
					{
						emit dataVar::fProItemTab->sig_WarnClick();
						emit dataVar::fProItemTab->sig_Log("�Ѵ��ڸ��û��޷���ӣ�");
						QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�Ѵ��ڸ��û��޷���ӣ�");
						msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));
						msgBox.exec();
						return;
					}					
				}
				bol_add = dataVar::sqlite.InsertData(dataVar::soft_key, "LoginTable", ui.txtUserName->text(), ui.txtUserPW->text());
				if (bol_add == true)
				{
					emit dataVar::fProItemTab->sig_InfoClick();
					emit dataVar::fProItemTab->sig_Log("�û���: " + ui.txtUserName->text() + "��ӳɹ���");
					QMessageBox msgBox(QMessageBox::Icon::NoIcon, "��ʾ", "�û�����ӳɹ���");
					msgBox.setWindowIcon(QIcon(":/res/ico/info.png"));
					msgBox.exec();					
				}
				else
				{
					emit dataVar::fProItemTab->sig_ErrorClick();
					emit dataVar::fProItemTab->sig_Log("�û���: " + ui.txtUserName->text() + "���ʧ�ܣ�");
					QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�û������ʧ�ܣ�");
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
	else
	{
		emit dataVar::fProItemTab->sig_WarnClick();
		emit dataVar::fProItemTab->sig_Log("���¼������ӣ�");
		QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "���¼������ӣ�");
		msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));
		msgBox.exec();		
	}
}

void frmPermis::on_btnAdd_pressed()
{
	ui.btnAdd->setStyleSheet("QPushButton{background-color: rgb(146, 217, 108);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmPermis::on_btnAdd_released()
{
	ui.btnAdd->setStyleSheet("QPushButton{background-color: rgb(85, 170, 127);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmPermis::on_btnDelete_clicked()
{
	if (dataVar::login_state == 1)
	{		
		if (dataVar::bol_connect == true)
		{
			if (ui.txtUserName->text() == "" || ui.txtUserPW->text() == "")
			{
				emit dataVar::fProItemTab->sig_WarnClick();
				emit dataVar::fProItemTab->sig_Log("�û������û����벻��Ϊ�գ�");
				QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�û������û����벻��Ϊ�գ�");
				msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));
				msgBox.exec();				
			}
			else
			{
				QString password;
				bol_query = dataVar::sqlite.QueryData(dataVar::soft_key, ui.txtUserName->text(), password);
				if (bol_query == true && password == ui.txtUserPW->text() && password != "")
				{
					if (ui.txtUserName->text() == "fz_lcl")
					{
						emit dataVar::fProItemTab->sig_ErrorClick();
						emit dataVar::fProItemTab->sig_Log("�޷�ɾ�����û���");
						QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�޷�ɾ�����û���");
						msgBox.setWindowIcon(QIcon(":/res/ico/error.png"));
						msgBox.exec();
					}
					else
					{
						bol_delete = dataVar::sqlite.DeleteData(dataVar::soft_key, ui.txtUserName->text());
						if (bol_delete == true)
						{
							emit dataVar::fProItemTab->sig_InfoClick();
							emit dataVar::fProItemTab->sig_Log("�û���: " + ui.txtUserName->text() + "ɾ���ɹ���");
							QMessageBox msgBox(QMessageBox::Icon::NoIcon, "��ʾ", "�û���ɾ���ɹ���");
							msgBox.setWindowIcon(QIcon(":/res/ico/info.png"));
							msgBox.exec();							
						}
						else
						{
							emit dataVar::fProItemTab->sig_ErrorClick();
							emit dataVar::fProItemTab->sig_Log("�û���ɾ��ʧ�ܣ�");
							QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "�û���ɾ��ʧ�ܣ�");
							msgBox.setWindowIcon(QIcon(":/res/ico/error.png"));
							msgBox.exec();						
						}
					}					
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
	else
	{
		emit dataVar::fProItemTab->sig_WarnClick();
		emit dataVar::fProItemTab->sig_Log("���¼����ɾ����");
		QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "���¼����ɾ����");
		msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));
		msgBox.exec();		
	}
}

void frmPermis::on_btnDelete_pressed()
{
	ui.btnDelete->setStyleSheet("QPushButton{background-color: rgb(255, 85, 185);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmPermis::on_btnDelete_released()
{
	ui.btnDelete->setStyleSheet("QPushButton{background-color: rgb(255, 85, 127);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmPermis::on_btnQuery_clicked()
{
	if (dataVar::admin_login_state == 1)
	{		
		ui.txtQuery->clear();		
		if (dataVar::bol_connect == true)
		{
			QSqlQuery query;
			query.exec("SELECT * FROM LoginTable");
			while (query.next())
			{
				QString user_name = query.value(0).toString();
				QString password = query.value(1).toString();
				ui.txtQuery->append("user: " + user_name + ", " + "pw: " + password);
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
	else
	{
		emit dataVar::fProItemTab->sig_WarnClick();
		emit dataVar::fProItemTab->sig_Log("��������ԱȨ�޲��ܲ�ѯ��");
		QMessageBox msgBox(QMessageBox::Icon::NoIcon, "����", "��������ԱȨ�޲��ܲ�ѯ��");
		msgBox.setWindowIcon(QIcon(":/res/ico/warn.png"));
		msgBox.exec();
	}	
}

void frmPermis::on_btnQuery_pressed()
{
	ui.btnQuery->setStyleSheet("QPushButton{background-color: rgb(146, 217, 108);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}

void frmPermis::on_btnQuery_released()
{
	ui.btnQuery->setStyleSheet("QPushButton{background-color: rgb(85, 170, 127);color: rgb(0, 0, 0);border: 2px groove gray; border-radius: 8px;}");
}
