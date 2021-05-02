#include "Parser.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

boost::property_tree::ptree buildJsonTree(const JsonData &jsonData)
{
    boost::property_tree::ptree data;
    data.put("requestType", jsonData.requestType);
    data.put("transmitterId", jsonData.transmitterId);
    data.put("receiverId", jsonData.receiverId);
    data.put("date", jsonData.date);
    data.put("text", jsonData.text);
    data.put("contentType", jsonData.contentType);
    data.put("type", jsonData.chatType);

    data.put("name", jsonData.name);
    data.put("surname", jsonData.surname);
    data.put("password", jsonData.password);
    data.put("login", jsonData.login);
    data.put("userId", jsonData.userId);
    data.put("updateDate", jsonData.updateDate);
    data.put("status", jsonData.status);

    data.put("pushServerPort", jsonData.pushServerPort);
    data.put("path", jsonData.path);

    return data;
}

JsonData JsonParser::jsonToJsonData(const std::string &jsonString) {
    boost::property_tree::ptree pt;

    std::stringstream jsonEncodedData(jsonString);

    read_json(jsonEncodedData, pt);

    JsonData jsonData;

    jsonData.requestType = pt.get<std::string>("requestType", "");
    jsonData.transmitterId = pt.get<int>("transmitterId", -1);
    jsonData.receiverId = pt.get<int>("receiverId", -1);
    jsonData.date = pt.get<std::string>("date", "");
    jsonData.text = pt.get<std::string>("text", "");
    jsonData.contentType = pt.get<std::string>("contentType", "");
    jsonData.chatType = pt.get<std::string>("chatType", "");

    jsonData.name = pt.get<std::string>("name", "");
    jsonData.surname = pt.get<std::string>("surname", "");
    jsonData.password = pt.get<std::string>("password", "");
    jsonData.login = pt.get<std::string>("login", "");
    jsonData.userId = pt.get<int>("userId", -1);
    jsonData.updateDate = pt.get<std::string>("updateDate", "");
    jsonData.status = pt.get<std::string>("status", "");

    jsonData.pushServerPort = pt.get<int>("pushServerPort", -1);
    jsonData.path = pt.get<std::string>("path", "");

    return jsonData;
}

std::string JsonParser::jsonDataToJson(const JsonData &jsonData) {
    boost::property_tree::ptree tree = buildJsonTree(jsonData);
    std::stringstream json;
    boost::property_tree::write_json(json, tree);
    std::string jsonString = json.str();
    return jsonString;
}
