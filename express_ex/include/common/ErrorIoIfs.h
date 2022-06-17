//
// Created by SVK on 17.06.2022.
//

#ifndef EXPRESS_EX_ENV_ERRORIOIFS_H
#define EXPRESS_EX_ENV_ERRORIOIFS_H
#include <string>
#include <list>

class ErrorIoIfs{
public:
    virtual ~ErrorIoIfs() = default;
    virtual void errorIo(const std::string & msg_a, const std::string & msg_b ="", const std::string & msg_c="") = 0;

};

#endif //EXPRESS_EX_ENV_ERRORIOIFS_H
