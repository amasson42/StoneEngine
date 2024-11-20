#include "Utils/Json.hpp"

#include <gtest/gtest.h>

using namespace Stone;

TEST(Json, ParseEmptyObject) {
	std::string jsonString = "{}";

	Json::Value json;
	Json::parseString(jsonString, json);

	ASSERT_TRUE(json.is<Json::Object>());
	ASSERT_TRUE(json.get<Json::Object>().empty());
}

TEST(Json, ParseSimpleObject) {
	std::string jsonString = R"({"name": "John", "age": 30, "isStudent": false})";

	Json::Value json;
	Json::parseString(jsonString, json);

	ASSERT_TRUE(json.is<Json::Object>());

	Json::Object obj = json.get<Json::Object>();
	ASSERT_TRUE(obj["name"].is<std::string>());
	ASSERT_EQ(obj["name"].get<std::string>(), "John");

	ASSERT_TRUE(obj["age"].is<double>());
	ASSERT_EQ(obj["age"].get<double>(), 30);

	ASSERT_TRUE(obj["isStudent"].is<bool>());
	ASSERT_EQ(obj["isStudent"].get<bool>(), false);
}

TEST(Json, ParseArray) {
	std::string jsonString = R"([1, "two", true, null])";

	Json::Value json;
	Json::parseString(jsonString, json);

	ASSERT_TRUE(json.is<Json::Array>());

	Json::Array arr = json.get<Json::Array>();
	ASSERT_EQ(arr.size(), 4);

	ASSERT_TRUE(arr[0].is<double>());
	ASSERT_EQ(arr[0].get<double>(), 1);

	ASSERT_TRUE(arr[1].is<std::string>());
	ASSERT_EQ(arr[1].get<std::string>(), "two");

	ASSERT_TRUE(arr[2].is<bool>());
	ASSERT_EQ(arr[2].get<bool>(), true);

	ASSERT_TRUE(arr[3].isNull());
}

TEST(Json, ParseNestedObject) {
	std::string jsonString = R"({"person": {"name": "John", "age": 30}})";

	Json::Value json;
	Json::parseString(jsonString, json);

	ASSERT_TRUE(json.is<Json::Object>());

	Json::Object obj = json.get<Json::Object>();
	ASSERT_TRUE(obj["person"].is<Json::Object>());

	Json::Object personObj = obj["person"].get<Json::Object>();
	ASSERT_TRUE(personObj["name"].is<std::string>());
	ASSERT_EQ(personObj["name"].get<std::string>(), "John");

	ASSERT_TRUE(personObj["age"].is<double>());
	ASSERT_EQ(personObj["age"].get<double>(), 30);
}

TEST(Json, MalformedJsonThrowsException) {
	std::string jsonString = R"({"name": "John")"; // Missing closing brace

	EXPECT_THROW(
		{
			Json::Value json;
			Json::parseString(jsonString, json);
		},
		std::runtime_error);
}

TEST(Json, MalformedJsonThrowsException2) {
	std::string jsonString = R"({"name": John})"; // No quotes around string

	EXPECT_THROW(
		{
			Json::Value json;
			Json::parseString(jsonString, json);
		},
		std::runtime_error);
}

/*
 * Serializer tests
 */

TEST(JsonSerializer, SerializeEmptyObject) {

	Json::Object obj;
	auto value = Json::object(obj);

	std::string result = value.serialize();
	ASSERT_EQ(result, "{}");
}

TEST(JsonSerializer, SerializeSimpleObject) {

	std::string result;

	{
		Json::Object obj;
		obj["name"] = "John";
		obj["age"] = 30.0;
		obj["isStudent"] = false;

		auto value = Json::object(obj);

		result = value.serialize();
	}

	Json::Value json;
	Json::parseString(result, json);

	ASSERT_TRUE(json.is<Json::Object>());

	auto obj = json.get<Json::Object>();

	ASSERT_TRUE(obj["name"].is<std::string>());
	ASSERT_EQ(obj["name"].get<std::string>(), "John");

	ASSERT_TRUE(obj["age"].is<double>());
	ASSERT_EQ(obj["age"].get<double>(), 30);

	ASSERT_TRUE(obj["isStudent"].is<bool>());
	ASSERT_EQ(obj["isStudent"].get<bool>(), false);
}

TEST(JsonSerializer, SerializeArray) {

	std::string result;

	{
		auto value = Json::array({Json::number(1.0), Json::string("two"), Json::boolean(true), Json::null()});

		result = value.serialize();
	}

	Json::Value json;
	Json::parseString(result, json);

	ASSERT_TRUE(json.is<Json::Array>());

	auto obj = json.get<Json::Array>();
	ASSERT_EQ(obj.size(), 4);

	ASSERT_TRUE(obj[0].is<double>());
	ASSERT_EQ(obj[0].get<double>(), 1);

	ASSERT_TRUE(obj[1].is<std::string>());
	ASSERT_EQ(obj[1].get<std::string>(), "two");

	ASSERT_TRUE(obj[2].is<bool>());
	ASSERT_EQ(obj[2].get<bool>(), true);

	ASSERT_TRUE(obj[3].isNull());
}

TEST(JsonSerializer, SerializeNestedObject) {

	std::string result;

	{
		auto value = Json::object({
			{"person", Json::object({{"name", Json::string("John")}, {"age", Json::number(30.0)}})}
		   });

		result = value.serialize();
	}

	Json::Value json;
	Json::parseString(result, json);

	ASSERT_TRUE(json.is<Json::Object>());

	auto obj = json.get<Json::Object>();

	ASSERT_TRUE(obj["person"].is<Json::Object>());

	auto personObj = obj["person"].get<Json::Object>();

	ASSERT_TRUE(personObj["name"].is<std::string>());
	ASSERT_EQ(personObj["name"].get<std::string>(), "John");

	ASSERT_TRUE(personObj["age"].is<double>());
	ASSERT_EQ(personObj["age"].get<double>(), 30);
}

TEST(JsonSerializer, SerializeComplexObject) {

	std::string result;
	{
		Json::Object addressObj;
		addressObj["city"] = Json::string("New York");
		addressObj["zip"] = Json::string("10001");

		Json::Object obj;
		obj["name"] = "John";
		obj["age"] = 30.0;
		obj["isStudent"] = false;
		obj["scores"] = Json::array({85.5, 92.0, 78.5});
		obj["address"] = addressObj;

		auto value = Json::object(obj);

		result = value.serialize();
	}

	Json::Value json;
	Json::parseString(result, json);

	ASSERT_TRUE(json.is<Json::Object>());

	auto obj = json.get<Json::Object>();
	ASSERT_TRUE(obj["name"].is<std::string>());
	ASSERT_EQ(obj["name"].get<std::string>(), "John");

	ASSERT_TRUE(obj["age"].is<double>());
	ASSERT_EQ(obj["age"].get<double>(), 30);

	ASSERT_TRUE(obj["isStudent"].is<bool>());
	ASSERT_EQ(obj["isStudent"].get<bool>(), false);

	ASSERT_TRUE(obj["scores"].is<Json::Array>());
	auto scores = obj["scores"].get<Json::Array>();
	ASSERT_EQ(scores.size(), 3);
	ASSERT_EQ(scores[0].get<double>(), 85.5);
	ASSERT_EQ(scores[1].get<double>(), 92);
	ASSERT_EQ(scores[2].get<double>(), 78.5);

	ASSERT_TRUE(obj["address"].is<Json::Object>());
	auto address = obj["address"].get<Json::Object>();
	ASSERT_EQ(address["city"].get<std::string>(), "New York");
	ASSERT_EQ(address["zip"].get<std::string>(), "10001");
}

TEST(JsonLexer, NextToken) {
	std::stringstream stream(R"(
{
    "name": "John",
    "age": 30,
    "isStudent": false,
    "scores": [85,-20,12.5],
    "address": {"city": "The \"Island\"","zip": 10501},
}
)");

	Json::Lexer lexer(stream);

#define CHECK_TOKEN(expected_type, expected_value)                                                                     \
	{                                                                                                                  \
		auto token = lexer.nextToken();                                                                                \
		EXPECT_EQ(token.type, expected_type);                                                                          \
		ASSERT_EQ(token.value, expected_value);                                                                        \
	}

	CHECK_TOKEN(Json::TokenType::LeftBrace, "{");
	CHECK_TOKEN(Json::TokenType::String, "name");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::String, "John");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::String, "age");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::Number, "30");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::String, "isStudent");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::False, "false");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::String, "scores");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::LeftBracket, "[");
	CHECK_TOKEN(Json::TokenType::Number, "85");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::Number, "-20");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::Number, "12.5");
	CHECK_TOKEN(Json::TokenType::RightBracket, "]");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::String, "address");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::LeftBrace, "{");
	CHECK_TOKEN(Json::TokenType::String, "city");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::String, "The \"Island\"");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::String, "zip");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::Number, "10501");
	CHECK_TOKEN(Json::TokenType::RightBrace, "}");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::RightBrace, "}");
	CHECK_TOKEN(Json::TokenType::EndOfFile, "");
#undef CHECK_TOKEN
}

TEST(JsonLexer, InterceptToken) {
	std::stringstream stream(R"({"name": "John"}$[1,2,3])");

	Json::Lexer lexer(stream);

#define CHECK_TOKEN(expected_type, expected_value)                                                                     \
	{                                                                                                                  \
		auto token = lexer.nextToken();                                                                                \
		EXPECT_EQ(token.type, expected_type);                                                                          \
		ASSERT_EQ(token.value, expected_value);                                                                        \
	}

	CHECK_TOKEN(Json::TokenType::LeftBrace, "{");
	CHECK_TOKEN(Json::TokenType::String, "name");
	CHECK_TOKEN(Json::TokenType::Colon, ":");
	CHECK_TOKEN(Json::TokenType::String, "John");
	CHECK_TOKEN(Json::TokenType::RightBrace, "}");

	char c;
	stream.get(c);
	ASSERT_EQ(c, '$');

	CHECK_TOKEN(Json::TokenType::LeftBracket, "[");
	CHECK_TOKEN(Json::TokenType::Number, "1");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::Number, "2");
	CHECK_TOKEN(Json::TokenType::Comma, ",");
	CHECK_TOKEN(Json::TokenType::Number, "3");
	CHECK_TOKEN(Json::TokenType::RightBracket, "]");
	CHECK_TOKEN(Json::TokenType::EndOfFile, "");

#undef CHECK_TOKEN
}

TEST(JsonParser, ParseJsonFromStream) {
	std::stringstream stream(R"({"name": "John"}24 0 1[1 2, 3] {"name": "Eric"})");

	{
		Json::Value json;
		stream >> json;

		EXPECT_TRUE(json.is<Json::Object>());

		Json::Object &obj = json.get<Json::Object>();
		EXPECT_TRUE(obj["name"].is<std::string>());
		EXPECT_EQ(obj["name"].get<std::string>(), "John");
	}

	{
		int twentyFour;
		stream >> twentyFour;
		EXPECT_EQ(twentyFour, 24);

		bool b;
		stream >> b;
		EXPECT_EQ(b, false);
		stream >> b;
		EXPECT_EQ(b, true);
	}

	{
		Json::Value json;
		stream >> json;

		EXPECT_TRUE(json.is<Json::Array>());

		Json::Array &arr = json.get<Json::Array>();
		ASSERT_EQ(arr.size(), 3);
		ASSERT_EQ(arr[0].get<double>(), 1);
		ASSERT_EQ(arr[1].get<double>(), 2);
		ASSERT_EQ(arr[2].get<double>(), 3);
	}

	{
		Json::Value json;
		stream >> json;

		EXPECT_TRUE(json.is<Json::Object>());

		Json::Object obj = json.get<Json::Object>();
		EXPECT_TRUE(obj["name"].is<std::string>());
		EXPECT_EQ(obj["name"].get<std::string>(), "Eric");
	}
}
