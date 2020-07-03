#include "center.h"

#include <QApplication>
#include "user_chat_page.h"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Ltalk for linux");
    app.setApplicationVersion("0.1");
    app.setOrganizationName("LYXF");
    app.setApplicationDisplayName("Ltalk");
//    Center center;
//    center.init();
//    center.changeTheme("default");
//    center.start();
    UserChatPage c;
    c.init();
    c.show();
    return app.exec();
}
