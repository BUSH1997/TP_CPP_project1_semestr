#ifndef BUSINESSLOGIC_JSONPARSER_H
#define BUSINESSLOGIC_JSONPARSER_H

#include "jsonData.h"

class JsonParser {
    static JsonData jsonToJsonData(const std::string& json);
    static std::string jsonDataToJson(const JsonData& jsonData);
};

#endif //BUSINESSLOGIC_JSONPARSER_H
