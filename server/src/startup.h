#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>

class StartUp {
public:
     StartUp();
    ~StartUp();
    bool init(int argv, char **argc);
    bool run();
    bool stop();

private:
    bool loadConfig();
    bool initNetwork();
    bool initDatabase();
    bool initLog();

    int threadNum = 4;
    int queueSize = 65535;
    int httpPort = 80;
    std::string path = "/";

};
