namespace Ltalk {
#include <memory>
class Http;

typedef std::shared_ptr<Http> SPHttp;
class Http final{
public:
    Http();
    ~Http();
};
}
