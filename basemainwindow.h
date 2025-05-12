#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QWidget>
#include <QDialog>
#include "mytitlebar.h"

//Ҫ�̳�QMainWindow������Ҫ��QWidget��ΪQMainWindow
class BaseWindow : public QDialog
{
	Q_OBJECT

public:
	BaseWindow(QWidget*parent = 0);
	~BaseWindow();
	
private:
	void initTitleBar();
	void paintEvent(QPaintEvent *event);
	void loadStyleSheet(const QString &sheetName);
	
private slots:
	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	MyTitleBar* m_titleBar = new MyTitleBar;	
};
