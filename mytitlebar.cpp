#include "mytitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QDebug>

#define BUTTON_HEIGHT 30	//��ť�߶�
#define BUTTON_WIDTH 30		//��ť���
#define TITLE_HEIGHT 35		//�������߶�

MyTitleBar::MyTitleBar(QWidget *parent)
	: QWidget(parent)
	, m_colorR(120)
	, m_colorG(120)
	, m_colorB(120)
	, m_isPressed(false)
	, m_buttonType(MIN_MAX_BUTTON)
{
	//��ʼ��
	initControl();
	initConnections();
	loadStyleSheet("MyTitle");
}

MyTitleBar::~MyTitleBar()
{
	this->deleteLater();
}

//��ʼ���ؼ�
void MyTitleBar::initControl()
{
	//��ť��ʽ
	QString btnStyle(
		"QWidget{"
		"border: none;background-color: rgba(0, 0, 0, 0);"
		"}"
		"QToolTip{"
		"border: none;background-color: rgb(235, 235, 255);color: rgb(45, 45, 45);font-size:15px;"
		"}"
		"QPushButton:hover{"
		"background-color: rgba(255, 0, 0, 15);"
		"}"
		"QPushButton:pressed{"
		"background-color: rgba(255, 0, 0, 45);"
		"}"		
	);
	m_pIcon = new QLabel(this);
	m_pIcon->setObjectName("IconContent");
	m_pTitleContent = new QLabel(this);
	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");	
	m_pButtonClose->setIconSize(QSize(30, 30));
	m_pButtonClose->setIcon(QIcon(":/Bitmaps/close_2.png"));
	m_pButtonClose->setToolTip("�ر�");
	m_pButtonClose->setStyleSheet(btnStyle);
	QHBoxLayout* mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);
	mylayout->setContentsMargins(5, 0, 0, 0);
	mylayout->setSpacing(3);
	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setFixedHeight(TITLE_HEIGHT);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

//�źŲ۵İ�
void MyTitleBar::initConnections()
{
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

//���ñ���������ɫ,��paintEvent�¼��н��л��Ʊ���������ɫ
//�ڹ��캯���и���Ĭ��ֵ�������ⲿ������ɫֵ�ı����������ɫ
void MyTitleBar::setBackgroundColor(int r, int g, int b)
{
	m_colorR = r;
	m_colorG = g;
	m_colorB = b;
	//���»���(����paintEvent�¼�)
	update();
}

//���ñ�����ͼ��
void MyTitleBar::setTitleIcon(QString filePath)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setPixmap(titleIcon.scaled(32, 32));
}

//���ñ�������
void MyTitleBar::setTitleContent(QString titleContent)
{
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

//���ñ���������
void MyTitleBar::setTitleWidth(int width)
{
	this->setFixedWidth(width);
}

//���ñ������ϰ�ť����
//���ڲ�ͬ���ڱ������ϵİ�ť����һ�������Կ����Զ���������еİ�ť
//�����ṩ���ĸ���ť���ֱ�Ϊ��С������ԭ����󻯡��رհ�ť�������Ҫ������ť�������������
void MyTitleBar::setButtonType(ButtonType buttonType)
{
	m_buttonType = buttonType;
	switch (buttonType)
	{
	case MIN_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
	case MIN_MAX_BUTTON:
		{
			m_pButtonRestore->setVisible(false);
		}
		break;
	case ONLY_CLOSE_BUTTON:
		{
			m_pButtonMin->setVisible(false);
			m_pButtonRestore->setVisible(false);
			m_pButtonMax->setVisible(false);
		}
		break;
	default:
		break;
	}
}

//���ñ������еı����Ƿ���Զ�����������Ƶ�Ч��
void MyTitleBar::setTitleRoll()
{
	connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
	m_titleRollTimer.start(200);
}

//���洰�����ǰ���ڵ�λ���Լ���С
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

//��ȡ�������ǰ���ڵ�λ���Լ���С
void MyTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}

//���Ʊ���������ɫ
void MyTitleBar::paintEvent(QPaintEvent *event)
{
	//���ñ���ɫ
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 0, 0);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB, 200)));
	//��������󻯻��߻�ԭ�󣬴��ڳ��ȱ��ˣ��������ĳ���Ӧ��һ��ı�
	if (this->width() != this->parentWidget()->width())
	{
		this->setFixedWidth(this->parentWidget()->width());
	}
	QWidget::paintEvent(event);
}

//˫����Ӧ�¼�����Ҫ��ʵ��˫��������������󻯺���С������
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	//ֻ�д�����󻯡���ԭ��ťʱ˫������Ч
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		//ͨ����󻯰�ť��״̬�жϵ�ǰ�����Ǵ�����󻯻���ԭʼ��С״̬
		//����ͨ���������ñ�������ʾ��ǰ����״̬
		if (m_pButtonMax->isVisible())
		{
			onButtonMaxClicked();
		}
		else
		{
			onButtonRestoreClicked();
		}
	}	
	return QWidget::mouseDoubleClickEvent(event);
}

//����ͨ��mousePressEvent��mouseMoveEvent��mouseReleaseEvent�����¼�ʵ��������϶��������ƶ����ڵ�Ч��
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		//�ڴ������ʱ��ֹ�϶�����
		if (m_pButtonMax->isVisible())
		{
			m_isPressed = true;
			m_startMovePos = event->globalPos();
		}
	}
	else
	{
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}	
	return QWidget::mousePressEvent(event);
}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
	if (m_isPressed)
	{
		QPoint movePoint = event->globalPos() - m_startMovePos;
		QPoint widgetPos = this->parentWidget()->pos();
		m_startMovePos = event->globalPos();
		this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
	}
	return QWidget::mouseMoveEvent(event);
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//���ر�����ʽ�ļ�
//���Խ���ʽֱ��д���ļ��У���������ʱֱ�Ӽ��ؽ���
void MyTitleBar::loadStyleSheet(const QString &sheetName)
{
	QFile file(":/Resource Files/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
}

//����Ϊ��ť������Ӧ�Ĳ�
void MyTitleBar::onButtonMinClicked()
{
	emit signalButtonMinClicked();
}

void MyTitleBar::onButtonRestoreClicked()
{
	m_pButtonRestore->setVisible(false);
 	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

void MyTitleBar::onButtonMaxClicked()
{
 	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
	emit signalButtonCloseClicked();
}

//�÷�����Ҫ���ñ������еı�����ʾΪ������Ч��
void MyTitleBar::onRollTitle()
{
	static int nPos = 0;
	QString titleContent = m_titleContent;
	//����ȡ��λ�ñ��ַ�����ʱ����ͷ��ʼ
	if (nPos > titleContent.length())
		nPos = 0;
	m_pTitleContent->setText(titleContent.mid(nPos));
	nPos++;
}
