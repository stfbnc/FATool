#include <QApplication>
#include "main_window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("stapp");
    QCoreApplication::setApplicationName("FATool");

    MainWindow win;
    win.show();

    return app.exec();
}
