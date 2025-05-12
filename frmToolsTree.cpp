#include "frmToolsTree.h"
#include <QHeaderView>
#include <QStyleFactory>
#include <QDir>
#include <QPluginLoader>
#include "GlobalVariable.h"

QString ThemeColor = "#20B2AA";

frmToolsTree::frmToolsTree(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	ToolsTreeWidgetInit();
}

//�����б��ʼ��
void frmToolsTree::ToolsTreeWidgetInit()
{
	ToolTreeWidget = new QtTreeWidget(this);
	QGridLayout* ToolGLayout = new QGridLayout(ui.ToolTreeFrame);
	ToolGLayout->setContentsMargins(0, 0, 0, 0);
	ToolGLayout->setSpacing(5);
	ToolGLayout->addWidget(ToolTreeWidget, 0, 0);
	ToolTreeWidget->setIconSize(QSize(24, 24));
	//��ʾ����
	ToolTreeWidget->setStyle(QStyleFactory::create("windows"));
	//����ϵͳĬ��ͼ��
	ToolTreeWidget->setStyleSheet("QTreeWidget::branch:has-children:!has-siblings:closed,\
	QWidget{border: 1px;border-style: solid;border-color: #d9d9d9}\
	QTreeWidget::item{color: #363636}\
	QTreeWidget::item{height: 30px}\
    QTreeView::item::selected{background-color:#96B5A8}\
	QTreeWidget::branch:closed:has-children:!has-siblings{border-style: none; border-image: none; image: url(:/res/ico/unfold.png);}\
	QTreeWidget::branch:closed:has-children:has-siblings{border-image: none; image: url(:/res/ico/unfold.png);}\
	QTreeWidget::branch:open:has-children:!has-siblings{border-image: none; image: url(:/res/ico/fold.png);}\
	QTreeWidget::branch:open:has-children:has-siblings{border-image: none; image: url(:/res/ico/fold.png);}"
	"QScrollBar:vertical{" //���ù�������ʽ
	"background:#FFFFF2;"  //����ɫ  
	"padding-top:20px;"    //��Ԥ��λ��(�������ϼ�ͷ��  
	"padding-bottom:20px;" //��Ԥ��λ��(�������¼�ͷ��  
	"padding-left:2px;"    //��Ԥ��λ�� 
	"padding-right:2px;"   //��Ԥ��λ�� 
	"border-left:1px solid #d7d7d7;}" //��ָ���  
	"QScrollBar::handle:vertical{" //������ʽ  
	"background:#dbdbdb;"  //������ɫ  
	"border-radius:6px;"   //�߽�Բ
	"min-height:20px;}"    //������С�߶�
	"QScrollBar::handle:vertical:hover{" //��괥��������ʽ  
	"background:#d0d0d0;}" //������ɫ  
	"QScrollBar::add-line:vertical{" //���¼�ͷ��ʽ  
	"background:url(:/res/ico/down.png) center no-repeat;}"
	"QScrollBar::sub-line:vertical{" //���ϼ�ͷ��ʽ
	"background:url(:/res/ico/up.png) center no-repeat;}");
	//�������弰�����С
	QFont font("Microsoft YaHei");
	font.setPixelSize(16.5);
	ToolTreeWidget->setFont(font);
	ToolTreeWidget->header()->setVisible(false);
	ToolTreeWidget->clear();
	this->setAcceptDrops(false);
	//�����б�
	ToolsPair toolPair;	
	//ͼ����
	LoadPlugins("ͼ����");  //����ͼ����	
	toolPair.first = "ͼ����";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//���ʶ��
	LoadPlugins("���ʶ��");  //���ؼ��ʶ��		
	toolPair.first = "���ʶ��";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//�궨����
	LoadPlugins("�궨����");  //���ر궨����	
	toolPair.first = "�궨����";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//��λ����
	LoadPlugins("��λ����");  //���ض�λ����		
	toolPair.first = "��λ����";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//���ι���
	LoadPlugins("���ι���");  //���ؼ��ι���
	toolPair.first = "���ι���";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//���β���
	LoadPlugins("���β���");  //���ؼ��β���	
	toolPair.first = "���β���";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//��ά���	
	LoadPlugins("��ά���");  //������ά���
	toolPair.first = "��ά���";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//�߼�����
	LoadPlugins("�߼�����");  //�����߼�����	
	toolPair.first = "�߼�����";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//ͨѶ����
	LoadPlugins("ͨѶ����");  //����ͨѶ����	
	toolPair.first = "ͨѶ����";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//ϵͳ����
	LoadPlugins("ϵͳ����");  //����ϵͳ����	
	toolPair.first = "ϵͳ����";
	toolPair.second = ToolNameList;
	ToolNamesVec.push_back(toolPair);
	ToolNameList.clear();
	//ѭ������ͼ��	
	for (int i = 0; i < ToolNamesVec.size(); i++)
	{
		QString toolBoxName = ToolNamesVec[i].first;
		QStringList toolList = QStringList::fromStdList(ToolNamesVec[i].second);
		QTreeWidgetItem* itemParent = new QTreeWidgetItem(ToolTreeWidget);
		itemParent->setText(0, toolBoxName);
		itemParent->setIcon(0, QIcon(":/res/ico/tool.ico"));
		itemParent->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
		for (int j = 0; j < toolList.count(); j++)
		{
			QTreeWidgetItem* treeItem = new QTreeWidgetItem(itemParent, QStringList(toolList[j]));
			QPixmap pixmap = GetIcon(GetIconName(toolList[j]));
			treeItem->setIcon(0, pixmap);
		}
	}
}

/*
 * @function PluginsApp::LoadPlugins ���ز�����������Plugins�ļ�����
 * @return ���ز���ĸ���
 */
int frmToolsTree::LoadPlugins(QString str)
{
	int count = 0;
	QDir pluginsDir = QDir(qApp->applicationDirPath());
	if (!pluginsDir.cd("Plugins")) return -1;
	QStringList filters;
	filters << "*.dll"; //���Զ��ָ�ʽ���ߵ�����ʽ
	pluginsDir.setFilter(QDir::Files | QDir::NoSymLinks); //�������͹�������ֻΪ�ļ���ʽ
	pluginsDir.setNameFilters(filters);  //�����ļ����ƹ�����
	foreach(QString fileName, pluginsDir.entryList())
	{
		QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
		QObject* plugin = pluginLoader.instance();
		if (plugin)
		{
			auto treeInterface = qobject_cast<MainInterface*>(plugin);
			if (treeInterface)
			{
				++count;
				//���ز��������Tree
				PopulateTree(str, plugin, treeInterface);
			}
		}
	}
	return count;
}

/*
 * @function PluginsApp::PopulateTree ���ݲ������Tree
 * @param pluginInterface ���
 * @param i ���ʵ�ֵĽӿ�
 */
void frmToolsTree::PopulateTree(QString str, QObject* pluginInterface, MainInterface* i)
{
	try
	{
		if (i->information() == str)
		{
			ToolNameList.push_back(i->name());
		}
	}
	catch(...){}	
}

// ��ȡͼ��
QPixmap frmToolsTree::GetIcon(QString fileName)
{
	return IconHelper::Instance()->getPixmap(fileName);
}

// ��ȡͼ��·��
QString frmToolsTree::GetIconName(QString Name)
{
	QString IconName;
	if (Name.contains("��ȡͼ��")) IconName = ":/res/ico/image_source.png";
	if (Name.contains("ͼ����ʾ")) IconName = ":/res/ico/image_view.ico";
	if (Name.contains("����ͼ��")) IconName = ":/res/ico/export_image.png";	
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
	if (Name.contains("�ߵ����")) IconName = ":/res/ico/blob.png";
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
	if (Name.contains("Ѱ��Բ")) IconName = ":/res/ico/find_circle.png";
	if (Name.contains("Ѱ��ֱ��")) IconName = ":/res/ico/find_line.png";
	if (Name.contains("���Բ")) IconName = ":/res/ico/fit_circle.png";
	if (Name.contains("�����Բ")) IconName = ":/res/ico/fit_ellipse.png";
	if (Name.contains("���ֱ��")) IconName = ":/res/ico/fit_line.png";
	if (Name.contains("��ȡ�߽��")) IconName = ":/res/ico/border_point.png";
	if (Name.contains("��Բ����")) IconName = ":/res/ico/line_circle.png";
	if (Name.contains("��+��")) IconName = ":/res/ico/point_point.png";
	if (Name.contains("��+��")) IconName = ":/res/ico/point_l.png";
	if (Name.contains("���߽���")) IconName = ":/res/ico/intersection.png";
	if (Name.contains("����Բȱ��")) IconName = ":/res/ico/rounded_c.png";	
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
