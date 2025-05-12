#include "frmAbout.h"
#include <QStyle>
#include <QPainter>
using namespace std;

frmAbout::frmAbout(QWidget*parent)
	: BaseWindow(parent)
{
	ui.setupUi(this);		
	this->setWindowIcon(QIcon(":/res/ico/about.png"));
	//��ʼ��������
	initTitleBar();		
}

frmAbout::~frmAbout()
{
	this->deleteLater();
}

void frmAbout::initTitleBar()
{	
	m_titleBar->setBackgroundColor(3, 110, 95);
	m_titleBar->setStyleSheet("background-color: rgba(0, 0, 0,0);color:white;font-size:17px");
	m_titleBar->setTitleIcon(":/res/ico/about.png");
	m_titleBar->setTitleContent("About");
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}

void frmAbout::paintEvent(QPaintEvent* event)
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
