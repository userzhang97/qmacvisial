#pragma once

#include <QtWidgets/QWidget>
#include <sstream>
#include <QString>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <vector>
using namespace std;

#pragma region SoftKey
class SoftKey
{
public:
	//SoftKey��	
	QString GetKey(QString key);
};
#pragma endregion

#pragma region FolderOperation
class FolderOperation
{
public:
	//ɾ��N��ǰ���ļ���
	bool FindFileForDelete(QString key, QString path, int day = -30);

	//ɾ��N��ǰ���ļ��У�
	bool FindFolderForDelete(QString key, QString path, int day = 30);
};
#pragma endregion

#pragma region SQLite
class SQLite
{
public:
	//����������SQLite���ݿ⣻
	//sql_nameΪ���ݿ��ļ�·�����磺QDir::currentPath() + "/MyDataBase.db"��
	//databaseΪ���ݿ⣻
	bool CreateConnectDB(QString key, QString sql_name, QSqlDatabase &database);

	//�������
	//table_nameΪ���ݿ�ı�����ƣ��磺LoginTable��
	bool CreateTable(QString key, QString table_name);

	//�������ݣ�
	//table_nameΪ���ݿ�ı�����ƣ��磺LoginTable��user_nameΪ�û�����passwordΪ���룻
	bool InsertData(QString key, QString table_name, QString user_name, QString password);

	//�޸�����(Ĭ�ϱ��ΪLoginTable)��
	//user_nameΪ�û�����passwordΪ�޸ĵ����룻
	bool ModifyData(QString key, QString user_name, QString password);

	//��ѯ����(Ĭ�ϱ��ΪLoginTable)��
	//user_nameΪ�û�����passwordΪ��ѯ�����룻
	bool QueryData(QString key, QString user_name, QString &password);

	//ɾ������(Ĭ�ϱ��ΪLoginTable)��
	//user_nameΪ�û�����
	bool DeleteData(QString key, QString user_name);

	//ɾ�����
	//table_nameΪ���ݿ�ı�����ƣ��磺LoginTable��
	bool DeleteTable(QString key, QString table_name);

	//�ر����ݿ⣻
	//databaseΪ���ݿ⣻
	bool CloseDB(QString key, QSqlDatabase database);
};
#pragma endregion	

#pragma region XML
class XML
{
public:
	//����5��������Xml�ļ���
	//file_nameΪXML�ļ�·�����磺QDir::currentPath() + "/Param.xml"��
	bool CreateXml5(QString key, QString file_name, QString value1, QString value2, QString value3, QString value4, QString value5);

	//����9��������Xml�ļ���
	//file_nameΪXML�ļ�·�����磺QDir::currentPath() + "/Param.xml"��
	bool CreateXml9(QString key, QString file_name, QString value1, QString value2, QString value3, QString value4, QString value5, QString value6, QString value7, QString value8, QString value9);

	//�޸�Xml�ļ��Ĳ�����
	bool ModifyXml(QString key, QString file_name, vector<string> parameter, vector<string> value);

	//��ȡXml�ļ��Ĳ�����
	bool ReadXml(QString key, QString file_name, QString parameter, QString& value);
};
#pragma endregion
