#include "myInputDialog.h"

#include<QMessageBox>
#include<QPainter>

myInputDialog::myInputDialog(QWidget *parent)
    : QDialog(parent)
{
    int width = 210, height = 120;
    this->setWindowTitle("���÷�Χ");
    this->setFixedSize(width,height);

    resize(width,height);

    // width ���ñ�ǩ
    WLabel = new QLabel(this);
    WLabel->setText("���: ");    WLabel->setGeometry(10,10,50,30);
    inputW = new QLineEdit(this);
    inputW->setGeometry(60,10,130,30);
    inputW->setFocus();

    // height ���ñ�ǩ
    HLabel = new QLabel(this);
    HLabel->setText("�߶�: ");   HLabel->setGeometry(10,50,50,30);
    inputH = new QLineEdit(this);
    inputH->setGeometry(60,50,130,30);

    // ȷ����ť
    okButton = new QPushButton(this);
    okButton->setText("ȷ ��");
    okButton->setGeometry(10,90,75,23);

    // ȡ����ť
    noButton = new QPushButton(this);
    noButton->setText("ȡ ��");
    noButton->setGeometry(115,90,75,23);

    connect(okButton,SIGNAL(clicked()),this,SLOT(get()));
    connect(noButton,SIGNAL(clicked()),this,SLOT(cancel()));

    //Ĭ���� 0
    width = 0;
    height = 0;
}

void myInputDialog::get()
{
    if( inputW->text()!="" && inputH->text()!="" )
    {
        width = inputW->text().toInt();
        height = inputH->text().toInt();
        hide();
    }
    else  // û��������ʱ
    {
        QMessageBox::information(this,"��ʾ","û���������ݣ�");
    }
}

//ȡ��ʱ������ֵ -1 ��������
void myInputDialog::cancel()
{
    width = -1;
    height = -1;
    inputW->setText("");
    inputH->setText("");
    hide();
}

bool myInputDialog::isOk()
{
    bool b;
    if( inputW->text()!="" && inputH->text()!="" )
    {
        b=true;
    }
    // Ϊ -1 ˵������ȡ����
    else if( width==-1 && height==-1 )
    {
        width=0;
        height=0;
        b=false;
    }
    return b;
}
