//
// Created by SVK on 17.06.2022.
//

#ifndef EXPRESS_EX_ENV_EXSTREAM_H
#define EXPRESS_EX_ENV_EXSTREAM_H
#include "ExStreamIfs.h"
#include "config.h"

class DLL_EXPORT ExStream: public  ExStreamIfs{
public :
    ~ExStream() override = default;
    ExStreamIfs &operator<<(const std::string &arg) override ;
    ExStreamIfs &operator<<(const ExColors & arg) override ;
    void finalize() override;

};

class DLL_EXPORT ExStringStream: public  ExStringStreamIfs{
public :
    ~ExStringStream() override = default;
    ExStreamIfs &operator<<(const std::string &arg) override ;
    ExStreamIfs &operator<<(const ExColors & arg) override ;
    void finalize() override;;
    std::string readAndClear() override ;
private:
    std::list<std::string> list_;
};


class DLL_EXPORT ExErrorStream: public  ExStreamIfs{
public :
    ~ExErrorStream() override = default;
    ExStreamIfs &operator<<(const std::string &arg) override ;
    ExStreamIfs &operator<<(const ExColors & arg) override ;
    void finalize() override;;
};


class DLL_EXPORT ExErrorStringStream: public  ExStringStream{
public :
    ~ExErrorStringStream() override = default;
};






#endif //EXPRESS_EX_ENV_EXSTREAM_H
