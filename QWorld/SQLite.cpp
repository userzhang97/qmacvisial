#include "QWorld.h"
#include <QString>

#pragma region SQLite
#pragma region ����������SQLite���ݿ�
//����������SQLite���ݿ�
bool SQLite::CreateConnectDB(QString key, QString sql_name, QSqlDatabase &database)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		//�����������ݿ�
		database = QSqlDatabase::addDatabase("QSQLITE");
		database.setDatabaseName(sql_name);
		bool bol_sql_open = database.open();
		if (bol_sql_open == false)
		{
			QMessageBox::critical(NULL, "Prompt", database.lastError().text());
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region �������
//�������
bool SQLite::CreateTable(QString key, QString table_name)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		QSqlQuery sql_query;
		QString sql = "create table " + table_name + "(USERNAME text, PASSWORD text)";
		if (!sql_query.exec(sql))
		{
			QMessageBox::critical(NULL, "Prompt", sql_query.lastError().text());
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region ��������
//��������
bool SQLite::InsertData(QString key, QString table_name, QString user_name, QString password)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		QSqlQuery sql_query;
		QString sql = "insert into " + table_name + " values (?, ?)";
		sql_query.prepare(sql);
		sql_query.addBindValue(user_name);
		sql_query.addBindValue(password);
		if (!sql_query.exec())
		{
			QMessageBox::critical(NULL, "Prompt", sql_query.lastError().text());
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region �޸�����
//�޸�����
bool SQLite::ModifyData(QString key, QString user_name, QString password)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		QSqlQuery sql_query;
		QString sql = "update LoginTable set PASSWORD = :PASSWORD where USERNAME = :USERNAME";
		sql_query.prepare(sql);
		sql_query.bindValue(":USERNAME", user_name);
		sql_query.bindValue(":PASSWORD", password);
		if (!sql_query.exec())
		{
			QMessageBox::critical(NULL, "Prompt", sql_query.lastError().text());
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region ��ѯ����
//��ѯ����
bool SQLite::QueryData(QString key, QString user_name, QString &password)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		//eg: "select * from LoginTable where USERNAME == 'user0';"
		QSqlQuery sql_query("select * from LoginTable where USERNAME == '" + user_name + "';");
		if (!sql_query.exec())
		{
			QMessageBox::critical(NULL, "Prompt", sql_query.lastError().text());
			return false;
		}
		else
		{
			while (sql_query.next())
			{
				password = sql_query.value(1).toString();
			}
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region ɾ������
//ɾ������
bool SQLite::DeleteData(QString key, QString user_name)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		QSqlQuery sql_query("delete from LoginTable where USERNAME == '" + user_name + "';");
		if (!sql_query.exec())
		{
			QMessageBox::critical(NULL, "Prompt", sql_query.lastError().text());
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region ɾ�����
//ɾ�����
bool SQLite::DeleteTable(QString key, QString table_name)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		QSqlQuery sql_query;
		sql_query.exec("drop table " + table_name);
		if (sql_query.exec())
		{
			QMessageBox::critical(NULL, "Prompt", sql_query.lastError().text());
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region �ر����ݿ�
//�ر����ݿ�
bool SQLite::CloseDB(QString key, QSqlDatabase database)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		database.close();
		return true;
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion
#pragma endregion
