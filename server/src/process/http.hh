#include <memory>
namespace Ltalk {

class Http;
typedef std::shared_ptr<Http> SPHttp;
class Http final{
public:
    Http();
    ~Http();
};
}
