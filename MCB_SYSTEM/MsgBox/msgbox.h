#ifndef MSGBOX_H
#define MSGBOX_H

#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>

#include <QWidget>

class MsgBox : public QDialog
{
    Q_OBJECT
public:
    explicit MsgBox(QWidget *parent = nullptr);
    //
    void setTitle(QString title);
    void setText(QString text);
    void setOK_Button(QString text);
    void setCANCEL_Button(QString text);
public:
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QLabel * titleLabel;
    QLabel *askLabel;
protected:
    QPoint move_point;                                    //移动的距离
    bool mouse_press;                                    //鼠标按下
    void mousePressEvent(QMouseEvent *qevent);            //鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *qevent);         //鼠标释放事件
    void mouseMoveEvent(QMouseEvent *qevent);             //鼠标移动事件
public slots:
    void okBtn_press();
    void cancelBtn_press();


};

#endif // MSGBOX_H
