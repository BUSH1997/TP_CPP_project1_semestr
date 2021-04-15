

#include "gtest/gtest.h"

#include "jsonParser.h"


TEST(jsonParser_jsonToJsonDataParse, allData) {
    std::string jsonString = "{'requestType': 'message', 'transmitterId': 100, receiverId': 1000, messageId: 500, 'date': '20/10/2021/23/55/58/000000', 'text': 'Hello', 'contentType': 'text', name': 'Sergey' 'surname': 'Kust', 'age': 23, 'password': '****', 'login': 'bush', 'userId': 5555, 'updateDate': 'date': '20/10/2021/23/55/58/000000', 'pushServerPort': 10000, 'messages': 'nullptr', 'messagesNumber': 0, 'chatType': 'private' }";

    ASSERT_NO_THROW(JsonParser::jsonToJsonData(jsonString));

    JsonData data = JsonParser::jsonToJsonData(jsonString);

    EXPECT_EQ(data.requestType, "message");
    EXPECT_EQ(data.transmitterId, 100);
    EXPECT_EQ(data.receiverId, 1000);
    EXPECT_EQ(data.messageId, 500);
    EXPECT_EQ(data.date, "20/10/2021/23/55/58/000000");
    EXPECT_EQ(data.text, "Hello");
    EXPECT_EQ(data.contentType, "text");
    EXPECT_EQ(data.name, "Sergey");
    EXPECT_EQ(data.surname, "Kust");
    EXPECT_EQ(data.age, 23);
    EXPECT_EQ(data.password, "****");
    EXPECT_EQ(data.login, "bush");
    EXPECT_EQ(data.userId, 5555);
    EXPECT_EQ(data.updateDate, "20/10/2021/23/55/58/000000");
    EXPECT_EQ(data.pushServerPort, 10000);
    EXPECT_EQ(data.messages, nullptr);
    EXPECT_EQ(data.messagesNumber, 0);
    EXPECT_EQ(data.chatType, "private");
}

TEST(jsonParser_jsonToJsonDataParse, notAllData) {
    std::string jsonString = "{'requestType': 'message'}";

    ASSERT_NO_THROW(JsonParser::jsonToJsonData(jsonString));

    JsonData data = JsonParser::jsonToJsonData(jsonString);

    EXPECT_EQ(data.requestType, "message");
    EXPECT_NE(data.transmitterId, 100);
    EXPECT_NE(data.receiverId, 1000);
    EXPECT_NE(data.messageId, 500);
    EXPECT_NE(data.date, "20/10/2021/23/55/58/000000");
    EXPECT_NE(data.text, "Hello");
    EXPECT_NE(data.contentType, "text");
    EXPECT_NE(data.name, "Sergey");
    EXPECT_NE(data.surname, "Kust");
    EXPECT_NE(data.age, 23);
    EXPECT_NE(data.password, "****");
    EXPECT_NE(data.login, "bush");
    EXPECT_NE(data.userId, 5555);
    EXPECT_NE(data.updateDate, "20/10/2021/23/55/58/000000");
    EXPECT_NE(data.pushServerPort, 10000);
    EXPECT_NE(data.messages, nullptr);
    EXPECT_NE(data.messagesNumber, 0);
    EXPECT_NE(data.chatType, "private");
}

TEST(jsonParser_jsonToJsonDataParse, notValidData) {
    std::string jsonString = "{'requestType': 'no_message'}";

    EXPECT_ANY_THROW(JsonParser::jsonToJsonData(jsonString));
}

TEST(jsonParser_jsonDataToJsonParse, equal) {
    JsonData jsonData;
    jsonData.name = "Sergey";
    jsonData.surname = "Kust";
    jsonData.age = 23;

    ASSERT_NO_THROW(JsonParser::jsonDataToJson(jsonData));

    std::string jsonString = JsonParser::jsonDataToJson(jsonData);

    EXPECT_EQ(jsonString, "'name':'Sergey', 'surname': 'Kust', 'age': 23");
}


TEST(jsonParser_jsonDataToJsonParse, notEqual) {
    JsonData jsonData;
    jsonData.name = "Sergey";
    jsonData.surname = "Kust";
    jsonData.age = 23;

    ASSERT_NO_THROW(JsonParser::jsonDataToJson(jsonData));

    std::string jsonString = JsonParser::jsonDataToJson(jsonData);

    EXPECT_NE(jsonString, "'name':'Sergey', 'surname': 'Kust', 'age': 24");
}

TEST(jsonParser_jsonDataToJsonParse, notValidData) {
    JsonData jsonData;
    jsonData.requestType = "no_message";

    EXPECT_ANY_THROW(JsonParser::jsonDataToJson(jsonData));
}




int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

