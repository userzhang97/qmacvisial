#include "frmLink.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QDesktopWidget>
#include "datavar.h"
using namespace std;

frmLink::frmLink(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowIcon(QIcon(":/res/ico/link.png"));
	//FramelessWindowHint�������ô���ȥ���߿�
	//WindowMinimizeButtonHint ���������ڴ�����С��ʱ��������������ڿ�����ʾ��ԭ����
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	//���ô�������Ļ�м�λ��
	QDesktopWidget* desktop = QApplication::desktop();
	move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);
	//���ô��ڱ���͸��
	setAttribute(Qt::WA_TranslucentBackground);
	//��ʼ��������
	initTitleBar();
	form_load();
	slot_CreateList();
	connect(ui.tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(slot_DoubleClicked(int, int)));
}

frmLink::~frmLink()
{
	if (this != nullptr)
	{
		this->deleteLater();
	}
}

void frmLink::initTitleBar()
{
	MyTitleBar* m_titleBar = new MyTitleBar(this);
	m_titleBar->move(0, 0);
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
	m_titleBar->setBackgroundColor(3, 110, 95);
	m_titleBar->setStyleSheet("background-color: rgba(0, 0, 0, 0);color:white;font-size:16px");
	m_titleBar->setTitleIcon(":/res/ico/link.png");
	m_titleBar->setTitleContent("��������");
	m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}

void frmLink::paintEvent(QPaintEvent* event)
{
	//���ñ���ɫ
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.fillPath(pathBack, QBrush(QColor("#bcc2bc")));
	return QWidget::paintEvent(event);
}

void frmLink::form_load()
{
	//���ñ���п�	
	ui.tableWidget->setColumnWidth(0, 50);
	ui.tableWidget->setColumnWidth(1, 203);
	ui.tableWidget->setColumnWidth(2, 300);
	ui.tableWidget->setColumnWidth(3, 232);
	//����ˮƽheader
	ui.tableWidget->verticalHeader()->setVisible(false);
	//��������ѡ��
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableWidget->horizontalHeader()->setDisabled(true);	
	//���List	 	
	ItemModel = new QStandardItemModel(this);
	ui.listProcess->setEditTriggers(QAbstractItemView::NoEditTriggers);  //��ֹ�༭
}

void frmLink::onButtonCloseClicked()
{
	//�ź���۽������
	disconnect(ui.listProcess, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_ListClick(QModelIndex)));
	disconnect(ui.tableWidget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(slot_DoubleClicked(int, int)));
	this->close();
	this->deleteLater();
}

void frmLink::slot_CreateList()
{
	//��ȡ�ӿؼ�
	objTabName = dataVar::fProItemTab->findChild<QTabWidget*>("ProItemTabWidget");
	objTreeName = dataVar::fProItemTab->findChild<QTreeWidget*>("ProItemTreeWidget");
	int flow = objTabName->currentIndex();  //������
	flow_index = 0;
	QTreeWidgetItemIterator it(objTreeName);  //����treeWidget
	QList<QTreeWidgetItem*> pro_keys = dataVar::fProItemTab->m_pro_value.uniqueKeys();
	for (int i = 0; i < (*it)->childCount(); i++)
	{
		QTreeWidgetItem* key = pro_keys[i];
		if ((*it)->child(flow) == pro_keys[i])
		{
			flow_index = dataVar::fProItemTab->m_pro_value.values(key).at(0);
		}
	}
	QVector<QString> FlowProItemList;
	FlowProItemList.reserve(100);
	FlowProItemList.clear();
	int item_id;
	QString flowPageName = objTabName->tabText(objTabName->currentIndex());
	QMap<QString, QtDragListWidget*>::iterator iter = dataVar::FlowProMap.begin();
	while (iter != dataVar::FlowProMap.end())
	{
		if (iter.key() == flowPageName)
		{
			TempDragListWidget = iter.value();
			FlowProItemList = TempDragListWidget->GetAllItemList();
			item_id = TempDragListWidget->GetCurrentItemId() - 1;
			break;
		}
		iter++;
	}
	//�������ڵ�����λ��	
	tool_name_buf = FlowProItemList[item_id];
	//���List	 		
	ItemModel->removeRows(0, ItemModel->rowCount());
	//���ȫ�ֱ���
	QStandardItem* item = new QStandardItem("ȫ�ֱ���");
	QPixmap pixmap = GetIcon(GetIconName("ȫ�ֱ���"));
	item->setIcon(pixmap);
	ItemModel->appendRow(item);
	for (int i = 0; i < item_id; i++)
	{
		//�������
		QStandardItem* item = new QStandardItem(FlowProItemList[i]);
		QPixmap pixmap = GetIcon(GetIconName(FlowProItemList[i]));
		item->setIcon(pixmap);
		ItemModel->appendRow(item);
	}
	ui.listProcess->setModel(ItemModel);
	connect(ui.listProcess, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_ListClick(QModelIndex)));
}

void frmLink::slot_ListClick(QModelIndex index)
{
	index_process_modify = index.row();
	tool_name = index.data().toString();
	//��ȡ���������ַ���	
	int str_count = 0;
	for (int k = 0; k < tool_name.length(); k++)
	{
		if (!(tool_name[k] > '0' && tool_name[k] < '9'))
		{
			++str_count;			
		}
	}
	QString str_name_buf = tool_name.mid(0, str_count);
	QStringList toolNames;
	toolNames.reserve(100);
	toolNames.clear();
	toolNames << "ȫ�ֱ���" << "��ȡͼ��" << "ͼ����ʾ" << "����ͼ��" << "Ԥ����" << "ͼ��ƴ��" << "ͼ���޸�" << "ͼ��ϸ��" << "ͼ��ת" << "ͼ����ת" << "͸�ӱ任" << "����ͼ��" << "����ROI" << "������ʶ��" << "�ߵ����" << "��ά��ʶ��"
		<< "�ַ�ʶ��" << "������" << "��ɫʶ��" << "���ȼ��" << "ͼ��������" << "��������ѡ��" << "N��궨" << "����궨" << "�����궨" << "�Ҷ�ƥ��" << "��״ƥ��" << "Ŀ�����" << "���Լ���" << "Ѱ��Բ" << "Ѱ��ֱ��" << "���Բ" << "�����Բ" << "���ֱ��" << "��ȡ�߽��"
	    << "��Բ����" << "��+��" << "��+��" << "���߽���" << "����Բȱ��" << "��Ե��Ȳ���"	<< "���ƽ��" << "��չ��" << "��ת���" << "�ж����" << "�������" << "�ű��༭" << "TCP/IP������" << "TCP/IP�ͻ���" << "PLCͨ��" << "����ͨ��" << "ͨ��I/O" << "��ʱ" << "����CSV";
	//���Table
	int rowNum = ui.tableWidget->rowCount();
	for (int i = rowNum - 1; i >= 0; i--)
	{
		ui.tableWidget->removeRow(i);
	}
	switch (toolNames.indexOf(str_name_buf)) {
#pragma region ȫ�ֱ���
	case 0:
		//ȫ�ֱ���		
		objTableName = dataVar::fGlobalVariable->findChild<QTableWidget*>("tableWidget");  //��ȡ�ӿؼ�
		row_count = objTableName->rowCount();
		ui.tableWidget->setRowCount(row_count);  //��������	
		for (int k = 0; k < row_count; k++)
		{
			item_number = new QTableWidgetItem(QString::number(k + 1));
			item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
			item_number->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(k, 0, item_number);
			item_variable_name = new QTableWidgetItem(objTableName->item(k, 1)->text());
			item_variable_value = new QTableWidgetItem(objTableName->item(k, 2)->text());
			if (objTableName->item(k, 0)->text() == "Int")
			{
				item_variable_type = new QTableWidgetItem("Int");
			}
			else if (objTableName->item(k, 0)->text() == "Double")
			{
				item_variable_type = new QTableWidgetItem("Double");
			}
			else if (objTableName->item(k, 0)->text() == "QString")
			{
				item_variable_type = new QTableWidgetItem("QString");
			}
			else if (objTableName->item(k, 0)->text() == "Bool")
			{
				item_variable_type = new QTableWidgetItem("Bool");
			}
			else if (objTableName->item(k, 0)->text() == "QPoint")
			{
				item_variable_type = new QTableWidgetItem("QPoint");
			}
			else if (objTableName->item(k, 0)->text() == "QPointF")
			{
				item_variable_type = new QTableWidgetItem("QPointF");
			}
			else if (objTableName->item(k, 0)->text() == "cv::Point")
			{
				item_variable_type = new QTableWidgetItem("cv::Point");
			}
			else if (objTableName->item(k, 0)->text() == "cv::Point2f")
			{
				item_variable_type = new QTableWidgetItem("cv::Point2f");
			}
			else if (objTableName->item(k, 0)->text() == "cv::Point3f")
			{
				item_variable_type = new QTableWidgetItem("cv::Point3f");
			}
			else if (objTableName->item(k, 0)->text() == "cv::Point3d")
			{
				item_variable_type = new QTableWidgetItem("cv::Point3d");
			}
			else if (objTableName->item(k, 0)->text() == "Float[]")
			{
				item_variable_type = new QTableWidgetItem("Float[]");
			}
			else if (objTableName->item(k, 0)->text() == "Double[]")
			{
				item_variable_type = new QTableWidgetItem("Double[]");
			}
			item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
			item_variable_name->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(k, 1, item_variable_name);
			item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
			item_variable_value->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(k, 2, item_variable_value);
			item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
			item_variable_type->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(k, 3, item_variable_type);
		}
		break;
#pragma endregion

#pragma region ͼ����
	case 1:
		//��ȡͼ��		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 2:
		//ͼ����ʾ		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputViewImage");
						item_variable_type = new QTableWidgetItem("QImage");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 3:
		//����ͼ��		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(1);  //��������	
				for (int k = 0; k < 1; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;	
	case 4:
		//Ԥ����		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 5:
		//ͼ��ƴ��		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 6:
		//ͼ���޸�
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 7:
		//ͼ��ϸ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 8:
		//ͼ��ת
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 9:
		//ͼ����ת
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 10:
		//͸�ӱ任
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 11:
		//����ͼ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 12:
		//����ROI
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".����");
						item_variable_value = new QTableWidgetItem("PublicImageProcess.CvRect");
						item_variable_type = new QTableWidgetItem("cv::Rect");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��ת����");
						item_variable_value = new QTableWidgetItem("PublicImageProcess.CvRotatedRect");
						item_variable_type = new QTableWidgetItem("cv::RotatedRect");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
#pragma endregion

#pragma region ���ʶ��
	case 13:
		//������ʶ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".������");
						item_variable_value = new QTableWidgetItem("PublicDetect.Code");
						item_variable_type = new QTableWidgetItem("vector<QString>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 14:
		//�ߵ����		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;					
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�ߵ����");
						item_variable_value = new QTableWidgetItem("PublicDetect.Quantity");
						item_variable_type = new QTableWidgetItem("Int");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;					
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 15:
		//��ά��ʶ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��ά��");
						item_variable_value = new QTableWidgetItem("PublicDetect.Code");
						item_variable_type = new QTableWidgetItem("vector<QString>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 16:
		//�ַ�ʶ��
		
		break;
	case 17:
		//������
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���");
						item_variable_value = new QTableWidgetItem("PublicDetect.Category");
						item_variable_type = new QTableWidgetItem("QString");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 18:
		//��ɫʶ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;					
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���ƶ�");
						item_variable_value = new QTableWidgetItem("PublicDetect.Score");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 19:
		//���ȼ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".����");
						item_variable_value = new QTableWidgetItem("PublicDetect.Brightness");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 20:
		//ͼ��������
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".������");
						item_variable_value = new QTableWidgetItem("PublicDetect.Clarity");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;		
	case 21:
		//��������ѡ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(5);  //��������	
				for (int k = 0; k < 5; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;					
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".����");
						item_variable_value = new QTableWidgetItem("PublicDetect.Contours");
						item_variable_type = new QTableWidgetItem("vector<vector<cv::Point>>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���");
						item_variable_value = new QTableWidgetItem("PublicDetect.Areas");
						item_variable_type = new QTableWidgetItem("vector<Double>");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".����");
						item_variable_value = new QTableWidgetItem("PublicDetect.CenterPoints");
						item_variable_type = new QTableWidgetItem("vector<cv::Point2f>");
						break;
					case 4:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
#pragma endregion

#pragma region �궨����
	case 22:
		//N��궨
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(9);  //��������	
				for (int k = 0; k < 9; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".N��궨����");
						item_variable_value = new QTableWidgetItem("PublicCalib.EstimateAffine");
						item_variable_type = new QTableWidgetItem("vector<Double>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��ת����");
						item_variable_value = new QTableWidgetItem("PublicCalib.RotateCenter");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���ص���X");
						item_variable_value = new QTableWidgetItem("PublicCalib.PixelEquivalentX");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 4:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���ص���Y");
						item_variable_value = new QTableWidgetItem("PublicCalib.PixelEquivalentY");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 5:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��׼��1");
						item_variable_value = new QTableWidgetItem("PublicCalib.DatumPoint1");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 6:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��׼��2");
						item_variable_value = new QTableWidgetItem("PublicCalib.DatumPoint2");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 7:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��׼�Ƕ�");
						item_variable_value = new QTableWidgetItem("PublicCalib.DatumAngle");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 8:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 23:
		//����궨
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�ڲ�K");
						item_variable_value = new QTableWidgetItem("PublicCalib.CameraMatrix");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".����ϵ��");
						item_variable_value = new QTableWidgetItem("PublicCalib.DistCoeffs");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 24:
		//�����궨
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���ص���X");
						item_variable_value = new QTableWidgetItem("PublicCalib.PixelEquivalentX");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���ص���Y");
						item_variable_value = new QTableWidgetItem("PublicCalib.PixelEquivalentY");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
#pragma endregion

#pragma region ��λ����
	case 25:
		//�Ҷ�ƥ��		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(6);  //��������	
				for (int k = 0; k < 6; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ����������");
						item_variable_value = new QTableWidgetItem("PublicTPosition.Center");
						item_variable_type = new QTableWidgetItem("vector<cv::Point2f>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ��Ƕ�");
						item_variable_value = new QTableWidgetItem("PublicTPosition.Angle");
						item_variable_type = new QTableWidgetItem("vector<Double>");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ�����");
						item_variable_value = new QTableWidgetItem("PublicTPosition.OutScore");
						item_variable_type = new QTableWidgetItem("vector<Double>");
						break;
					case 4:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ���׼����");
						item_variable_value = new QTableWidgetItem("PublicTPosition.DatumCenter");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 5:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;	
	case 26:
		//��״ƥ��		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(6);  //��������	
				for (int k = 0; k < 6; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ����������");
						item_variable_value = new QTableWidgetItem("PublicTPosition.Center");
						item_variable_type = new QTableWidgetItem("vector<cv::Point2f>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ��Ƕ�");
						item_variable_value = new QTableWidgetItem("PublicTPosition.Angle");
						item_variable_type = new QTableWidgetItem("vector<Double>");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ�����");
						item_variable_value = new QTableWidgetItem("PublicTPosition.OutScore");
						item_variable_type = new QTableWidgetItem("vector<Double>");
						break;
					case 4:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƥ���׼����");
						item_variable_value = new QTableWidgetItem("PublicTPosition.DatumCenter");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 5:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 27:
		//Ŀ�����
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��ת����");
						item_variable_value = new QTableWidgetItem("PublicTPosition.CvRotatedRect");
						item_variable_type = new QTableWidgetItem("cv::RotatedRect");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 28:
		//���Լ���
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {					
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�ƶ���X");
						item_variable_value = new QTableWidgetItem("PublicTPosition.MovingDistanceX");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�ƶ���Y");
						item_variable_value = new QTableWidgetItem("PublicTPosition.MovingDistanceY");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���������");
						item_variable_value = new QTableWidgetItem("PublicTPosition.WorldCoordinatePoint");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;					
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
#pragma endregion

#pragma region ���ι���/����
	case 29:
		//Ѱ��Բ
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Center");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�뾶");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Radius");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 30:
		//Ѱ��ֱ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".б��");
						item_variable_value = new QTableWidgetItem("PublicGeometry.k");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�ؾ�");
						item_variable_value = new QTableWidgetItem("PublicGeometry.b");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 31:
		//���Բ
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Center");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�뾶");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Radius");
						item_variable_type = new QTableWidgetItem("Double");
						break;					
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 32:
		//�����Բ
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(6);  //��������	
				for (int k = 0; k < 6; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Center");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�Ƕ�");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Angle");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���᳤��");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Length1");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 4:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".���᳤��");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Length2");
						item_variable_type = new QTableWidgetItem("Double");
						break;							
					case 5:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 33:
		//���ֱ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;					
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".б��");
						item_variable_value = new QTableWidgetItem("PublicGeometry.k");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�ؾ�");
						item_variable_value = new QTableWidgetItem("PublicGeometry.b");
						item_variable_type = new QTableWidgetItem("Double");
						break;		
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 34:
		//��ȡ�߽��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;					
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�㼯");
						item_variable_value = new QTableWidgetItem("PublicGeometry.BorderPoints");
						item_variable_type = new QTableWidgetItem("vector<cv::Point2f>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 35:
		//��Բ����
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Intersections");
						item_variable_type = new QTableWidgetItem("vector<cv::Point2f>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 36:
		//��+��	
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(5);  //��������	
				for (int k = 0; k < 5; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Center");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�Ƕ�");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Angle");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".����");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Distance");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 4:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 37:
		//��+��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Pedal");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".����");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Distance");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 38:
		//���߽���
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(4);  //��������	
				for (int k = 0; k < 4; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Intersection");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�н�");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Angle");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 39:
		//����Բȱ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(5);  //��������	
				for (int k = 0; k < 5; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".Բ������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Center");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Pedal");
						item_variable_type = new QTableWidgetItem("cv::Point2f");
						break;
					case 3:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�Ƕ�");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Angle");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 4:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 40:
		//��Ե��Ȳ���
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;					
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��Ե���");
						item_variable_value = new QTableWidgetItem("PublicGeometry.Distance");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
#pragma endregion

#pragma region ��ά���
	case 41:
		//���ƽ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ƽ�����");
						item_variable_value = new QTableWidgetItem("PublicThreeD.Flatness");
						item_variable_type = new QTableWidgetItem("vector<Double>");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�㵽ƽ�����");
						item_variable_value = new QTableWidgetItem("PublicThreeD.Distance");
						item_variable_type = new QTableWidgetItem("Double");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;	
#pragma endregion

#pragma region �߼�����
	case 42:
		//��չ��		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".ͼ��");
						item_variable_value = new QTableWidgetItem("PublicImage.OutputImage");
						item_variable_type = new QTableWidgetItem("cv::Mat");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 43:
		//��ת���
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(1);  //��������	
				for (int k = 0; k < 1; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 44:
		//�ж����
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(1);  //��������	
				for (int k = 0; k < 1; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 45:
		//�������
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(1);  //��������	
				for (int k = 0; k < 1; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:						
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 46:
		//�ű��༭
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(1);  //��������	
				for (int k = 0; k < 1; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
#pragma endregion

#pragma region ͨѶ����
	case 47:
		//TCP/IP������
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicCommunication.InputData");
						item_variable_type = new QTableWidgetItem("QString");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicCommunication.OutputData");
						item_variable_type = new QTableWidgetItem("QString");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 48:
		//TCP/IP�ͻ���
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicCommunication.InputData");
						item_variable_type = new QTableWidgetItem("QString");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicCommunication.OutputData");
						item_variable_type = new QTableWidgetItem("QString");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 49:
		//PLCͨ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�Ĵ�����ȡ����");
						item_variable_value = new QTableWidgetItem("PublicCommunication.RegisterReadData");
						item_variable_type = new QTableWidgetItem("QVector<int>");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�Ĵ���д������");
						item_variable_value = new QTableWidgetItem("PublicCommunication.RegisterWriteData");
						item_variable_type = new QTableWidgetItem("QVector<int>");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 50:
		//����ͨ��
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(3);  //��������	
				for (int k = 0; k < 3; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicCommunication.InputData");
						item_variable_type = new QTableWidgetItem("QString");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".��������");
						item_variable_value = new QTableWidgetItem("PublicCommunication.OutputData");
						item_variable_type = new QTableWidgetItem("QString");
						break;
					case 2:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 51:
		//ͨ��I/O
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(2);  //��������	
				for (int k = 0; k < 2; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".�����");
						item_variable_value = new QTableWidgetItem("PublicCommunication.InPutIoX");
						item_variable_type = new QTableWidgetItem("QVector<Bool>");
						break;
					case 1:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
#pragma endregion

#pragma region ϵͳ����
	case 52:
		//��ʱ		
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(1);  //��������	
				for (int k = 0; k < 1; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;
	case 53:
		//����CSV
		for (int j = 0; j < QConfig::ToolBase[flow_index]->m_Tools.size(); j++)
		{
			QString toolName;
			toolName = QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName;
			if (tool_name == toolName)
			{
				ui.tableWidget->setRowCount(1);  //��������	
				for (int k = 0; k < 1; k++)
				{
					item_number = new QTableWidgetItem(QString::number(k + 1));
					item_number->setFlags(item_number->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_number->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 0, item_number);
					switch (k) {					
					case 0:
						item_variable_name = new QTableWidgetItem(QConfig::ToolBase[flow_index]->m_Tools[j].PublicToolName + ".״̬");
						item_variable_value = new QTableWidgetItem("PublicResult.State");
						item_variable_type = new QTableWidgetItem("Bool");
						break;
					}
					item_variable_name->setFlags(item_variable_name->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_name->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 1, item_variable_name);
					item_variable_value->setFlags(item_variable_value->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_value->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 2, item_variable_value);
					item_variable_type->setFlags(item_variable_type->flags() & (~Qt::ItemIsEditable));  //�����в��ɱ༭	
					item_variable_type->setTextAlignment(Qt::AlignCenter);
					ui.tableWidget->setItem(k, 3, item_variable_type);
				}
			}
		}
		break;	
#pragma endregion
	}	
}

void frmLink::slot_DoubleClicked(int row, int column)
{
	dataVar::str_link = ui.tableWidget->item(row, 1)->text();
	QConfig::ToolBase[flow_index]->RunToolLink(tool_name_buf, dataVar::int_link, dataVar::str_link);
	onButtonCloseClicked();
}

// ��ȡͼ��
QPixmap frmLink::GetIcon(QString fileName)
{
	return IconHelper::Instance()->getPixmap(fileName);
}

// ��ȡͼ��·��
QString frmLink::GetIconName(QString Name)
{
	QString IconName;
	if (Name.contains("ȫ�ֱ���")) IconName = ":/res/ico/var.ico";
	if (Name.contains("��ȡͼ��")) IconName = ":/res/ico/image_source.png";
	if (Name.contains("ͼ����ʾ")) IconName = ":/res/ico/image_view.ico";
	if (Name.contains("����ͼ��")) IconName = ":/res/ico/export_image.png";
	if (Name.contains("�ߵ����")) IconName = ":/res/ico/blob.png";
	if (Name.contains("Ԥ����")) IconName = ":/res/ico/morphology.png";
	if (Name.contains("ͼ��ƴ��")) IconName = ":/res/ico/image_splice.png";
	if (Name.contains("ͼ���޸�")) IconName = ":/res/ico/repair.png";
	if (Name.contains("ͼ��ϸ��")) IconName = ":/res/ico/skeleton.png";
	if (Name.contains("ͼ��ת")) IconName = ":/res/ico/flip.png";
	if (Name.contains("ͼ����ת")) IconName = ":/res/ico/rotate.png";
	if (Name.contains("͸�ӱ任")) IconName = ":/res/ico/perspective.png";
	if (Name.contains("����ͼ��")) IconName = ":/res/ico/crop.png";
	if (Name.contains("����ROI")) IconName = ":/res/ico/roi.png";
	if (Name.contains("������ʶ��")) IconName = ":/res/ico/barcode.png";
	if (Name.contains("��ά��ʶ��")) IconName = ":/res/ico/qrcode.png";
	if (Name.contains("�ַ�ʶ��")) IconName = ":/res/ico/ocr.png";
	if (Name.contains("������")) IconName = ":/res/ico/classifier.png";
	if (Name.contains("��ɫʶ��")) IconName = ":/res/ico/color_r.png";
	if (Name.contains("���ȼ��")) IconName = ":/res/ico/brightness.png";
	if (Name.contains("ͼ��������")) IconName = ":/res/ico/clarity.png";	
	if (Name.contains("��������ѡ��")) IconName = ":/res/ico/shape.png";
	if (Name.contains("N��궨")) IconName = ":/res/ico/ert_calib.png";
	if (Name.contains("����궨")) IconName = ":/res/ico/distortion_calib.png";
	if (Name.contains("�����궨")) IconName = ":/res/ico/measure_calib.png";
	if (Name.contains("�Ҷ�ƥ��")) IconName = ":/res/ico/match.png";	
	if (Name.contains("��״ƥ��")) IconName = ":/res/ico/shape_match.png";
	if (Name.contains("Ŀ�����")) IconName = ":/res/ico/track.png";
	if (Name.contains("���Լ���")) IconName = ":/res/ico/affine.png";
	if (Name.contains("��Բ����")) IconName = ":/res/ico/line_circle.png";
	if (Name.contains("��+��")) IconName = ":/res/ico/point_point.png";
	if (Name.contains("��+��")) IconName = ":/res/ico/point_l.png";
	if (Name.contains("���߽���")) IconName = ":/res/ico/intersection.png";
	if (Name.contains("����Բȱ��")) IconName = ":/res/ico/rounded_c.png";
	if (Name.contains("Ѱ��Բ")) IconName = ":/res/ico/find_circle.png";
	if (Name.contains("Ѱ��ֱ��")) IconName = ":/res/ico/find_line.png";
	if (Name.contains("���Բ")) IconName = ":/res/ico/fit_circle.png";
	if (Name.contains("�����Բ")) IconName = ":/res/ico/fit_ellipse.png";
	if (Name.contains("���ֱ��")) IconName = ":/res/ico/fit_line.png";
	if (Name.contains("��ȡ�߽��")) IconName = ":/res/ico/border_point.png";
	if (Name.contains("��Ե��Ȳ���")) IconName = ":/res/ico/edge.png";
	if (Name.contains("���ƽ��")) IconName = ":/res/ico/flatness.png";	
	if (Name.contains("��չ��")) IconName = ":/res/ico/extension_library.png";
	if (Name.contains("��ת���")) IconName = ":/res/ico/goto.png";
	if (Name.contains("�ж����")) IconName = ":/res/ico/logic_judge.png";
	if (Name.contains("�������")) IconName = ":/res/ico/end.png";
	if (Name.contains("�ű��༭")) IconName = ":/res/ico/script_edit.png";
	if (Name.contains("TCP/IP������")) IconName = ":/res/ico/server.png";
	if (Name.contains("TCP/IP�ͻ���")) IconName = ":/res/ico/client.png";
	if (Name.contains("PLCͨ��")) IconName = ":/res/ico/plc_communicate.png";
	if (Name.contains("����ͨ��")) IconName = ":/res/ico/serial_port.png";
	if (Name.contains("ͨ��I/O")) IconName = ":/res/ico/general_io.png";
	if (Name.contains("��ʱ")) IconName = ":/res/ico/delay.png";
	if (Name.contains("����CSV")) IconName = ":/res/ico/export_csv.png";
	return IconName;
}
