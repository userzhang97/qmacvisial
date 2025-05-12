#include "mainwidget.h"
#include <QVBoxLayout>
#include <QtDebug>
#include <QMenu>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QApplication>
#include "headwidget.h"
#include "frmLogin.h"
#include "frmPermis.h"
#include "frmAbout.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{    
    this->resize(1360, 768);
    this->setWindowFlags(this->windowFlags() |
                         Qt::FramelessWindowHint |
                         Qt::WindowSystemMenuHint);       
    m_pHeadWidget = new HeadWidget(this);
    m_pHeadWidget->setFixedHeight(36);
    dataVar::m_pWindow = new MainWindow(this);
    dataVar::m_pWindow->setVisible(true);
    auto *vlay = new QVBoxLayout(this);
    vlay->setSpacing(0);
    vlay->setContentsMargins(0,0,0,0);
    vlay->addWidget(m_pHeadWidget);
    vlay->addWidget(dataVar::m_pWindow, 1);
    connect(dataVar::fProItemTab, &frmProItemTab::sig_AllCycleStop, dataVar::m_pWindow, &MainWindow::slot_AllCycleStop);
    connect(this, &MainWidget::sig_Quit, m_pHeadWidget, &HeadWidget::shot_Quit);
    connect(m_pHeadWidget, &HeadWidget::sigColor, this, &MainWidget::doColorMenu);
    connect(m_pHeadWidget, &HeadWidget::sigMinimized, this, &MainWidget::showMinimized);
    connect(m_pHeadWidget, &HeadWidget::sigClose, this, &MainWidget::doClose, Qt::DirectConnection);
    connect(m_pHeadWidget, &HeadWidget::sigToggleMaximized, this, &MainWidget::doToggleMaximized); 
    //��¼�����ź����
    connect(dataVar::m_pWindow, &MainWindow::sig_LoginSignal, this, &MainWidget::slot_Login);
    //Ȩ�޹������ź����
    connect(dataVar::m_pWindow, &MainWindow::sig_PermisSet, this, &MainWidget::slot_Permis);
    //���ڻ����ź����
    connect(dataVar::m_pWindow, &MainWindow::sig_AboutSignal, this, &MainWidget::slot_About);    
}

void MainWidget::doClose()
{
    QMessageBox *msgBox = new QMessageBox(this);  
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowTitle("��ʾ!");
    msgBox->setText("~~��ȷ��Ҫ�뿪ϵͳ������?~~");
    QPushButton *btn_sure = msgBox->addButton("ȷ��", QMessageBox::ButtonRole::YesRole);
    QPushButton *bun_cancel = msgBox->addButton("ȡ��", QMessageBox::ButtonRole::NoRole);
    btn_sure->setStyleSheet("QPushButton{background-color:rgb(125,125,125);border:5px solid gray;font-size:15px;color:white;}");
    bun_cancel->setStyleSheet("QPushButton{background-color:rgb(125,125,125);border:5px solid gray;font-size:15px;color:white;}");  
    msgBox->setStyleSheet("color:black;font-size:16px;");   
    if (msgBox->exec())
    {
        msgBox->deleteLater();
        btn_sure->deleteLater();
        bun_cancel->deleteLater();
    }
    else
    {          
        m_pHeadWidget->close();
        m_pHeadWidget->deleteLater();
        dataVar::m_pWindow->close(); 
        this->close();
        emit sig_Quit();            
    }    
}

void MainWidget::setLogo(const QString &imgFile, const QString &name)
{
    m_pHeadWidget->setLogo(imgFile, name);
}

void MainWidget::setTitle(const QString &info)
{
    m_pHeadWidget->setTitle(info);
}

void MainWidget::setEnabelHeadMove(bool can_move)
{
    m_pHeadWidget->setEnabelMove(can_move);
}

void MainWidget::setToolDockWidget(QWidget *w)
{
    dataVar::m_pWindow->setToolDockWidget(w);
}

void MainWidget::setFlowDockWidget(QWidget *w)
{
    dataVar::m_pWindow->setFlowDockWidget(w);
}

void MainWidget::setLogDockWidget(QWidget *w)
{
    dataVar::m_pWindow->setLogDockWidget(w);
}

void MainWidget::setStatusInfo(const QString &info)
{
    dataVar::m_pWindow->setStatusInfo(info);
}

void MainWidget::setCenterWidget(QWidget *w)
{
    dataVar::m_pWindow->setCenterWidget(w);
}

void MainWidget::doToggleMaximized()
{
    if(this->isMaximized())
        this->showNormal();
    else
        this->showMaximized();
}

void MainWidget::doColorMenu(const QPoint &pos)
{
    if(!m_pColorMenu)
    {
        m_pColorMenu = new QMenu(this);
        QAction* greenAc = m_pColorMenu->addAction("��ɫ");
        QAction *blueAc  = m_pColorMenu->addAction("��ɫ");      
        QAction *redAc   = m_pColorMenu->addAction("��ɫ");
        greenAc->setData("rgb(33,115,70)");
        blueAc->setData("rgb(43, 87, 154)");      
        redAc->setData("rgb(183, 71, 42)");
        greenAc->setCheckable(true);
        greenAc->setChecked(true);
        blueAc->setCheckable(true);     
        redAc->setCheckable(true);   
        QActionGroup *pGroup = new QActionGroup(this);
        pGroup->setExclusive(true);      
        pGroup->addAction(greenAc);
        pGroup->addAction(blueAc);
        pGroup->addAction(redAc);
        connect(pGroup, &QActionGroup::triggered, this, [this](QAction *ac){
            m_pHeadWidget->setBgColor(ac->data().toString());});      
    }
    m_pColorMenu->exec(pos);
}

//��¼����
void MainWidget::slot_Login()
{    
    frmLogin *fLogin = new frmLogin(this);    
    fLogin->exec();
}

//Ȩ�޹�����
void MainWidget::slot_Permis()
{
    frmPermis* fPermis = new frmPermis(this);
    fPermis->exec();
}

//���ڻ���
void MainWidget::slot_About()
{
    frmAbout *fAbout = new frmAbout(this);
    fAbout->exec();   
}
