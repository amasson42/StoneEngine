// Copyright 2024 Stone-Engine

#include "Utils/Json.hpp"

#include "Utils/FileSystem.hpp"
#include "Utils/StringExt.hpp"

#include <cassert>
#include <fstream>
#include <sstream>


namespace Stone::Json {

void Value::serialize(std::ostream &stream) const {
	Serializer serializer(stream);
	return serializer.serialize(*this);
}

std::string Value::serialize() const {
	std::stringstream ss;
	serialize(ss);
	return ss.str();
}

void parseStream(std::istream &input, Value &out) {
	Parser parser(input);
	parser.parse(out);
}

void parseString(const std::string &input, Value &out) {
	std::istringstream stream(input);
	parseStream(stream, out);
}

void parseFile(const std::string &path, Value &out) {
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Failed to open file: " + path);
	parseStream(file, out);
	file.close();
}

Value object(const Object &obj) {
	return {obj};
}

Value array(const Array &arr) {
	return {arr};
}

Value string(const std::string &str) {
	return {str};
}

Value number(double num) {
	return {num};
}

Value boolean(bool b) {
	return {b};
}

Value null() {
	return {};
}


std::string to_string(TokenType type) {
	switch (type) {
	case TokenType::LeftBrace: return "LeftBrace '{'";
	case TokenType::RightBrace: return "RightBrace '}'";
	case TokenType::LeftBracket: return "LeftBracket '['";
	case TokenType::RightBracket: return "RightBracket ']'";
	case TokenType::Comma: return "Comma ','";
	case TokenType::Colon: return "Colon ':'";
	case TokenType::String: return "String";
	case TokenType::Number: return "Number";
	case TokenType::True: return "True";
	case TokenType::False: return "False";
	case TokenType::Null: return "Null";
	case TokenType::EndOfFile: return "EndOfFile";
	default: return "Unknown";
	}
}

std::ostream &operator<<(std::ostream &os, TokenType type) {
	return os << to_string(type);
}

Lexer::Lexer(std::istream &input) : _input(input) {
}

Token Lexer::nextToken() {

	if (_currentChar == ' ') {
		while (_input.get(_currentChar) && std::isspace(_currentChar))
			;
	}

	if (_input.eof())
		return {TokenType::EndOfFile, ""};

	switch (_currentChar) {
	case '{': _currentChar = ' '; return {TokenType::LeftBrace, "{"};
	case '}': _currentChar = ' '; return {TokenType::RightBrace, "}"};
	case '[': _currentChar = ' '; return {TokenType::LeftBracket, "["};
	case ']': _currentChar = ' '; return {TokenType::RightBracket, "]"};
	case ':': _currentChar = ' '; return {TokenType::Colon, ":"};
	case ',': _currentChar = ' '; return {TokenType::Comma, ","};
	case '"': return _stringToken();
	default: return _otherTokens();
	}
}

Token Lexer::_stringToken() {
	std::stringstream ss;
	while (_input.get(_currentChar) && _currentChar != '"') {
		if (_currentChar == '\\') {
			if (!_input.get(_currentChar))
				throw std::runtime_error("Unexpected end of input after \\");
		}
		ss << _currentChar;
	}
	if (_currentChar != '"')
		throw std::runtime_error("Unexpected end of input while parsing string");
	_currentChar = ' ';
	return {TokenType::String, ss.str()};
}

Token Lexer::_otherTokens() {
	auto expect = [&](std::string expected) {
		for (char c : expected) {
			if (!_input.get(_currentChar))
				throw std::runtime_error("Unexpected end of input");
			if (_currentChar != c)
				throw std::runtime_error("Expected token in input");
		}
	};
	if (std::isdigit(_currentChar) || _currentChar == '-') {
		return _numberToken();
	} else if (_currentChar == 't') {
		expect("rue");
		_currentChar = ' ';
		return {TokenType::True, "true"};
	} else if (_currentChar == 'f') {
		expect("alse");
		_currentChar = ' ';
		return {TokenType::False, "false"};
	} else if (_currentChar == 'n') {
		expect("ull");
		_currentChar = ' ';
		return {TokenType::Null, "null"};
	} else {
		throw std::runtime_error(std::string("Unexpected character ") + _currentChar + " in input");
	}
}

Token Lexer::_numberToken() {
	std::stringstream ss;
	do {
		ss << _currentChar;
	} while (_input.get(_currentChar) && (std::isdigit(_currentChar) || _currentChar == '.'));
	return {TokenType::Number, ss.str()};
}


Parser::Parser(std::istream &input) : _lexer(input) {
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
	auto &object(std::get<Object>(out.value));
	_consume(TokenType::LeftBrace);
	while (_currentToken.type != TokenType::RightBrace) {
		const std::string key = _currentToken.value; // Store the key before consuming the tokens
		_consume(TokenType::String);
		_consume(TokenType::Colon);
		Value value;
		_parseValue(value);
		object[key] = std::move(value);
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
	auto &array(std::get<Array>(out.value));
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
		throw std::runtime_error("Expected " + to_string(expected) + ", but got " + _currentToken.value);
	}
	_currentToken = _lexer.nextToken();
}


Serializer::Serializer(std::ostream &stream) : _stream(stream) {
}

void Serializer::serialize(const Value &value) {
	std::visit(*this, value.value);
}

void Serializer::operator()(const Object &object) {
	_stream << "{";
	bool first = true;
	for (const auto &pair : object) {
		if (!first)
			_stream << ",";
		_stream << "\"" << pair.first << "\":";
		std::visit(*this, pair.second.value);
		first = false;
	}
	_stream << "}";
}

void Serializer::operator()(const Array &array) {
	_stream << "[";
	bool first = true;
	for (const auto &item : array) {
		if (!first)
			_stream << ",";
		std::visit(*this, item.value);
		first = false;
	}
	_stream << "]";
}

void Serializer::operator()(const std::string &str) {
	_stream << "\"" << escape_string(str) << "\"";
}

void Serializer::operator()(double num) {
	_stream << num;
}

void Serializer::operator()(bool b) {
	_stream << (b ? "true" : "false");
}

void Serializer::operator()(std::nullptr_t) {
	_stream << "null";
}

std::ostream &operator<<(std::ostream &os, const Value &value) {
	value.serialize(os);
	return os;
}

} // namespace Stone::Json
