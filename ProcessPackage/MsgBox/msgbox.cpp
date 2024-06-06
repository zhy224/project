#include "msgbox.h"

#if _MSC_VER>=1600
#pragma execution_character_set("utf-8");
#endif

static int msg_width = 0;
static int msg_height = 0;

MsgBox::MsgBox(QWidget *parent) : QDialog(parent)
{
    this->resize(400,200);

    // 获取主界面的宽度
    msg_width = this->width();
    msg_height = this->height();

    setStyleSheet("QWidget{background-color: rgb(255,255,255);border:1px solid;border-color: rgb(52,152,219);}"); // rgb(219,229,245)

    // 设置字体大小
    QFont font;
    font.setFamily("微软雅黑");
    font.setPointSize(13);
    // 设置标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    titleLabel = new QLabel(this);
    titleLabel->setFont(font);
    titleLabel->setGeometry(0,0,msg_width,msg_height/6);
    titleLabel->setStyleSheet("QLabel{font-family :Times New Roman;font-size : 5em;color : rgb(255,255,255);background-color: rgb(52,152,219);}");
    // 设置显示文本
    askLabel = new QLabel(this);
    askLabel->setFont(font);
    // setGeometry(x,y,width,height)(左边距，上边距，宽度，高度)
    askLabel->setGeometry(QRect(msg_width/6, msg_height/4, msg_width/3*2, msg_height/5*2));
    // 自动换行(纯字母/纯数字不支持换行)
    askLabel->setWordWrap(true);
    askLabel->setAlignment(Qt::AlignCenter);
    askLabel->setStyleSheet("border:0;");
    // ok按钮
    okBtn=new QPushButton(this);
    okBtn->setFont(font);
    okBtn->setGeometry(msg_width/5,msg_height/4*3,msg_width/5,msg_height/6);
    //okBtn->setText(text);
    okBtn->setStyleSheet("QPushButton{border:1px groove gray;border-radius:1px;border-color: rgb(52,152,219);}"
                         "QPushButton:hover{background-color: rgb(52,152,219);}"
                         "QPushButton:pressed{background-color: rgb(52,152,219);}");
    // cancel按钮
    cancelBtn = new QPushButton(this);
    cancelBtn->setFont(font);
    cancelBtn->setGeometry(msg_width/5*3,msg_height/4*3,msg_width/5,msg_height/6);
    //cancelBtn->setText(text);
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
    okBtn->setGeometry(msg_width/5*2,msg_height/4*3,msg_width/5,msg_height/6);
    cancelBtn->hide();
    cancelBtn->setEnabled(false);
}
void MsgBox::setCANCEL_Button(QString text)
{
    cancelBtn->setText(text);
    // 同时设置ok按钮
    okBtn->setGeometry(msg_width/5,msg_height/4*3,msg_width/5,msg_height/6);
    cancelBtn->show();
    cancelBtn->setEnabled(true);
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

int MsgBox::timerQBox(QString title,QString text,QString leftText,QString rightText,int s)
{
    MsgBox *msgBox = new MsgBox();
    msgBox->setTitle(title);
    msgBox->setText(text);
    msgBox->setOK_Button(leftText);
    msgBox->setCANCEL_Button(rightText);
    int resIndex = 0;
    int countDown = s-1;
    QTimer timer;
    timer.start(1000);
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (countDown > 0) {
            msgBox->setText(QString("请在%1s内确认!").arg(countDown));
            countDown--;
        } else {
            timer.stop();
            msgBox->close();

        }
    });
    int res = msgBox->exec();
    switch(res)
    {
        case QDialog::Accepted:	// 确定
            resIndex = 1;
            timer.stop();
            msgBox->close();
            break;
        case QDialog::Rejected:	// 取消
            resIndex = 2;
            timer.stop();
            msgBox->close();
            break;
    }
    delete msgBox;

    return resIndex;
}
bool MsgBox::askQBox(QString title,QString text,QString leftText,QString rightText)
{
    MsgBox *msgBox = new MsgBox();
    msgBox->setTitle(title);
    msgBox->setText(text);
    msgBox->setOK_Button(leftText);
    msgBox->setCANCEL_Button(rightText);
    bool resIndex = false;
    int res = msgBox->exec();
    switch(res)
    {
        case QDialog::Accepted:	// 确定
            resIndex = true;
            msgBox->close();
            break;
        case QDialog::Rejected:	// 取消
            resIndex = false;
            msgBox->close();
            break;
    }
    delete msgBox;

    return resIndex;
}
int MsgBox::confirmQBox(QString title,QString text,QString okText)
{
    MsgBox *msgBox = new MsgBox();
    msgBox->setTitle(title);
    msgBox->setText(text);
    msgBox->setOK_Button(okText);

    int resIndex = 0;

    int res = msgBox->exec();
    if(res == QDialog::Accepted)
    {
        resIndex = 1;
        msgBox->close();
    }
    delete msgBox;

    return resIndex;
}

