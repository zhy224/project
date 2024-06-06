#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "Public/cn.h"
#include "widget/deploy.h"
#include "widget/proparset.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    Deploy *deploy;
    proParSet *proSet;

private:
    Ui::MainWindow *ui;
private:
    // 弹出框
    MsgBox *msgBox;
    // 界面map
    QMap<DT::CHANGED_PAGE,QWidget*> widgetMap;
    //
    void main_init();
    //
    void pages(QString upIcon, QString downIcon, DT::CHANGED_PAGE index);
signals:
    void pageChanged(DT::CHANGED_PAGE page);
private slots:
    // 界面切换
    void onPageChanged(DT::CHANGED_PAGE index);

protected:
    void changeEvent(QEvent *) override;
    void closeEvent(QCloseEvent *) override;

};
#endif // MAINWINDOW_H
