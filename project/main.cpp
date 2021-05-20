#include <iostream>
#include <vector>
#include <iomanip>

#include "DBwrapper.h"
#include "handlers.h"
#include "parser.h"
#include "dbExceptions.h"

JsonData createUser() {
   /* UserCreatorHandler handler;

    JsonData data;
    UserData user1;
    user1.name = "Yuriy";
    user1.surname = "Gagarin";
    user1.login = "yura_сosmos123";
    user1.password = "yuriygaga123";
    user1.updateDate = "2021-05-20 16:36:54";
    user1.avatarName = "yuraCosmos123_20210520163654";

    data.users.push_back(user1);
    JsonData newData = handler.handle(data);*/

  /* UserCreatorHandler handler;

    JsonData data;
    UserData user1;
    user1.name = "Vladimir";
    user1.surname = "Puplin";
    user1.login = "TheBoss2014";
    user1.password = "qwerty12345";
    user1.updateDate = "2021-05-20 16:38:54";
    user1.avatarName = "TheBoss2014_20210520163854";

    data.users.push_back(user1);

    JsonData newData = handler.handle(data);*/


 /*   UserCreatorHandler handler;

    JsonData data;
    UserData user1;
    user1.name = "Anatoliy";
    user1.surname = "Alexandrov";
    user1.login = "tolyaRector";
    user1.password = "ana12345";
    user1.updateDate = "2021-05-20 16:40:54";
    user1.avatarName = "tolyaRector_20210520164054";

    data.users.push_back(user1);

    JsonData newData = handler.handle(data);*/

    //return newData;
}

JsonData deAuthorizeUser() {
    UserData user1;
    user1.userId = 3;
    JsonData data;
    data.users.push_back(user1);

    UserDeAuthorizerHandler handler;

    JsonData newData = handler.handle(data);

    return newData;
}

JsonData authorizeUser() {
    UserData user1;
    user1.login = "yura_сosmos123";
    user1.password = "yuriygaga123";  // "yuriygaga123"; qwerty12345, ana12345
    user1.userId = 1;
    JsonData data;
    data.users.push_back(user1);

    UserAuthorizerHandler handler;

    JsonData newData = handler.handle(data);

    return newData;
}

JsonData startChat() {
    UserData user1;
    user1.userId = 1;
    JsonData data;
    data.users.push_back(user1);

    StartChatHandler handler;

    JsonData newData = handler.handle(data);

    return newData;
}

JsonData sendMessage() {
    JsonData data;
    data.requestType = "message";
    MessageData message1;
    message1.transmitterId = 2;
    message1.receiverId = 3;
    message1.text = "Как дела?";
    message1.date = "2021-05-20 18:52:40";
    message1.contentType = "text";
  //  message1.fileName = "2_1_20210520180120";
    message1.chatType = "private";

    data.messages.push_back(message1);

    MessageControllerHandler handler;
    JsonData newData = handler.handle(data);

    /*JsonData data;
    data.requestType = "message";
    MessageData message1;
    message1.transmitterId = 2;
    message1.receiverId = 1;
    message1.text = "Хорошо";
    message1.date = "2021-05-20 17:48:20";
    message1.contentType = "text";

    message1.chatType = "private";

    data.messages.push_back(message1);

    MessageControllerHandler handler;
    JsonData newData = handler.handle(data);*/

   /* JsonData data;
    data.requestType = "message";
    MessageData message1;
    message1.transmitterId = 2;
    message1.receiverId = 1;
    message1.date = "2021-05-20 18:01:20";
    message1.contentType = "audio";

    message1.fileName = "2_1_20210520180120";

    message1.chatType = "private";

    data.messages.push_back(message1);

    MessageControllerHandler handler;
    JsonData newData = handler.handle(data);*/

    return newData;
}

JsonData loadArchive() {
    MessageData message1;
    message1.transmitterId = 1;
    message1.receiverId = 2;
    message1.date =  "2021-05-20 19:50:07";

    JsonData data;
    data.messages.push_back(message1);

    LoadArchiveHandler handler;

    JsonData newData = handler.handle(data);

    return newData;
}

int main() {

  // std::cout << JsonParser::jsonDataToJson(createUser()) << std::endl;
    std::cout << JsonParser::jsonDataToJson(deAuthorizeUser()) << std::endl;
  //  std::cout << JsonParser::jsonDataToJson(authorizeUser()) << std::endl;
  //  std::cout << JsonParser::jsonDataToJson(startChat()) << std::endl;
   // std::cout << JsonParser::jsonDataToJson(sendMessage()) << std::endl;
    //std::cout << JsonParser::jsonDataToJson(loadArchive()) << std::endl;

    return 0;
}
