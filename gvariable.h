#pragma once

#include <QMap>
#include <QPoint>
#include <QPointF>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSerialPort>
#include <opencv2\opencv.hpp>
#include "winsock2.h"
#include "./CommTools/Camera/MindVision/Include/CameraApi.h"

class QToolBase;

class gVariable 
{	
public:		
	//����ȫ�ֱ����ṹ��
	typedef struct GLOBALVAR
	{		
		QString global_type;
		int global_int_value;		
		double global_double_value;		
		QString global_qstring_value;		
		bool global_bool_value;		
		QPoint global_qpoint_value;		
		QPointF global_qpointf_value;
		cv::Point global_cvpoint_value;
		cv::Point2f global_cvpoint2f_value;
		cv::Point3f global_cvpoint3f_value;
		cv::Point3d global_cvpoint3d_value;
		std::vector<float> global_array_float_value;
		std::vector<double> global_array_double_value;
	} Global_Var;
	static Global_Var GlobalVar;
	static QMap<QString, Global_Var> global_variable_link;	
	//������ת���߽ṹ��
	typedef struct GOTOVAR
	{		
		std::vector<QString> goto_array_name_value;		
	} Goto_Var;
	static Goto_Var GotoVar;
	static QMap<QString, Goto_Var> goto_variable_link;
	//����ͨѶ�ṹ��
	typedef struct CAMERAVAR  //���
	{
		QString camera_type;
		CameraHandle mindvision_haldle_value;
		BYTE* mindvision_framebuffer_value = nullptr;
		int time_out;
	} Camera_Var;
	static Camera_Var CameraVar;
	static QMap<QString, Camera_Var> camera_variable_link;
	typedef struct GENERALIOVAR  //ͨ��I/O
	{
		QTcpSocket* io_value = nullptr;
		int connect_state;
		QString ip_value;
		int port_value;
		int over_time_value;
		QString remark_value;
	} GeneralIo_Var;
	static GeneralIo_Var GeneralIoVar;
	static QMap<QString, GeneralIo_Var> generalio_variable_link;
	typedef struct PLCCOMMUNICATEVAR  //PLCͨ��
	{
		QTcpSocket* mit_value = nullptr;
		int connect_state;
		QString plc_type;
		QString mit_ip_value;
		int mit_port_value;
		int mit_over_time_value;
		QString mit_remark_value;
	} PlcCommunicate_Var;
	static PlcCommunicate_Var PlcCommunicateVar;
	static QMap<QString, PlcCommunicate_Var> plccommunicate_variable_link;
	typedef struct SERIALPORTVAR  //����ͨ��
	{
		QSerialPort* serialport_value = nullptr;
		int connect_state;
		QString portname_value;
		int baudrate_value;
		QSerialPort::Parity parity_value;
		QSerialPort::DataBits databits_value;
		QSerialPort::StopBits stopbits_value;
		QSerialPort::FlowControl flowcontrol_value;
		QString remark_value;
	} SerialPort_Var;
	static SerialPort_Var SerialPortVar;
	static QMap<QString, SerialPort_Var> serialport_variable_link;
	typedef struct SOCKETTCPSERVERVAR  //TCP/IP������
	{
		QTcpServer* server_value = nullptr;	
		int connect_state;
		QString protocol_value;
		QString ip_value;
		int port_value;
		QString remark_value;
	} SocketTcpServer_Var;
	static SocketTcpServer_Var SocketTcpServerVar;
	static QMap<QString, SocketTcpServer_Var> sockettcpserver_variable_link;
	typedef struct SOCKETTCPCLIENTVAR  //TCP/IP�ͻ���
	{
		QTcpSocket* client_value = nullptr;
		int connect_state;
		QString protocol_value;
		QString ip_value;
		int port_value;
		QString remark_value;
	} SocketTcpClient_Var;
	static SocketTcpClient_Var SocketTcpClientVar;
	static QMap<QString, SocketTcpClient_Var> sockettcpclient_variable_link;
	typedef struct SCRIPTEDITVAR  //�ű��༭
	{
		QVector<QString> flow_pro_item_list;
		int item_id;
	} ScriptEdit_Var;
	static ScriptEdit_Var ScriptEditVar;
	static QMap<QToolBase*, ScriptEdit_Var> scriptedit_variable_link;
};
