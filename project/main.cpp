#include <iostream>
#include <vector>
#include <iomanip>

#include "DBwrapper.h"
#include "handlers.h"
#include "parser.h"
#include "dbExceptions.h"

int main() {
   /* std::cout << "Hello, world!" << std::endl;
    JsonData data;
    data.requestType = "message";
    MessageData message1;
    message1.transmitterId = 3;
    message1.receiverId = 7;
    //message1.text = "Technopark is cool";
    message1.date = "2021-05-13 14:51:00";
    message1.contentType = "audio";
    message1.fileName = "3_7_20210513145100";
    message1.fileData = "01101001000101111100100010001011001001";
    message1.chatType = "private";
    data.messages.push_back(message1);

    MessageControllerHandler handler;
    JsonData newData = handler.handle(data);

    std::cout << newData.requestStatus << std::endl;*/

  /* std::string jsonString = R"({"requestType": "message", "users": [{"name": "Artur", "surname": "Pirozhkov", "age": "30", "login": "artStyle", "password": "art228", "userId": "1000", "updateDate":"2021-05-08 17:14:07", "status": "active"}]})";
   JsonData jsonData = JsonParser::jsonToJsonData(jsonString);
    std::cout << jsonData.requestType << std::endl;
    std::cout << jsonData.users[0].name << std::endl;*/

    /*JsonData data;
    data.requestType = "message";
    UserData user1;
    user1.name = "Alex";
    user1.surname = "Norm";
    user1.age = 28;
    user1.login = "alexandr";
    user1.password = "superman228";
    user1.updateDate = "2021-05-08 17:14:07";
    user1.status = "active";
    data.users.push_back(user1);

    std::cout << JsonParser::jsonDataToJson(data) << std::endl;*/
   // ss << "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }";
    /*UserCreatorHandler handler;

    JsonData data;
    UserData user1;
    user1.name = "Alex";
    user1.surname = "Norm";
    user1.age = 28;
    user1.login = "alexandr";
    user1.password = "superman228";
    user1.updateDate = "2021-05-08 17:14:07";
    user1.status = "active";

    data.users.push_back(user1);

    JsonData newData = handler.handle(data);

    std::cout << newData.registrationStatus << std::endl;*/

    UserData user1;
    user1.login = "art-stile";
    user1.password = "art228322";
    user1.userId = 7;
    JsonData data;
    data.users.push_back(user1);

    UserAuthorizerHandler handler;

    JsonData newData = handler.handle(data);

    std::cout << JsonParser::jsonDataToJson(newData) << std::endl;
   /* MessageData message1;
    message1.transmitterId = 3;
    message1.receiverId = 12;
    UserData user1;
    user1.updateDate = "2022-01-19 03:14:07";

    JsonData data;
    data.users.push_back(user1);
    data.messages.push_back(message1);

    LoadArchiveHandler handler;

    JsonData newData = handler.handle(data);

    std::cout << JsonParser::jsonDataToJson(newData) << std::endl;

    data.messages[0].date = "2022-01-19 03:14:07";

    UpdateDateHandler handler1;
    JsonData newData1 = handler1.handle(data);*/

  //  std::cout << JsonParser::jsonDataToJson(newData1) << std::endl;


   /* struct connection_details mysqlD{"localhost", "bushcast", "Sergeykust_333", "test_base"};

    MySQLManager dbManager(mysqlD);

    MessagesData messageData{0, 100, 200, "2021-01-19 03:14:07", "Hello, world!", .contentType = "tex", .chatType = "private"};

    MessageTable messageTb(dbManager);
    try {
        messageTb.INSERT(messageData);
    }
    catch (MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
    }

    std::vector<MessagesData> messages = messageTb.SELECT();

    for (auto& message : messages) {
        std::cout << message.receiverId << ' ' << message.transmitterId << std::endl;
    }*/



   /* UsersData user_by_id;
    try {
        user_by_id = dbManager.getUserTb().SELECT(12);
        std::cout << user_by_id.name << ' ' << user_by_id.status << std::endl;
    }
    catch (MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        user_by_id.name = "No one";
    }

    std::cout << user_by_id.name << std::endl;*/



    return 0;
}

