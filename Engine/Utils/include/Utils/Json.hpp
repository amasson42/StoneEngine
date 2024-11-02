// Copyright 2024 Stone-Engine

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Stone::Json {

struct Value;

using Object = std::unordered_map<std::string, Value>;
using Array = std::vector<Value>;

struct Value {

	std::variant<Object, Array, std::string, double, bool, std::nullptr_t> value;

	Value() : value(nullptr) {
	}

	template <typename T, typename = std::enable_if_t<std::is_constructible_v<
							  std::variant<Object, Array, std::string, double, bool, std::nullptr_t>, T>>>
	Value(T &&val) : value(std::forward<T>(val)) {
	}

	template <typename T>
	bool is() const {
		return std::holds_alternative<T>(value);
	}

	bool isNull() const {
		return std::holds_alternative<std::nullptr_t>(value);
	}

	template <typename T>
	T &get() {
		return std::get<T>(value);
	}

	template <typename T>
	const T &get() const {
		return std::get<T>(value);
	}

	static void parseString(const std::string &input, Value &out);
	static void parseFile(const std::string &path, Value &out);
	std::string serialize() const;
};

Value object(const Object &obj = {});
Value array(const Array &arr = {});
Value string(const std::string &str = "");
Value number(double num = 0.0);
Value boolean(bool b = false);
Value null();


enum class TokenType {
	LeftBrace,
	RightBrace,
	LeftBracket,
	RightBracket,
	Comma,
	Colon,
	String,
	Number,
	True,
	False,
	Null,
	EndOfFile
};

std::string to_string(TokenType type);

struct Token {
	TokenType type;
	std::string value;
};

class Lexer {
public:
	explicit Lexer(const std::string &input);

	Token nextToken();

private:
	const std::string &_input;
	std::size_t _pos = 0;

	Token _stringToken();
	Token _otherTokens();
	Token _numberToken();
};

class Parser {
public:
	explicit Parser(const std::string &input);

	void parse(Value &out);

private:
	Lexer _lexer;
	Token _currentToken;

	void _parseValue(Value &out);
	void _parseObject(Value &out);
	void _parseArray(Value &out);
	void _parsePrimitive(Value &out);
	void _consume(TokenType expected);
};

class Serializer {

public:
	std::string serialize(const Value &value);

	void operator()(const Object &obj);
	void operator()(const Array &arr);
	void operator()(const std::string &str);
	void operator()(double num);
	void operator()(bool b);
	void operator()(std::nullptr_t);

private:
	std::stringstream _ss;
};

} // namespace Stone::Json
