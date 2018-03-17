#include <QApplication>
#include "frmmain.h"
#include "myStaticClass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormHelper::SetUTF8Code();
    FormHelper::SetPlastiqueStyle();
    frmMain w;
    w.show();

    return a.exec();
}
