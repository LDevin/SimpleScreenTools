#include "frmMain.h"
#include "ui_frmMain.h"
#include<QMenu>
#include<QCloseEvent>
#include<QMessageBox>
#include<QApplication>
#include<QDesktopWidget>
#include<QColor>
#include<QBrush>
#include<QFileDialog>
#include<QTime>

frmMain::frmMain(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::frmMain)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    //tian jia tuo pan
    trayIcon = new QSystemTrayIcon(QIcon(":/1462.ico"),this);
    trayIcon->setToolTip(tr("��ͼ-----ʥͽ"));
    //tian jia tuo pan cai dang
    QMenu *menu = new QMenu;
    menu->addAction(tr("�˳�"),qApp,SLOT(quit()));
    trayIcon->setContextMenu(menu);
    //
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
    //ȡ����Ļ��С����ʼ�� cutScreen
    cutScreen = new myScreen(QApplication::desktop()->size());
    resize(cutScreen->width(),cutScreen->height());

    //����ȫ��
    this->hide();
    fullScreen = new QPixmap();
    *fullScreen = QPixmap::grabWindow(QApplication::desktop()->winId(),0,0,cutScreen->width(),cutScreen->height());

    //����͸����ʵ��ģ������
    QPixmap pix(cutScreen->width(),cutScreen->height());
    pix.fill((QColor(160,160,165,192)));
    bgScreen = new QPixmap(*fullScreen);
    QPainter p(bgScreen);
    p.drawPixmap(0,0,pix);

    //��ͼ��Ϣ��ʾ���򱳾�
    infoPix = new QPixmap(WIDTH_SHOW,HEIGHT_SHOW);
    QPainter infoP(infoPix);
    infoP.setBrush(QBrush(QColor(Qt::black),Qt::SolidPattern));
    infoP.drawRect(0,0,WIDTH_SHOW,HEIGHT_SHOW);

    input = new myInputDialog(this);
    //show init screen
    this->show();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::paintEvent(QPaintEvent *e)
{
    int x = cutScreen->getLeftUp().x();
    int y = cutScreen->getLeftUp().y();
    int w = cutScreen->getRightDown().x()-x;
    int h = cutScreen->getRightDown().y()-y;

    QPainter painter(this);
    QPen pen; pen.setColor(Qt::green); pen.setWidth(1); pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawPixmap(0,0,*bgScreen);  //��ģ������
    if( w!=0 && h!=0 )    painter.drawPixmap(x,y,fullScreen->copy(x,y,w,h));  //����ȡ����
    painter.drawRect(x,y,w,h);  //��ȡ����߿�

    //��ʾ��ȡ������Ϣ width height
    painter.drawPixmap(x,y-32,*infoPix);
    painter.drawText(x+2,y-20,QString("��ͼ��Χ��(%1 x %2)-(%3 x %4) ͼƬ��С��(%5 x %6)").arg(x).arg(y).arg(x+w).arg(y+h).arg(w).arg(h));
    painter.drawText(x+2,y-6,QString("ʹ��˵����(%1)").arg("enter����ͼƬ,Fȫ����ͼ,ESC�˳�����,˫�����ÿ��"));
}

//��ȡȫ��
void frmMain::saveFullScreen()
{
    fullScreen->save("fullScreen.jpg","JPG");
}

//�����ȡ����
void frmMain::saveScreen()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString str = QDateTime().currentDateTime().toString("yyyyMMddHHmmss");
    QString fileName = str;
    fileName = QFileDialog::getSaveFileName(this,
                                            tr("����ͼƬ"),
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

//��������
void frmMain::keyReleaseEvent(QKeyEvent *e )
{
    if( e->key()==Qt::Key_Escape )// esc �˳�
    {
        this->hide();
    }
    else if( e->key()==Qt::Key_F )// f ��ȡȫ��
    {
        saveFullScreen();
        this->hide();
    }
    else if( e->key()==Qt::Key_Return||e->key()==Qt::Key_Enter)//�س� �����ȡ����
    {
        saveScreen();
        this->hide();
    }
}

//����ƶ��� SELECT MOV ����״̬��
void frmMain::mouseMoveEvent(QMouseEvent *e)
{
    if( cutScreen->getStatus()==SELECT ) // ѡ������
    {
        cutScreen->setEnd( e->pos() );
    }
    else if( cutScreen->getStatus()==MOV ) //�ƶ���ѡ����
    {
        QPoint p(e->x()-movPos.x(),e->y()-movPos.y());
        cutScreen->move(p);
        movPos = e->pos();
    }
    update();
}

//������꣬��¼��ֵ
void frmMain::mousePressEvent(QMouseEvent *e)
{
    int status = cutScreen->getStatus();

    if( status==SELECT ) // ��¼���
    {
        cutScreen->setStart( e->pos() );
    }
    else if( status==MOV ) //
    {
        // ���ڽ�ͼ�����ڣ�����ѡ��
        if( cutScreen->isInArea(e->pos())==false )
        {
            cutScreen->setStart( e->pos() );
            cutScreen->setStatus(SELECT);
        }
        // �ڽ�ͼ�����ƶ���ͼ ���ָ���ʮ��
        else
        {
            movPos = e->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }
    update();
}

//�κ�����£�˫������ width height���ÿ�
void frmMain::mouseDoubleClickEvent(QMouseEvent *e)
{
    if( cutScreen->getStatus()!= SET_W_H )
    {
        cutScreen->setStatus(SET_W_H);

        input->show();
        input->exec();

        if( input->isOk()==true ) //������Ч���ı������С
        {
            QPoint pos = cutScreen->getLeftUp();
            cutScreen->setStart(pos);
            cutScreen->setEnd(QPoint(pos.x()+input->getWidth(),pos.y()+input->getHeight()));
        }

        cutScreen->setStatus(MOV);// �ص� MOV ״̬
    }
}

void frmMain::mouseReleaseEvent(QMouseEvent *)
{
    if( cutScreen->getStatus()==SELECT )// SELECT״̬�� �ͷ����
    {
        cutScreen->setStatus(MOV);//�ƶ�������
    }
    else if( cutScreen->getStatus()==MOV )// ��������״̬
    {
        this->setCursor(Qt::ArrowCursor);
    }
}
void frmMain::trayIconActivated(QSystemTrayIcon::ActivationReason activationReason){
    if(activationReason == QSystemTrayIcon::Trigger);
    {
        this->show();
    }
}
