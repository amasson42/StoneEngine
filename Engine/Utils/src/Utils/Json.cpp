// Copyright 2024 Stone-Engine

#include "Utils/Json.hpp"

#include "Utils/FileSystem.hpp"
#include "Utils/StringExt.hpp"

#include <sstream>


namespace Stone::Json {

Value::Value(const Object &obj) : value(std::move(obj)) {
}

Value::Value(const Array &arr) : value(std::move(arr)) {
}

Value::Value(const std::string &str) : value(std::move(str)) {
}

Value::Value(double num) : value(num) {
}

Value::Value(bool b) : value(b) {
}

Value::Value(std::nullptr_t n) : value(n) {
}

void Value::parseString(const std::string &input, Value &out) {
	Parser parser(input);
	parser.parse(out);
}

void Value::parseFile(const std::string &path, Value &out) {
	parseString(Utils::readTextFile(path), out);
}

std::string Value::serialize() const {
	Serializer serializer;
	return serializer.serialize(*this);
}

Value object(const Object &obj) {
	return Value(obj);
}

Value array(const Array &arr) {
	return Value(arr);
}

Value string(const std::string &str) {
	return Value(str);
}

Value number(double num) {
	return Value(num);
}

Value boolean(bool b) {
	return Value(b);
}

Value null() {
	return Value();
}


Lexer::Lexer(const std::string &input) : _input(input) {
}

Token Lexer::nextToken() {
	while (_pos < _input.size() && std::isspace(_input[_pos]))
		_pos++;

	if (_pos >= _input.size())
		return {TokenType::EndOfFile, ""};

	char current = _input[_pos];

	switch (current) {
	case '{': _pos++; return {TokenType::LeftBrace, "{"};
	case '}': _pos++; return {TokenType::RightBrace, "}"};
	case '[': _pos++; return {TokenType::LeftBracket, "["};
	case ']': _pos++; return {TokenType::RightBracket, "]"};
	case ':': _pos++; return {TokenType::Colon, ":"};
	case ',': _pos++; return {TokenType::Comma, ","};
	case '"': return _stringToken();
	default: return _otherTokens();
	}
}

Token Lexer::_stringToken() {
	std::stringstream ss;
	_pos++; // Skip the opening quote
	while (_pos < _input.size() && _input[_pos] != '"') {
		if (_input[_pos] == '\\')
			_pos++; // Handle escape sequences
		ss << _input[_pos++];
	}
	_pos++; // Skip the closing quote
	return {TokenType::String, ss.str()};
}

Token Lexer::_otherTokens() {
	if (std::isdigit(_input[_pos]) || _input[_pos] == '-') {
		return _numberToken();
	} else if (_input.substr(_pos, 4) == "true") {
		_pos += 4;
		return {TokenType::True, "true"};
	} else if (_input.substr(_pos, 5) == "false") {
		_pos += 5;
		return {TokenType::False, "false"};
	} else if (_input.substr(_pos, 4) == "null") {
		_pos += 4;
		return {TokenType::Null, "null"};
	} else {
		throw std::runtime_error("Unexpected character in input");
	}
}

Token Lexer::_numberToken() {
	std::stringstream ss;
	if (_input[_pos] == '-')
		ss << _input[_pos++];
	while (_pos < _input.size() && std::isdigit(_input[_pos])) {
		ss << _input[_pos++];
	}
	if (_pos < _input.size() && _input[_pos] == '.') {
		ss << _input[_pos++];
		while (_pos < _input.size() && std::isdigit(_input[_pos])) {
			ss << _input[_pos++];
		}
	}
	return {TokenType::Number, ss.str()};
}


Parser::Parser(const std::string &input) : _lexer(input) {
	_currentToken = _lexer.nextToken();
}

void Parser::parse(Value &out) {
	return _parseValue(out);
}

void Parser::_parseValue(Value &out) {
	switch (_currentToken.type) {
	case TokenType::LeftBrace: return _parseObject(out);
	case TokenType::LeftBracket: return _parseArray(out);
	case TokenType::String:
	case TokenType::Number:
	case TokenType::True:
	case TokenType::False:
	case TokenType::Null: return _parsePrimitive(out);
	default: throw std::runtime_error("Unexpected token in input");
	}
}

void Parser::_parseObject(Value &out) {
	out.value = Object();
	Object &object(std::get<Object>(out.value));
	_consume(TokenType::LeftBrace);
	while (_currentToken.type != TokenType::RightBrace) {
		const std::string key = _currentToken.value;
		_consume(TokenType::String);
		_consume(TokenType::Colon);
		_parseValue(object[key]);
		if (_currentToken.type == TokenType::Comma) {
			_consume(TokenType::Comma);
		} else {
			break;
		}
	}
	_consume(TokenType::RightBrace);
}

void Parser::_parseArray(Value &out) {
	out.value = Array();
	Array &array(std::get<Array>(out.value));
	_consume(TokenType::LeftBracket);
	while (_currentToken.type != TokenType::RightBracket) {
		_parseValue(array.emplace_back());
		if (_currentToken.type == TokenType::Comma) {
			_consume(TokenType::Comma);
		} else {
			break;
		}
	}
	_consume(TokenType::RightBracket);
}

void Parser::_parsePrimitive(Value &out) {
	switch (_currentToken.type) {
	case TokenType::String: out.value = _currentToken.value; break;
	case TokenType::Number: out.value = std::stod(_currentToken.value); break;
	case TokenType::True: out.value = true; break;
	case TokenType::False: out.value = false; break;
	case TokenType::Null: out.value = std::nullptr_t(); break;
	default: throw std::runtime_error("Unexpected token in input");
	}
	_currentToken = _lexer.nextToken();
}

void Parser::_consume(TokenType expected) {
	if (_currentToken.type != expected) {
		throw std::runtime_error("Unexpected token in input");
	}
	_currentToken = _lexer.nextToken();
}


std::string Serializer::serialize(const Value &value) {
	std::visit(*this, value.value);
	return _ss.str();
}

void Serializer::operator()(const Object &object) {
	_ss << "{";
	bool first = true;
	for (const auto &pair : object) {
		if (!first)
			_ss << ",";
		_ss << "\"" << pair.first << "\":";
		std::visit(*this, pair.second.value);
		first = false;
	}
	_ss << "}";
}

void Serializer::operator()(const Array &array) {
	_ss << "[";
	bool first = true;
	for (const auto &item : array) {
		if (!first)
			_ss << ",";
		std::visit(*this, item.value);
		first = false;
	}
	_ss << "]";
}

void Serializer::operator()(const std::string &str) {
	_ss << "\"" << escape_string(str) << "\"";
}

void Serializer::operator()(double num) {
	_ss << num;
}

void Serializer::operator()(bool b) {
	_ss << (b ? "true" : "false");
}

void Serializer::operator()(std::nullptr_t) {
	_ss << "null";
}

} // namespace Stone::Json
