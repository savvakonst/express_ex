//
// Created by SVK on 17.06.2022.
//

#ifndef EXPRESS_EX_ENV_EXSTREAM_H
#define EXPRESS_EX_ENV_EXSTREAM_H
#include "ExStreamIfs.h"


class ExStream: public  ExStringStreamIfs{
public :
    ~ExStream() override = default;
    ExStringStreamIfs &operator<<(const std::string &arg) override ;
    ExStringStreamIfs &operator<<(const ExColors & arg) override ;
};

class ExStringStream: public  ExStringStreamIfs{
public :
    ~ExStringStream() override = default;
    ExStringStreamIfs &operator<<(const std::string &arg) override ;
    ExStringStreamIfs &operator<<(const ExColors & arg) override ;

    std::string readAndClear() override ;
private:
    std::list<std::string> list_;
};


class ExErrorStream: public  ExStringStreamIfs{
public :
    ~ExErrorStream() override = default;
    ExStringStreamIfs &operator<<(const std::string &arg) override ;
    ExStringStreamIfs &operator<<(const ExColors & arg) override ;
};


class ExErrorStringStream: public  ExStringStream{
public :
    ~ExErrorStringStream() override = default;
};






#endif //EXPRESS_EX_ENV_EXSTREAM_H
