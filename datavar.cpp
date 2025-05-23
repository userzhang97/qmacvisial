#include "datavar.h"
#include <QDateTime>

//静态数据成员的定义 
QString dataVar::soft_key;
QWidget* dataVar::p_MainWidget = nullptr;
MainWindow* dataVar::m_pWindow = nullptr;
FrmImageView* dataVar::fImageView = nullptr;
frmProItemTab* dataVar::fProItemTab = nullptr;
frmProcessSetUp* dataVar::fProcessSetUp = nullptr;
frmSystemSetUp* dataVar::fSystemSetUp = nullptr;
frmImageLayout* dataVar::fImageLayout = nullptr;
frmGlobalVariable* dataVar::fGlobalVariable = nullptr;
frmCameraSetUp* dataVar::fCameraSetUp = nullptr;
frmInstrumentation* dataVar::fInstrumentation = nullptr;
FrmLog* dataVar::fLog = nullptr;
QMap <QString, QtDragListWidget*> dataVar::FlowProMap;
QVector<QString> dataVar::camera_state;
QVector<QString> dataVar::io_client_ip;
QVector<QString> dataVar::io_client_port;
QVector<QString> dataVar::mit_plc_client_ip;
QVector<QString> dataVar::mit_plc_client_port;
QVector<QString> dataVar::tcp_client_ip;
QVector<QString> dataVar::tcp_client_port;
manualThread dataVar::manualCycleThread;
autoThread_B1 dataVar::autoCycleThread_B1;
autoThread_B2 dataVar::autoCycleThread_B2;
autoThread_B3 dataVar::autoCycleThread_B3;
autoThread_B4 dataVar::autoCycleThread_B4;
autoThread_B5 dataVar::autoCycleThread_B5;
autoThread_B6 dataVar::autoCycleThread_B6;
autoThread_B7 dataVar::autoCycleThread_B7;
autoThread_B8 dataVar::autoCycleThread_B8;
autoThread_B9 dataVar::autoCycleThread_B9;
autoThread_B10 dataVar::autoCycleThread_B10;
autoThread_B11 dataVar::autoCycleThread_B11;
autoThread_B12 dataVar::autoCycleThread_B12;
autoThread_B13 dataVar::autoCycleThread_B13;
autoThread_B14 dataVar::autoCycleThread_B14;
autoThread_B15 dataVar::autoCycleThread_B15;
autoThread_B16 dataVar::autoCycleThread_B16;
autoThread_B17 dataVar::autoCycleThread_B17;
autoThread_B18 dataVar::autoCycleThread_B18;
autoThread_B19 dataVar::autoCycleThread_B19;
autoThread_B20 dataVar::autoCycleThread_B20;
int dataVar::stopThread_A = 0;
int dataVar::stopThread_B1 = 0;
int dataVar::stopThread_B2 = 0;
int dataVar::stopThread_B3 = 0;
int dataVar::stopThread_B4 = 0;
int dataVar::stopThread_B5 = 0;
int dataVar::stopThread_B6 = 0;
int dataVar::stopThread_B7 = 0;
int dataVar::stopThread_B8 = 0;
int dataVar::stopThread_B9 = 0;
int dataVar::stopThread_B10 = 0;
int dataVar::stopThread_B11 = 0;
int dataVar::stopThread_B12 = 0;
int dataVar::stopThread_B13 = 0;
int dataVar::stopThread_B14 = 0;
int dataVar::stopThread_B15 = 0;
int dataVar::stopThread_B16 = 0;
int dataVar::stopThread_B17 = 0;
int dataVar::stopThread_B18 = 0;
int dataVar::stopThread_B19 = 0;
int dataVar::stopThread_B20 = 0;
int dataVar::m_a_state = 0;
QString dataVar::pageState;
QString dataVar::user_name;
int dataVar::login_state = 0;
int dataVar::admin_login_state = 0;
int dataVar::auto_table;
QSqlDatabase dataVar::database;
SQLite dataVar::sqlite;
bool dataVar::bol_connect = false;
XML dataVar::xml;
int dataVar::img_choose;
bool dataVar::runFinish_A = false;
bool dataVar::runFinish_B1 = false;
bool dataVar::runFinish_B2 = false;
bool dataVar::runFinish_B3 = false;
bool dataVar::runFinish_B4 = false;
bool dataVar::runFinish_B5 = false;
bool dataVar::runFinish_B6 = false;
bool dataVar::runFinish_B7 = false;
bool dataVar::runFinish_B8 = false;
bool dataVar::runFinish_B9 = false;
bool dataVar::runFinish_B10 = false;
bool dataVar::runFinish_B11 = false;
bool dataVar::runFinish_B12 = false;
bool dataVar::runFinish_B13 = false;
bool dataVar::runFinish_B14 = false;
bool dataVar::runFinish_B15 = false;
bool dataVar::runFinish_B16 = false;
bool dataVar::runFinish_B17 = false;
bool dataVar::runFinish_B18 = false;
bool dataVar::runFinish_B19 = false;
bool dataVar::runFinish_B20 = false;
int dataVar::img_view_count_B1 = 0;
int dataVar::img_view_count_buf_B1 = 0;
int dataVar::img_view_count_B2 = 0;
int dataVar::img_view_count_buf_B2 = 0;
int dataVar::img_view_count_B3 = 0;
int dataVar::img_view_count_buf_B3 = 0;
int dataVar::img_view_count_B4 = 0;
int dataVar::img_view_count_buf_B4 = 0;
int dataVar::img_view_count_B5 = 0;
int dataVar::img_view_count_buf_B5 = 0;
int dataVar::img_view_count_B6 = 0;
int dataVar::img_view_count_buf_B6 = 0;
int dataVar::img_view_count_B7 = 0;
int dataVar::img_view_count_buf_B7 = 0;
int dataVar::img_view_count_B8 = 0;
int dataVar::img_view_count_buf_B8 = 0;
int dataVar::img_view_count_B9 = 0;
int dataVar::img_view_count_buf_B9 = 0;
int dataVar::img_view_count_B10 = 0;
int dataVar::img_view_count_buf_B10 = 0;
int dataVar::img_view_count_B11 = 0;
int dataVar::img_view_count_buf_B11 = 0;
int dataVar::img_view_count_B12 = 0;
int dataVar::img_view_count_buf_B12 = 0;
int dataVar::img_view_count_B13 = 0;
int dataVar::img_view_count_buf_B13 = 0;
int dataVar::img_view_count_B14 = 0;
int dataVar::img_view_count_buf_B14 = 0;
int dataVar::img_view_count_B15 = 0;
int dataVar::img_view_count_buf_B15 = 0;
int dataVar::img_view_count_B16 = 0;
int dataVar::img_view_count_buf_B16 = 0;
int dataVar::img_view_count_B17 = 0;
int dataVar::img_view_count_buf_B17 = 0;
int dataVar::img_view_count_B18 = 0;
int dataVar::img_view_count_buf_B18 = 0;
int dataVar::img_view_count_B19 = 0;
int dataVar::img_view_count_buf_B19 = 0;
int dataVar::img_view_count_B20 = 0;
int dataVar::img_view_count_buf_B20 = 0;
bool dataVar::app_state = false;
bool dataVar::form_max_state = true;
bool dataVar::process_self_start = false;
bool dataVar::auto_login_out = false;
QDateTime dataVar::startTime;
QString dataVar::path_P = QDir::currentPath() + "/Parameters/";
QString dataVar::path_PF = QDir::currentPath() + "/Parameters/Formula/";
QString dataVar::path_L = QDir::currentPath() + "/Log/";
QString dataVar::path_LA = QDir::currentPath() + "/Log/AppLog/";
QString dataVar::path_Image = QDir::currentPath() + "/Image/";
QString dataVar::path_DB = QDir::currentPath() + "/Database/";
QGraphicsViews* dataVar::img_view_B1 = nullptr;
QGraphicsViews* dataVar::img_view_B2 = nullptr;
QGraphicsViews* dataVar::img_view_B3 = nullptr;
QGraphicsViews* dataVar::img_view_B4 = nullptr;
QGraphicsViews* dataVar::img_view_B5 = nullptr;
QGraphicsViews* dataVar::img_view_B6 = nullptr;
QGraphicsViews* dataVar::img_view_B7 = nullptr;
QGraphicsViews* dataVar::img_view_B8 = nullptr;
QGraphicsViews* dataVar::img_view_B9 = nullptr;
QGraphicsViews* dataVar::img_view_B10 = nullptr;
QGraphicsViews* dataVar::img_view_B11 = nullptr;
QGraphicsViews* dataVar::img_view_B12 = nullptr;
QGraphicsViews* dataVar::img_view_B13 = nullptr;
QGraphicsViews* dataVar::img_view_B14 = nullptr;
QGraphicsViews* dataVar::img_view_B15 = nullptr;
QGraphicsViews* dataVar::img_view_B16 = nullptr;
QGraphicsViews* dataVar::img_view_B17 = nullptr;
QGraphicsViews* dataVar::img_view_B18 = nullptr;
QGraphicsViews* dataVar::img_view_B19 = nullptr;
QGraphicsViews* dataVar::img_view_B20 = nullptr;
QGraphicsViews* dataVar::img_view1 = nullptr;
QGraphicsViews* dataVar::img_view2 = nullptr;
QGraphicsViews* dataVar::img_view3 = nullptr;
QGraphicsViews* dataVar::img_view4 = nullptr;
QGraphicsViews* dataVar::img_view5 = nullptr;
QGraphicsViews* dataVar::img_view6 = nullptr;
QGraphicsViews* dataVar::img_view7 = nullptr;
QGraphicsViews* dataVar::img_view8 = nullptr;
QGraphicsViews* dataVar::img_view9 = nullptr;
//项目名称
QString dataVar::projectName = QString();
int dataVar::int_link;
QString dataVar::str_link;
QtWidgetsSaveLoad* dataVar::qtWidgetsSaveLoad;
int dataVar::dragNumber = 0;
//计算耗时
int dataVar::nCostTime_B1 = 0;
int dataVar::nCostTime_B2 = 0;
int dataVar::nCostTime_B3 = 0;
int dataVar::nCostTime_B4 = 0;
int dataVar::nCostTime_B5 = 0;
int dataVar::nCostTime_B6 = 0;
int dataVar::nCostTime_B7 = 0;
int dataVar::nCostTime_B8 = 0;
int dataVar::nCostTime_B9 = 0;
int dataVar::nCostTime_B10 = 0;
int dataVar::nCostTime_B11 = 0;
int dataVar::nCostTime_B12 = 0;
int dataVar::nCostTime_B13 = 0;
int dataVar::nCostTime_B14 = 0;
int dataVar::nCostTime_B15 = 0;
int dataVar::nCostTime_B16 = 0;
int dataVar::nCostTime_B17 = 0;
int dataVar::nCostTime_B18 = 0;
int dataVar::nCostTime_B19 = 0;
int dataVar::nCostTime_B20 = 0;
//多线程自动运行一次
short dataVar::manualRunOnce_A = 0;
short dataVar::autoRunOnce_B1 = 0;
short dataVar::autoRunOnce_B2 = 0;
short dataVar::autoRunOnce_B3 = 0;
short dataVar::autoRunOnce_B4 = 0;
short dataVar::autoRunOnce_B5 = 0;
short dataVar::autoRunOnce_B6 = 0;
short dataVar::autoRunOnce_B7 = 0;
short dataVar::autoRunOnce_B8 = 0;
short dataVar::autoRunOnce_B9 = 0;
short dataVar::autoRunOnce_B10 = 0;
short dataVar::autoRunOnce_B11 = 0;
short dataVar::autoRunOnce_B12 = 0;
short dataVar::autoRunOnce_B13 = 0;
short dataVar::autoRunOnce_B14 = 0;
short dataVar::autoRunOnce_B15 = 0;
short dataVar::autoRunOnce_B16 = 0;
short dataVar::autoRunOnce_B17 = 0;
short dataVar::autoRunOnce_B18 = 0;
short dataVar::autoRunOnce_B19 = 0;
short dataVar::autoRunOnce_B20 = 0;
QVector<DragListWidgetPrivate*> dataVar::dragD = QVector<DragListWidgetPrivate*>(20);
QMap<int, int> dataVar::all_link_process;
int dataVar::comm_state_buf = 0;
