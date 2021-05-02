#include <iostream>

#include "Parser.h"

int main() {
    JsonData jsdata = {"message", 20, 30, "20032020", "Hello",  "text", "private"};
    std::string jsString = JsonParser::jsonDataToJson(jsdata);
    // std::cout << jsString;

    std::string s = "{\"requestType\":\"update\", \"updateDate\":\"20/12/2020\"}";

    JsonData newJsonData = JsonParser::jsonToJsonData(s);

    std::cout << newJsonData.updateDate << std::endl;

    return 0;
}
