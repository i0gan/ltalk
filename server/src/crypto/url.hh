#pragma once

#include "../ltalk.hh"
#include <string>

class Crypto::Url {
public:
    static std::string Decode(const std::string &url) {
                std::string ret;
        for(size_t i = 0, j = 0; i < url.size(); ++j, ++i) {
            unsigned char n = 0;
            if(url[i] == '%') {
                if(url[i + 1] >= '0' && url[i + 1] <= '9')      n = (url[i + 1] - '0') * 0x10;
                else if(url[i + 1] >= 'a' && url[i + 1] <= 'f') n = ((url[i + 1] - 'a') + 0x0a) * 0x10;
                else if(url[i + 1] >= 'A' && url[i + 1] <= 'F') n = ((url[i + 1] - 'A') + 0x0a)* 0x10;

                if(url[i + 2] >= '0' && url[i + 2] <= '9')      n += (url[i + 2] - '0');
                else if(url[i + 2] >= 'a' && url[i + 2] <= 'f') n += (url[i + 2] - 'a') + 0x0a;
                else if(url[i + 2] >= 'A' && url[i + 2] <= 'F') n += (url[i + 2] - 'A') + 0x0a;
                i += 2;
            }else n = url[i];
                        ret += n;
        }
        return ret;
    }
};
