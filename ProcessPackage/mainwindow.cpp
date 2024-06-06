#include "mainwindow.h"
#include "ui_mainwindow.h"

#if _MSC_VER>=1600
#pragma execution_character_set("utf-8");
#endif

static QList<QToolButton*> toolButtons;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/Resource/icon/ic_mainwindow.ico"));
    // 初始化
    main_init();

    //
    deploy = new Deploy();
    widgetMap.insert(DT::PAGE_DEPLOY, deploy);
    pages(":/Resource/icon/ic_settings1_f.png", ":/Resource/icon/ic_settings1_t.png",  DT::PAGE_DEPLOY);
    ui->stackedWidget->addWidget(deploy);
    //
    proSet = new proParSet();
    widgetMap.insert(DT::PAGE_PRO_PAR, proSet);
    pages(":/Resource/icon/ic_proParData1_f.png", ":/Resource/icon/ic_proParData1_t.png",  DT::PAGE_PRO_PAR);
    ui->stackedWidget->addWidget(proSet);

    // 切换界面
    connect(this, &MainWindow::pageChanged, this, &MainWindow::onPageChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::main_init()
{
    toolButtons = ui->toolBar->findChildren<QToolButton*>();
    toolButtons.first()->setChecked(true);
    toolButtons.first()->setIcon(QIcon(":/Resource/icon/ic_settings1_t.png"));
}
/**
 * @brief 添加界面信号绑定
 * @param page 界面序号
 */
void MainWindow::pages(QString upIcon, QString downIcon, DT::CHANGED_PAGE page)
{
    // 获取指定元素
    QToolButton *button = toolButtons.at(page);
    // 信号绑定(toggled状态切换后才会发送信号，clicke只要点击就发送信号，未等状态切换)
    connect(button, &QToolButton::toggled, [=](bool checked) {

        if(checked) {
            emit pageChanged(page);
            button->setIcon(QIcon(downIcon));
        }
        else {
            button->setIcon(QIcon(upIcon));
        }
    });


}
/**
 * @brief 界面切换
 * @param index 界面序号
 */
void MainWindow::onPageChanged(DT::CHANGED_PAGE index)
{
    //qDebug()<<"DT::CHANGED_PAGE:"<<index;
    // 获取当前键的值（窗口类型）
    QWidget* widget = widgetMap.value(index);
    // 跳转到窗口
    ui->stackedWidget->setCurrentWidget(widget);
}
/**
 * 重写界面事件（语言改变）
 */
void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        this->ui->retranslateUi(this);
    }
}
/**
 * 重写界面事件（语言改变）
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    int res = msgBox->askQBox(tr("关闭程序提示"),tr("是否关闭程序？"),tr("是"),tr("否"));
    if(res == 1)
    {
        //若用户点击确认，则接收这个事件,当前窗口会关闭
        event->accept();
    }
    else
    {
        //若用户点击取消，则忽略这个事件，当前窗口不会关闭
        event->ignore();
    }
}

