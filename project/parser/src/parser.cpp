#include "parser.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/range/algorithm.hpp>

boost::property_tree::ptree buildJsonTree(const JsonData &jsonData)
{
    boost::property_tree::ptree proto;

    proto.put("requestType", jsonData.requestType);
    proto.put("requestStatus", jsonData.requestStatus);
    proto.put("errorDescription", jsonData.errorDescription);

    boost::property_tree::ptree usersArray;
    boost::range::for_each(jsonData.users, [&usersArray](const UserData& user) {
        boost::property_tree::ptree arrayElem;
        arrayElem.put("name", user.name);
        arrayElem.put("surname", user.surname);
        arrayElem.put("login", user.login);
        arrayElem.put("password", user.password);
        arrayElem.put("userId", user.userId);
        arrayElem.put("updateDate", user.updateDate);
        arrayElem.put("status", user.status);
        arrayElem.put("avatarName", user.avatarName);
        usersArray.push_back(boost::property_tree::ptree::value_type("", arrayElem)); });


    proto.put_child("users", usersArray);

    boost::property_tree::ptree messagesArray;
    boost::range::for_each(jsonData.messages, [&messagesArray](const MessageData& message) {
        boost::property_tree::ptree arrayElem;
        arrayElem.put("receiverId", message.receiverId);
        arrayElem.put("transmitterId", message.transmitterId);
        arrayElem.put("date", message.date);
        arrayElem.put("text", message.text);
        arrayElem.put("contentType", message.contentType);
        arrayElem.put("chatType", message.chatType);
        arrayElem.put("fileName", message.fileName);
        messagesArray.push_back(boost::property_tree::ptree::value_type("", arrayElem)); });

    proto.put_child("messages", messagesArray);

    return proto;
}

JsonData JsonParser::jsonToJsonData(const std::string &jsonString) {
    boost::property_tree::ptree pt;

    std::stringstream jsonEncodedData(jsonString);

    read_json(jsonEncodedData, pt);

    JsonData jsonData;

    jsonData.requestType = pt.get<std::string>("requestType", "");
    jsonData.requestStatus = pt.get<std::size_t>("requestStatus", 0);
    jsonData.errorDescription = pt.get<std::size_t>("errorDescription", 0);

    try {
        boost::property_tree::ptree& usersArray = pt.get_child("users");

        for (auto& element : usersArray) {
            UserData user;
            user.name = element.second.get<std::string>("name", "");
            user.surname = element.second.get<std::string>("surname", "");
            user.login = element.second.get<std::string>("login", "");
            user.password = element.second.get<std::string>("password", "");
            user.userId = element.second.get<std::size_t>("userId", 0);
            user.updateDate = element.second.get<std::string>("updateDate", "");
            user.status = element.second.get<std::string>("status", "");
            user.avatarName = element.second.get<std::string>("avatarName", "");
            jsonData.users.push_back(user);
        }
    }

    catch (boost::wrapexcept<boost::property_tree::ptree_bad_path>& e) {}

    try {
        boost::property_tree::ptree &messagesArray = pt.get_child("messages");

        for (auto& element : messagesArray) {
            MessageData message;
            message.transmitterId = element.second.get<std::size_t>("transmitterId", 0);
            message.receiverId = element.second.get<std::size_t>("receiverId", 0);
            message.date = element.second.get<std::string>("date", "");
            message.text = element.second.get<std::string>("text", "");
            message.contentType = element.second.get<std::string>("contentType", "");
            message.chatType = element.second.get<std::string>("chatType", "");
            message.fileName = element.second.get<std::string>("fileName", "");
            jsonData.messages.push_back(message);
       }
    }
    catch (boost::wrapexcept<boost::property_tree::ptree_bad_path>& e) {}

    return jsonData;
}

std::string JsonParser::jsonDataToJson(const JsonData &jsonData) {
    boost::property_tree::ptree tree = buildJsonTree(jsonData);
    std::stringstream json;
    boost::property_tree::write_json(json, tree);
    std::string jsonString = json.str();
    return jsonString;
}
