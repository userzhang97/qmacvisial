#include "gvariable.h"

//����ȫ�ֱ����ṹ��
gVariable::Global_Var gVariable::GlobalVar;
QMap<QString, gVariable::Global_Var> gVariable::global_variable_link = QMap<QString, gVariable::Global_Var>();
//������ת���߽ṹ��
gVariable::Goto_Var gVariable::GotoVar;
QMap<QString, gVariable::Goto_Var> gVariable::goto_variable_link = QMap<QString, gVariable::Goto_Var>();
//����ͨѶ�ṹ��
gVariable::Camera_Var gVariable::CameraVar;  //���
QMap<QString, gVariable::Camera_Var> gVariable::camera_variable_link = QMap<QString, gVariable::Camera_Var>();
gVariable::GeneralIo_Var gVariable::GeneralIoVar;  //ͨ��I/O
QMap<QString, gVariable::GeneralIo_Var> gVariable::generalio_variable_link = QMap<QString, gVariable::GeneralIo_Var>();
gVariable::PlcCommunicate_Var gVariable::PlcCommunicateVar;  //PLCͨ��
QMap<QString, gVariable::PlcCommunicate_Var> gVariable::plccommunicate_variable_link = QMap<QString, gVariable::PlcCommunicate_Var>();
gVariable::SerialPort_Var gVariable::SerialPortVar;  //����ͨ��
QMap<QString, gVariable::SerialPort_Var> gVariable::serialport_variable_link = QMap<QString, gVariable::SerialPort_Var>();
gVariable::SocketTcpServer_Var gVariable::SocketTcpServerVar;  //TCP/IP������
QMap<QString, gVariable::SocketTcpServer_Var> gVariable::sockettcpserver_variable_link = QMap<QString, gVariable::SocketTcpServer_Var>();
gVariable::SocketTcpClient_Var gVariable::SocketTcpClientVar;  //TCP/IP�ͻ���
QMap<QString, gVariable::SocketTcpClient_Var> gVariable::sockettcpclient_variable_link = QMap<QString, gVariable::SocketTcpClient_Var>();
gVariable::ScriptEdit_Var gVariable::ScriptEditVar;  //�ű��༭
QMap<QToolBase*, gVariable::ScriptEdit_Var> gVariable::scriptedit_variable_link = QMap<QToolBase*, gVariable::ScriptEdit_Var>();
