#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>
#include "winsock2.h"

class QLabel;
class QTimer;
class QProcess;

class ShowCpuMemory : public QObject
{
    Q_OBJECT

public:
    explicit ShowCpuMemory(QObject *parent = 0);

    void SetLab(QLabel *labCPUMemory);    

private:   
    int memoryPercent;
    int memoryAll;
    int memoryUse;
    int memoryFree;          
    QLabel *labMemory;   //��ʾ�ڴ���Ϣ�Ŀؼ�
    QProcess *process;	
	int cpu;

public slots:
    void GetCpuMemory();

private slots:     
	int calCpuUsage();
	//ʱ��ת��
	__int64 Filetime2Int64(const FILETIME* ftime);
	//����ʱ���������
	__int64 CompareFileTime(FILETIME preTime, FILETIME nowTime);
};

