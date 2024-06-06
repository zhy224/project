#include "msgbox.h"

MsgBox::MsgBox(QWidget *parent) : QDialog(parent)
{
    this->resize(332,167);

    // 获取主界面的宽度
    int width = this->width();
    setStyleSheet("QWidget{background-color: rgb(255,255,255);}"); // rgb(219,229,245)

    // 设置字体大小
    QFont font;
    font.setFamily("微软雅黑");
    font.setPointSize(12);
    // 设置标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    titleLabel = new QLabel(this);
    titleLabel->setFont(font);
    titleLabel->setGeometry(0,0,width,30);

    titleLabel->setStyleSheet("QLabel{font-family :Times New Roman;font-size : 5em;color : rgb(255,255,255);background-color: rgb(52,152,219);}");

    askLabel = new QLabel(this);
    askLabel->setFont(font);
    //    askLabel->setGeometry(133,56,width,50);
        // 自动换行
    askLabel->setGeometry(QRect(80, 56, width/3*2, 27*4)); //四倍行距
    askLabel->setWordWrap(true);
    askLabel->setAlignment(Qt::AlignTop);


    okBtn=new QPushButton(this);
    okBtn->setFont(font);
    okBtn->setGeometry(90,119,85,30);
    //        okBtn->setGeometry(135,119,85,30);
    //okBtn->setText(tr("OK"));
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:1px;border-color: rgb(52,152,219);}"
                         "QPushButton:hover{background-color: rgb(52,152,219);}"
                         "QPushButton:pressed{background-color: rgb(52,152,219);}");

    cancelBtn = new QPushButton(this);
    cancelBtn->setFont(font);
    cancelBtn->setGeometry(187,119,85,30);
    //cancelBtn->setText(tr("Cancel"));
    cancelBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:1px;border-color: rgb(52,152,219);}"
                             "QPushButton:hover{background-color: rgb(52,152,219);}"
                             "QPushButton:pressed{background-color: rgb(52,152,219);}");

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_press()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_press()));

}

void MsgBox::setTitle(QString title)
{
    titleLabel->setText(title);
}

void MsgBox::setText(QString text)
{
    askLabel->setText(text);
}
void MsgBox::setOK_Button(QString text)
{
    okBtn->setText(text);
}
void MsgBox::setCANCEL_Button(QString text)
{
    cancelBtn->setText(text);
}
void MsgBox::mousePressEvent(QMouseEvent *qevent)
{
    if(qevent->button() == Qt::LeftButton)
    {
        mouse_press = true;
        // 鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = qevent->pos();;
    }
}
void MsgBox::mouseMoveEvent(QMouseEvent *qevent)
{
    // 若鼠标左键被按下
    if(mouse_press)
    {
        // 鼠标相对于屏幕的位置
        QPoint move_pos = qevent->globalPos();

        // 移动主窗体位置
        this->move(move_pos - move_point);
    }
}
void MsgBox::mouseReleaseEvent(QMouseEvent *)
{
    // 设置鼠标为未被按下
    mouse_press = false;
}

void MsgBox::okBtn_press()
{
    this->accept();
}
void MsgBox::cancelBtn_press()
{
    this->reject();
}

