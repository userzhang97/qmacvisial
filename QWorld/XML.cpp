#include "QWorld.h"

#pragma region XML
#pragma region ����5��������Xml�ļ�
//����5��������Xml�ļ�
bool XML::CreateXml5(QString key, QString file_name, QString value1, QString value2, QString value3, QString value4, QString value5)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		//�Զ�д��ʽ��ָ��Ŀ¼��.xml�ļ��������ļ�����������Զ�����
		QFile file(file_name);
		if (!file.open(QFile::WriteOnly | QFile::Truncate))
		{
			return false;
		}
		//ʹ��QDomDocument����	
		QDomDocument doc;
		//д��xmlͷ��
		QDomProcessingInstruction instruction; //��Ӵ�������
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		//��Ӹ��ڵ�
		QDomElement root = doc.createElement("Parameters");
		doc.appendChild(root);
		//��ӵ�һ���ӽڵ㼰����Ԫ��
		QDomElement book = doc.createElement("ParamSetting");
		QDomElement param = doc.createElement("Parameter1"); //������Ԫ��
		QDomText text; //�������ű�ǩ�м��ֵ
		text = doc.createTextNode(value1);
		book.appendChild(param);
		param.appendChild(text);
		param = doc.createElement("Parameter2"); //������Ԫ��
		text = doc.createTextNode(value2);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter3"); //������Ԫ��
		text = doc.createTextNode(value3);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter4"); //������Ԫ��
		text = doc.createTextNode(value4);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter5"); //������Ԫ��
		text = doc.createTextNode(value5);
		param.appendChild(text);
		book.appendChild(param);		
		root.appendChild(book);
		//������ļ�
		QTextStream out_stream(&file);
		doc.save(out_stream, 4); //����4��
		file.close();
		return true;
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region ����9��������Xml�ļ�
//����9��������Xml�ļ�
bool XML::CreateXml9(QString key, QString file_name, QString value1, QString value2, QString value3, QString value4, QString value5, QString value6, QString value7, QString value8, QString value9)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		//�Զ�д��ʽ��ָ��Ŀ¼��.xml�ļ��������ļ�����������Զ�����
		QFile file(file_name);
		if (!file.open(QFile::WriteOnly | QFile::Truncate))
		{
			return false;
		}
		//ʹ��QDomDocument����	
		QDomDocument doc;
		//д��xmlͷ��
		QDomProcessingInstruction instruction; //��Ӵ�������
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		//��Ӹ��ڵ�
		QDomElement root = doc.createElement("Parameters");
		doc.appendChild(root);
		//��ӵ�һ���ӽڵ㼰����Ԫ��
		QDomElement book = doc.createElement("ParamSetting");
		QDomElement param = doc.createElement("Parameter1"); //������Ԫ��
		QDomText text; //�������ű�ǩ�м��ֵ
		text = doc.createTextNode(value1);
		book.appendChild(param);
		param.appendChild(text);
		param = doc.createElement("Parameter2"); //������Ԫ��
		text = doc.createTextNode(value2);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter3"); //������Ԫ��
		text = doc.createTextNode(value3);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter4"); //������Ԫ��
		text = doc.createTextNode(value4);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter5"); //������Ԫ��
		text = doc.createTextNode(value5);
		param.appendChild(text);
		book.appendChild(param);		
		param = doc.createElement("Parameter6"); //������Ԫ��
		text = doc.createTextNode(value6);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter7"); //������Ԫ��
		text = doc.createTextNode(value7);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter8"); //������Ԫ��
		text = doc.createTextNode(value8);
		param.appendChild(text);
		book.appendChild(param);
		param = doc.createElement("Parameter9"); //������Ԫ��
		text = doc.createTextNode(value9);
		param.appendChild(text);
		book.appendChild(param);
		root.appendChild(book);
		//������ļ�
		QTextStream out_stream(&file);
		doc.save(out_stream, 4); //����4��
		file.close();
		return true;
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region �޸�Xml�ļ��Ĳ���
//�޸�Xml�ļ��Ĳ���
bool XML::ModifyXml(QString key, QString file_name, vector<string> parameter, vector<string> value)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		//�Զ���ʽ��ָ��Ŀ¼��.xml�ļ�
		QFile file(file_name);
		if (!file.open(QFile::ReadOnly))
		{
			return false;
		}
		//ʹ��QDomDocument����	
		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			return false;
		}
		file.close();
		for (int i = 0; i < parameter.size(); i++)
		{
			QDomElement root = doc.documentElement();
			QDomNodeList list = root.elementsByTagName(QString::fromStdString(parameter[i]));
			QDomNode node = list.at(0);
			QDomNode oldnode = node.firstChild();
			node.firstChild().setNodeValue(QString::fromStdString(value[i]));
			QDomNode newnode = node.firstChild();
			node.replaceChild(newnode, oldnode);
		}
		if (!file.open(QFile::WriteOnly | QFile::Truncate))
			return false;
		//������ļ�
		QTextStream out_stream(&file);
		doc.save(out_stream, 4); //����4��
		file.close();
		return true;
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion

#pragma region ��ȡXml�ļ��Ĳ���
//��ȡXml�ļ��Ĳ���
bool XML::ReadXml(QString key, QString file_name, QString parameter, QString &value)
{
	if (key != "59226f4700f1cc8a5de755d501af308d2d2")
	{
		return false;
	}
	try
	{
		//�Զ���ʽ��ָ��Ŀ¼��.xml�ļ�
		QFile file(file_name);
		if (!file.open(QFile::ReadOnly))
		{
			return false;
		}
		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			return false;
		}
		file.close();
		QDomElement root = doc.documentElement(); //���ظ��ڵ�	
		QDomNode node = root.firstChild(); //��õ�һ���ӽڵ�
		while (!node.isNull())  //����ڵ㲻��
		{
			if (node.isElement()) //����ڵ���Ԫ��
			{
				QDomElement e = node.toElement(); //ת��ΪԪ�أ�ע��Ԫ�غͽڵ����������ݽṹ		
				QDomNodeList list = e.childNodes();
				for (int i = 0; i < list.count(); i++) //������Ԫ��
				{
					QDomNode n = list.at(i);
					if (node.isElement())
					{
						if (n.nodeName() == parameter)
						{
							value = n.toElement().text();
						}
					}
				}
			}
			node = node.nextSibling();
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}
#pragma endregion
#pragma endregion
