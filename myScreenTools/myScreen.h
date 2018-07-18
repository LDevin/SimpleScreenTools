#ifndef MYSCREEN_H
#define MYSCREEN_H

#include "QPoint"
#include "QSize"
#pragma execution_character_set("utf-8")
enum STATUS{SELECT,MOV,SET_W_H};

class myScreen
{
public:
    myScreen(){}
    myScreen(QSize size);

    void setStart(QPoint pos);
    void setEnd(QPoint pos);
    QPoint getStart();
    QPoint getEnd();

    QPoint getLeftUp();
    QPoint getRightDown();

    STATUS getStatus();
    void setStatus(STATUS st);

    int width();
    int height();
    bool isInArea(QPoint pos); // ���pos�Ƿ��ڽ�ͼ������
    void move(QPoint p);       // �� p �ƶ���ͼ����

private:
    QPoint leftUpPos, rightDownPos;     // ��¼ ��ͼ���� ���Ͻǡ����½�
    QPoint startPos, endPos;            //��¼ ��꿪ʼλ�á�����λ��
    int maxWidth, maxHeight;            //��¼��Ļ��С
    // ����״̬ :  ѡ�������ƶ���������width height
    STATUS status;

    void cmpPoint(QPoint &s, QPoint &e); // �Ƚ���λ�ã��ж����Ͻǡ����½�
};

#endif // MYSCREEN_H
