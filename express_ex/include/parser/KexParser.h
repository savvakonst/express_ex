//
// Created by SVK on 17.06.2022.
//

#ifndef EXPRESS_EX_ENV_KEXPARSER_H
#define EXPRESS_EX_ENV_KEXPARSER_H
#include <string>
#include <map>
#include <list>
#include <strstream>

class BodyTemplate;
class TreeShapeListener;


class KEXParser {
public:
    KEXParser(BodyTemplate* body, const std::string& str, bool is_file_name = true);

    KEXParser(const std::string& str, bool is_file_name = true,
              const std::map<std::string, bool /*is_file_name*/>& lib_str_map = {});

    ~KEXParser();

    TreeShapeListener* getListener() { return listener_; }

    BodyTemplate* getActivBody();

private:
    /*
    std::vector<BodyTemplate*> & getContext() {
        print_error("KEXParser::getContext()");
        return {nullptr};
        //return listener_.context_;
    }
    */

    void init(std::string str, bool is_file_name = true);

    TreeShapeListener *listener_ = nullptr;
};



#endif //EXPRESS_EX_ENV_KEXPARSER_H
