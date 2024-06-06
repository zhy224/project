#include "navigationbar.h"
#include "ui_navigationbar.h"

#pragma execution_character_set("utf-8")

NavigationBar::NavigationBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavigationBar)
{
    ui->setupUi(this);

    uiInit();

    loadStyle();
}

NavigationBar::~NavigationBar()
{
    delete ui;
}

/**
 * @brief ui初始化
 */
void NavigationBar::uiInit()
{
    this->setAttribute(Qt::WA_StyledBackground);
    // 布局
    ui->buttonLayout->setAlignment(Qt::AlignTop);
    //
    this->setMinimumWidth(108);
}

/**
 * @brief 加载样式表
 */
void NavigationBar::loadStyle()
{
    QFile file(":/styleSheet/navigationbarStyle.css");
    if (!file.exists()) {
        qDebug() << "菜单栏界面加载样式表失败,样式表navigationbarStyle不存在.";
        return;
    }
    bool ret = file.open(QIODevice::ReadOnly);
    if (!ret) {
        qDebug() << "菜单栏界面加载样式表失败,样式表navigationbarStyle打开失败.";
        return;
    }
    QString style = QLatin1String(file.readAll());
    file.close();
    this->setStyleSheet(style);
}

/**
 * @brief 插入bar
 * @param index     序号
 * @param name      名称
 * @param upIcon    按钮弹起图标
 * @param downIcon  按钮按下图标
 * @param page      按钮对应界面
 */
void NavigationBar::insertBar(int index, QString name, QString upIcon, QString downIcon,DT::CHANGED_PAGE page)
{
    QToolButton *button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    button->setObjectName(QString("bar_%1").arg(index));
    button->setText(name);
    button->setIconSize(QSize(32, 32));
    button->setIcon(QIcon(upIcon));
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setCheckable(true);
    button->setAutoExclusive(true);
    // 添加到布局
    ui->buttonLayout->insertWidget(index, button);
    // 信号绑定
    connect(button, &QToolButton::toggled, [=](bool checked) {
        if(checked) {
            button->setIcon(QIcon(downIcon));
            emit pageChanged(page);
        }
        else {
            button->setIcon(QIcon(upIcon));
        }
    });
}
