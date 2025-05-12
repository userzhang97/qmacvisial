#include "headwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "frmQuit.h"
#include "datavar.h"

HeadWidget::HeadWidget(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground); 
    m_pLogo = new QLabel(this);
    m_pLogo->setObjectName("pLogo");
    m_pLogo->setFixedSize(24, 24);
    m_pName = new QLabel(this);
    m_pName->setObjectName("pName");
    m_pName->setStyleSheet("color:white; font-size:18px");
    m_pInfo = new QLabel(this);
    m_pInfo->setObjectName("pInfo");
    m_pInfo->setStyleSheet("color:white; font-size:18px");
    m_pInfo->setAlignment(Qt::AlignCenter);
    //��ť��ʽ
    QString btnStyle(
        "QWidget{"
        "border: none;"
        "}"
        "QPushButton:hover{"
        "background-color: rgba(243, 235, 197,45);"
        "}"
        "QPushButton:pressed{"
        "background-color: rgba(243, 235, 197,45);"
        "}"
    );
    auto *pBtnXXX = new QPushButton("", this);
    pBtnXXX->setIconSize(QSize(16, 16));
    pBtnXXX->setIcon(QIcon(":/Bitmaps/menu.png"));
    pBtnXXX->setToolTip("������ɫ");
    pBtnXXX->setStyleSheet(btnStyle);
    auto *pBtnMin = new QPushButton("", this);
    pBtnMin->setIconSize(QSize(16, 16));
    pBtnMin->setIcon(QIcon(":/Bitmaps/minimize.png"));
    pBtnMin->setToolTip("��С��");
    pBtnMin->setStyleSheet(btnStyle);
    pBtnMax = new QPushButton("", this);
    pBtnMax->setIconSize(QSize(16, 16));
    pBtnMax->setIcon(QIcon(":/Bitmaps/restore.png"));
    pBtnMax->setToolTip("���»�ԭ");
    pBtnMax->setStyleSheet(btnStyle);
    auto *pBtnCls = new QPushButton("", this);
    pBtnCls->setIconSize(QSize(16, 16));
    pBtnCls->setIcon(QIcon(":/Bitmaps/menu_close.png"));
    pBtnCls->setToolTip("�ر�");
    pBtnCls->setStyleSheet(btnStyle);
    QFont font;
    //����Ӵ�
    font.setBold(true);
    pBtnXXX->setFixedSize(36, 36);
    pBtnXXX->setFont(font);
    pBtnMin->setFixedSize(36, 36);
    pBtnMin->setFont(font);
    pBtnMax->setFixedSize(36, 36);
    pBtnMax->setFont(font);
    pBtnCls->setFixedSize(36, 36);
    pBtnCls->setFont(font);
    auto *hlay = new QHBoxLayout(this);
    hlay->setContentsMargins(3,0,4,2);
    hlay->setSpacing(2);
    hlay->addWidget(m_pLogo);
    hlay->addWidget(m_pName);
    hlay->addSpacing(20);
    hlay->addWidget(m_pInfo, 1);
    hlay->addSpacing(20);
    hlay->addWidget(pBtnXXX);
    hlay->addWidget(pBtnMin);
    hlay->addWidget(pBtnMax);
    hlay->addWidget(pBtnCls);
    connect(pBtnMin, &QPushButton::clicked, this, &HeadWidget::sigMinimized);
    connect(pBtnMax, &QPushButton::clicked, this, &HeadWidget::sigToggleMaximized);
    connect(pBtnMax, &QPushButton::clicked, this, &HeadWidget::min_maxState);
    connect(pBtnCls, &QPushButton::clicked, this, &HeadWidget::sigClose);
    connect(pBtnXXX, &QPushButton::clicked, this, [this, pBtnXXX](){
        auto pos = this->mapToGlobal(pBtnXXX->pos());
        pos += QPoint(0, pBtnXXX->height() + 2);
        emit sigColor(pos);
    });
}

void HeadWidget::shot_Quit()
{   
    //���滭��
    frmQuit* fQuit = new frmQuit();
    fQuit->showNormal();
}

void HeadWidget::min_maxState()
{
    if (dataVar::form_max_state == false)
    {
        pBtnMax->setIcon(QIcon(":/Bitmaps/restore.png"));
        pBtnMax->setToolTip("���»�ԭ");        
        dataVar::form_max_state = true;
    }
    else
    {
        pBtnMax->setIcon(QIcon(":/Bitmaps/maximize.png"));
        pBtnMax->setToolTip("���");       
        dataVar::form_max_state = false;
    }
}

void HeadWidget::setLogo(const QString &imgFile, const QString &name)
{
    m_pLogo->clear();
    if(!imgFile.isEmpty())
    {
        QPixmap pic(imgFile);
        if (!pic.isNull())
        {           
            m_pLogo->setPixmap(pic);
        }           
    }
    m_pName->setText(name);
    m_pName->adjustSize();
}

void HeadWidget::setTitle(const QString &info)
{
    m_pInfo->setText(info);
}

void HeadWidget::setBgColor(const QString &color)
{
    this->setStyleSheet(QString("HeadWidget{background:%1}").arg(color));
}

void HeadWidget::mousePressEvent(QMouseEvent *e)
{
    e->accept();
    m_bPressed = true;
    m_pressedWidgetPos = this->parentWidget()->pos();
    m_pressedMousePos  = e->globalPos();     
}

void HeadWidget::mouseMoveEvent(QMouseEvent *e)
{
    e->accept();
    if(m_bEnabelMove && m_bPressed)
    {
        QPoint pos = m_pressedWidgetPos + e->globalPos() - m_pressedMousePos;
        //�˴��õ�parent widget���ƶ�
        //���������Ƕ��, ��Ҫ�Լ�д������, ����Ҫ�ƶ���widget���ݽ����滻������,
        //mousePressEvent����Ҳ��Ҫ���ж�Ӧ���滻
        if (pos.x() != 0 || pos.y() != 0)
        {
            this->parentWidget()->move(pos);
            m_bMouseIsMove = true;
        }      
    }
}

void HeadWidget::mouseReleaseEvent(QMouseEvent *e)
{
    e->accept();
    m_bPressed = false;    
    if (dataVar::form_max_state == true && m_bMouseIsMove == true)
    {              
        //���������źŵ�Normal״̬
        emit sigToggleMaximized();        
        emit sigToggleMaximized();    
        min_maxState();
    }
    m_bMouseIsMove = false;
}
