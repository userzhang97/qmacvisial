#pragma once

#include <QObject>

class manualThread : public QObject
{
	Q_OBJECT

public:
	explicit manualThread(QObject *parent = nullptr);
	//�̴߳�����
	void funThread_A();
	//�����߳��б�־,����ֹͣ�߳�
	void setStopFlag(bool flag = true); 
	void setPauseFlag(bool flag = true);

private:
	//�̹߳�����־
	bool isStop; 
	bool isPause;
};
