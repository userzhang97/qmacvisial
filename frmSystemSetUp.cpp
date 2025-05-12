#include "frmSystemSetUp.h"
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include "datavar.h"
using namespace std;

frmSystemSetUp::frmSystemSetUp(QWidget*parent)
	: BaseWindow(parent)
{
	ui.setupUi(this);	
	this->setWindowIcon(QIcon(":/res/ico/set.png"));	
	connect(ui.checkProgramSelfStart, SIGNAL(toggled(bool)), this, SLOT(slot_ProgramSelfStart(bool)));
	connect(ui.checkProcessSelfStart, SIGNAL(toggled(bool)), this, SLOT(slot_ProcessSelfStart(bool)));
	connect(ui.checkSignOutTime, SIGNAL(toggled(bool)), this, SLOT(slot_SignOutTime(bool)));
	//��ʼ��������
	initTitleBar();		
}

frmSystemSetUp::~frmSystemSetUp()	
{
	this->deleteLater();
}

void frmSystemSetUp::initTitleBar()
{	
	m_titleBar->setBackgroundColor(3, 110, 95);
	m_titleBar->setStyleSheet("background-color: rgba(0, 0, 0,0);color:white;font-size:16px");
	m_titleBar->setTitleIcon(":/res/ico/set.png");
	m_titleBar->setTitleContent("ϵͳ����");
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}

void frmSystemSetUp::paintEvent(QPaintEvent* event)
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

//���ó���������appPath����·��
void frmSystemSetUp::SetProcessAutoRunSelf(const QString& appPath)
{
	//ע���·����Ҫʹ��˫��б�ܣ������32λϵͳ��Ҫʹ��QSettings::Registry32Format
	QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::Registry64Format);
	//�Գ���������Ϊע����еļ������ݼ���ȡ��Ӧ��ֵ(����·��)
	QFileInfo fInfo(appPath);
	QString name = fInfo.baseName();
	QString path = settings.value(name).toString();
	QString newPath = QDir::toNativeSeparators(appPath);
	if (path != newPath)
	{
		settings.setValue(name, newPath);
	}
}

//�Ƴ�����������appPath����·��
void frmSystemSetUp::RemoveProcessAutoRunSelf(const QString& appPath)
{
	//ע���·����Ҫʹ��˫��б�ܣ������32λϵͳ��Ҫʹ��QSettings::Registry32Format
	QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::Registry64Format);
	//�Գ���������Ϊע����еļ������ݼ���ȡ��Ӧ��ֵ(����·��)
	QFileInfo fInfo(appPath);
	QString name = fInfo.baseName();
	settings.remove(name);
}

void frmSystemSetUp::slot_ProgramSelfStart(bool state)
{
	if (state == true)
	{
		SetProcessAutoRunSelf(qApp->applicationFilePath());
	}
	else
	{
		RemoveProcessAutoRunSelf(qApp->applicationFilePath());
	}
}

void frmSystemSetUp::slot_ProcessSelfStart(bool state)
{
	if (state == true)
	{
		dataVar::process_self_start = true;
	}
	else
	{
		dataVar::process_self_start = false;
	}
}

void frmSystemSetUp::slot_SignOutTime(bool state)
{
	if (state == true)
	{
		dataVar::auto_login_out = true;
	}
	else
	{
		dataVar::auto_login_out = false;
	}
}
