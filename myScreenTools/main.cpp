#include <QApplication>
#include "frmMain.h"

#include "QTextCodec"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //֧�����ı���
    //QTextCodec *codec=QTextCodec::codecForLocale();
    //QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec);

    frmMain w;
    w.show();

    //QApplication::setStyle(QStyleFactory::create("Plastique"));

    return a.exec();
}
