//
// Created by SVK on 17.06.2022.
//

#ifndef EXPRESS_EX_ENV_ERRORIOIFS_H
#define EXPRESS_EX_ENV_ERRORIOIFS_H
#include <string>

class ErrorIoIfs{
public:
    virtual ~ErrorIoIfs() = default;
    virtual void errorIo(const  std::string & msg) = 0;
    virtual void errorIo(const  char* msg) = 0;
};

#endif //EXPRESS_EX_ENV_ERRORIOIFS_H
