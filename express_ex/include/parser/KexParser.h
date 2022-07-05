//
// Created by SVK on 17.06.2022.
//

#ifndef EXPRESS_EX_ENV_KEXPARSER_H
#define EXPRESS_EX_ENV_KEXPARSER_H
#include <list>
#include <map>
#include <string>
#include <strstream>

class BodyTemplate;
class TreeShapeListener;


class KEXParser {
   public:
    KEXParser(BodyTemplate* body, const std::string& str, bool is_file_name = true);

    explicit KEXParser(const std::string& str, bool is_file_name = true,
                       const std::list<std::pair<std::string, bool /*is_file_name*/>>& lib_str_map = {});

    ~KEXParser();

    TreeShapeListener* getListener() { return listener_; }

    BodyTemplate* getActivBody();

   private:
    void init(std::string str, bool is_file_name = true);

    TreeShapeListener* listener_ = nullptr;
};



#endif  // EXPRESS_EX_ENV_KEXPARSER_H
