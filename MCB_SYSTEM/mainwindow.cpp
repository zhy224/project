#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 通信类
    cn = new CN();

    // 网络连接界面
    netConnect = new NetworkConnect(this);
    widgetMap.insert(DT::PAGE_S_NET_CONNECT, netConnect);
    ui->stackedWidget->addWidget(netConnect);
    // 主控测试界面
    mcb_test = new MCB_Test(this);
    widgetMap.insert(DT::PAGE_S_MCB_TEST, mcb_test);
    ui->stackedWidget->addWidget(mcb_test);




}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 界面切换
 * @param index 界面序号
 */
void MainWindow::onPageChanged(DT::CHANGED_PAGE index)
{
    // 获取当前键的值（窗口类型）
    QWidget* widget = widgetMap.value(index);
    // 跳转到窗口
    ui->stackedWidget->setCurrentWidget(widget);
    // 改变按钮颜色
    //this->styleSheet("background:color;);
}


/**
 *
 * 网络连接按钮
 */
void MainWindow::on_toolButton_NetWork_clicked()
{
    onPageChanged(DT::PAGE_S_NET_CONNECT);
}
/**
 *
 * 主控测试按钮
 */
void MainWindow::on_toolButton_MCB_test_clicked()
{
    onPageChanged(DT::PAGE_S_MCB_TEST);
}
