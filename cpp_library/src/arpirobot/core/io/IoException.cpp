
#include <arpirobot/core/io/IoException.hpp>

using namespace arpirobot;


IoException::IoException(const char *msg) : msg(std::string(msg)){

}

IoException::IoException(std::string msg) : msg(msg){

}

const char *IoException::what() const throw(){
    return msg.c_str();
}
