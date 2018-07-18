#ifndef FRMMAIN_H
#define FRMMAIN_H

#define WIDTH_SHOW 340
#define HEIGHT_SHOW 30

#include <QWidget>
#include <QSystemTrayIcon>
#include<QtGui>
#include<QPixmap>
#include<QPainter>
#include<QPoint>
#include<QLabel>
#include<QPixmap>
#include<QLineEdit>

#include "myScreen.h"
#include "myInputDialog.h"
#pragma execution_character_set("utf-8")
namespace Ui {
    class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private:
    Ui::frmMain *ui;

    myScreen *cutScreen;      // RCutScreen����,��¼x y w h ����
    myInputDialog *input;     // ���� width height
    QPixmap *infoPix;         // ��ͼ��Ϣ��ʾ����
    QPixmap *fullScreen;      // ����ȫ��ͼ��
    QPixmap *bgScreen;        // ģ������ͼ
    QPoint movPos;

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);


private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason activationReason);

private:
    void saveScreen();
    void saveFullScreen();
    QSystemTrayIcon *trayIcon;


};

#endif // FRMMAIN_H
