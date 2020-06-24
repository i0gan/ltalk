#include "center.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Ltalk for linux");
    app.setApplicationVersion("0.1");
    app.setOrganizationName("LYXF");
    //app.setApplicationDisplayName("Ltalk");

    Center center;
    center.init();
    center.start();

    return app.exec();
}
