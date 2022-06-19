//
// Created by SVK on 17.06.2022.
//

#ifndef EXPRESS_EX_ENV_EXSTREAMIFS_H
#define EXPRESS_EX_ENV_EXSTREAMIFS_H
#include <string>
#include <list>
#include <map>


enum class Delimiter {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    SAVEDCOLOR,


    AnsiBLACK = 32,
    AnsiRED,
    AnsiGREEN,
    AnsiYELLOW,
    AnsiBLUE,
    AnsiMAGENTA,
    AnsiCYAN,
    AnsiWHITE
};




enum class ExColors {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    SAVEDCOLOR,
    RESET,

    AnsiBLACK = 32,
    AnsiRED,
    AnsiGREEN,
    AnsiYELLOW,
    AnsiBLUE,
    AnsiMAGENTA,
    AnsiCYAN,
    AnsiWHITE,
    AnsiSAVEDCOLOR,
    AnsiRESET
};

// maybe it would be better to inherit this  class from std::basic_ostream
// and redefine std::basic_streambuf . But this way is faster

class ExStreamIfs{
public :
    virtual ~ExStreamIfs() = default;
    virtual ExStreamIfs &operator<<(const std::string &arg) = 0;
    virtual ExStreamIfs &operator<<(const ExColors & arg) = 0;

    virtual void finalize() = 0;
private:

};



class ExStringStreamIfs:public ExStreamIfs{
public :
    ~ExStringStreamIfs() override = default;
    ExStreamIfs &operator<<(const std::string &arg) override = 0;
    ExStreamIfs &operator<<(const ExColors & arg) override = 0;
    void finalize() override = 0;
    virtual std::string readAndClear() = 0;
};




inline ExStreamIfs & operator<<(ExStreamIfs & stream, bool arg){
    return stream << (arg ? "true" : "false");
}

template <typename T, typename  std::enable_if<
        std::is_arithmetic<T>::value && !std::is_same<T,bool>::value,
        bool>::type = true>
ExStreamIfs & operator<<(ExStreamIfs & stream,T arg ){
    return stream << std::to_string(arg);
}


inline ExStreamIfs & operator<<(ExStreamIfs & stream,Delimiter delimiter){
    return stream << "\n" << (ExColors)(delimiter) << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << (((int)delimiter <= (int)ExColors::RESET) ? ExColors::RESET : ExColors::AnsiRESET) << "\n";
}

template<typename Key_T, typename _T>
inline ExStreamIfs & operator<<(ExStreamIfs & stream, const std::map<Key_T, _T> & arg) {
    for (auto i : arg)
        stream << i.first << ": " << i.second << " ";
    return stream;
}

template< typename _T>
inline ExStreamIfs & operator<<(ExStreamIfs & stream, const std::vector<_T> & arg) {
    for (auto i : arg)
        stream << i << " ";
    return stream;
}


#endif //EXPRESS_EX_ENV_EXSTREAMIFS_H
