﻿#include "showmain.h"
#include "ui_showmain.h"
#include<QMenu>
#include<QCloseEvent>
#include<QMessageBox>
#include<QApplication>
#include<QDesktopWidget>
#include<QColor>
#include<QBrush>
#include<QFileDialog>
#include<QTime>
#include<QMouseEvent>




ShowMain::ShowMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowMain)
{
    ui->setupUi(this);

    /*隐藏窗口*/
    this->setWindowFlags(Qt::FramelessWindowHint);

    /*添加系统托盘*/
    trayIcon = new QSystemTrayIcon(QIcon(":/icons8_Slack.ico"),this);
    trayIcon->setToolTip(tr("截图-----圣徒"));
    QMenu *menu = new QMenu;//添加托盘菜单
    menu->addAction(tr("退出"),qApp,SLOT(quit()));
    trayIcon->setContextMenu(menu);
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),\
            this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    //取得屏幕大小，初始化 cutScreen
    cutScreen = new MyScreen(QApplication::desktop()->size());
    resize(cutScreen->width(),cutScreen->height());

    //保存全屏
    this->hide();
    fullScreen = new QPixmap();
    *fullScreen = QPixmap::grabWindow(QApplication::desktop()->winId(),\
                                      0,0,cutScreen->width(),cutScreen->height());

    //设置透明度实现模糊背景
    QPixmap pix(cutScreen->width(),cutScreen->height());
    pix.fill((QColor(160,160,165,192)));
    bgScreen = new QPixmap(*fullScreen);
    QPainter p(bgScreen);
    p.drawPixmap(0,0,pix);

    //截图信息显示区域背景
    infoPix = new QPixmap(WIDTH_SHOW,HEIGHT_SHOW);
    QPainter infoP(infoPix);
    infoP.setBrush(QBrush(QColor(Qt::black),Qt::SolidPattern));
    infoP.drawRect(0,0,WIDTH_SHOW,HEIGHT_SHOW);


    //展示初始截屏界面
    this->show();
}

ShowMain::~ShowMain()
{
    delete ui;
}

void ShowMain::paintEvent(QPaintEvent *e)
{
    int x = cutScreen->getLeftUp().x();
    int y = cutScreen->getLeftUp().y();
    int w = cutScreen->getRightDown().x()-x;
    int h = cutScreen->getRightDown().y()-y;

    QPainter painter(this);
    QPen pen; pen.setColor(Qt::green); pen.setWidth(1); pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawPixmap(0,0,*bgScreen);  //画模糊背景
    if( w!=0 && h!=0 )    painter.drawPixmap(x,y,fullScreen->copy(x,y,w,h));  //画截取区域
    painter.drawRect(x,y,w,h);  //截取区域边框

    //显示截取区域信息 width height
    painter.drawPixmap(x,y-32,*infoPix);
    painter.drawText(x+2,y-20,QString("截图范围：(%1 x %2)-(%3 x %4) 图片大小：(%5 x %6)").arg(x).arg(y).arg(x+w).arg(y+h).arg(w).arg(h));
    painter.drawText(x+2,y-6,QString("使用说明：(%1)").arg("enter保存图片,F全屏截图,ESC退出程序"));
}

//截取全屏
void ShowMain::saveFullScreen()
{
    fullScreen->save("fullScreen.jpg","JPG");
}

//保存截取区域
void ShowMain::saveScreen()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString str = QDateTime().currentDateTime().toString("yyyyMMddHHmmss");
    QString fileName = str;
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("保存图片"),
                                            fileName,
                                            tr("JPEG Files (*.jpg);;JPEG (*.jpg)"),
                                            &selectedFilter,
                                            options);

    int x = cutScreen->getLeftUp().x();
    int y = cutScreen->getLeftUp().y();
    int w = cutScreen->getRightDown().x()-x;
    int h = cutScreen->getRightDown().y()-y;

    fullScreen->copy(x,y,w,h).save(fileName,"JPG");
}

//按键功能
void ShowMain::keyReleaseEvent(QKeyEvent *e )
{
    if( e->key()==Qt::Key_Escape )// esc 退出
    {
        this->hide();
    }
    else if( e->key()==Qt::Key_F )// f 截取全屏
    {
        saveFullScreen();
        this->hide();
    }
    else if( e->key()==Qt::Key_Return||e->key()==Qt::Key_Enter)//回车 保存截取区域
    {
        saveScreen();
        this->hide();
    }
}

//鼠标移动在 SELECT MOV 两种状态下
void ShowMain::mouseMoveEvent(QMouseEvent *e)
{
    if( cutScreen->getStatus()==SELECT ) // 选择区域
    {
        cutScreen->setEnd( e->pos() );
    }
    else if( cutScreen->getStatus()==MOV ) //移动所选区域
    {
        QPoint p(e->x()-movPos.x(),e->y()-movPos.y());
        cutScreen->move(p);
        movPos = e->pos();
    }
    update();
}

//按下鼠标，记录初值
void ShowMain::mousePressEvent(QMouseEvent *e)
{
    int status = cutScreen->getStatus();

    if( status==SELECT ) // 记录鼠标
    {
        cutScreen->setStart( e->pos() );

    }
    else if( status==MOV ) //
    {
        // 不在截图区域内，重新选择
        if( cutScreen->isInArea(e->pos())==false )
        {
            cutScreen->setStart( e->pos() );
            cutScreen->setStatus(SELECT);
        }
        // 在截图区域，移动截图 鼠标指针成十字
        else
        {
            movPos = e->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }
    update();

}


void ShowMain::mouseReleaseEvent(QMouseEvent *)
{
    if( cutScreen->getStatus()==SELECT )// SELECT状态下 释放鼠标
    {
        cutScreen->setStatus(MOV);//移动、撤销
    }
    else if( cutScreen->getStatus()==MOV )// 鼠标成正常状态
    {
        this->setCursor(Qt::ArrowCursor);
    }
}

void ShowMain::trayIconActivated(QSystemTrayIcon::ActivationReason activationReason)
{
     connect(trayIcon,&QSystemTrayIcon::activated, this, \
                                                     &ShowMain::activeTray);
}


void ShowMain::activeTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {

            case QSystemTrayIcon::DoubleClick:
                this->show();
                break;
            case QSystemTrayIcon::Trigger:

                break;

    }

}
