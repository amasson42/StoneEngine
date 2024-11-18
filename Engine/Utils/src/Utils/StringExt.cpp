// Copyright 2024 Stone-Engine

#include "Utils/StringExt.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>


bool string_ends_with(const std::string &str, const std::string &suffix) {
	if (str.size() < suffix.size()) {
		return false;
	}
	return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool string_starts_with(const std::string &str, const std::string &prefix) {
	if (str.size() < prefix.size()) {
		return false;
	}
	return str.compare(0, prefix.size(), prefix) == 0;
}

bool string_contains_non_printable(const std::string &str) {
	return std::any_of(str.begin(), str.end(), [](char c) {
		return !std::isprint(static_cast<unsigned char>(c)) && !std::isspace(static_cast<unsigned char>(c));
	});
}

std::string int_to_hex(unsigned char c, int width) {
	std::stringstream ss;
	ss << std::hex << std::uppercase;
	ss << std::setw(width) << std::setfill('0') << static_cast<int>(c);
	return ss.str();
}

std::string escape_string(const std::string &str) {
	std::string result;
	for (char c : str) {
		switch (c) {
		case '\"': result += "\\\""; break;
		case '\\': result += "\\\\"; break;
		case '\b': result += "\\b"; break;
		case '\f': result += "\\f"; break;
		case '\n': result += "\\n"; break;
		case '\r': result += "\\r"; break;
		case '\t': result += "\\t"; break;
		default:
			if ('\x00' <= c && c <= '\x1f') {
				result += "\\u" + int_to_hex(static_cast<unsigned char>(c));
			} else {
				result += c;
			}
			break;
		}
	}
	return result;
}

const static std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void bytes_to_base64(const u_int8_t *bytes, std::size_t size, std::string &out) {
	out.reserve(out.size() + ((size + 2) / 3) * 4);

	for (std::size_t i = 0; i < size; i += 3) {
		u_int8_t b0, b1, b2;
		b0 = static_cast<u_int8_t>(bytes[i]);
		b1 = i + 1 < size ? static_cast<u_int8_t>(bytes[i + 1]) : 0;
		b2 = i + 2 < size ? static_cast<u_int8_t>(bytes[i + 2]) : 0;

		u_int8_t c0, c1, c2, c3;
		c0 = b0 >> 2;
		c1 = ((b0 & 0x03) << 4) | (b1 >> 4);
		c2 = ((b1 & 0x0f) << 2) | (b2 >> 6);
		c3 = b2 & 0x3f;

		out += base64_chars[c0];
		out += base64_chars[c1];
		out += i + 1 < size ? base64_chars[c2] : '=';
		out += i + 2 < size ? base64_chars[c3] : '=';
	}
}

void base64_to_bytes(const std::string &base64, std::vector<u_int8_t> &out) {
	out.reserve(out.size() + (base64.size() / 4) * 3);

	for (size_t i = 0; i < base64.size(); i += 4) {
		u_int8_t c0, c1, c2, c3;
		c0 = base64_chars.find(base64[i]);
		c1 = base64_chars.find(base64[i + 1]);
		c2 = base64[i + 2] == '=' ? 0 : base64_chars.find(base64[i + 2]);
		c3 = base64[i + 3] == '=' ? 0 : base64_chars.find(base64[i + 3]);

		u_int8_t b0, b1, b2;
		b0 = (c0 << 2) | (c1 >> 4);
		b1 = ((c1 & 0x0f) << 4) | (c2 >> 2);
		b2 = ((c2 & 0x03) << 6) | c3;

		out.push_back(b0);
		if (base64[i + 2] != '=') {
			out.push_back(b1);
		}
		if (base64[i + 3] != '=') {
			out.push_back(b2);
		}
	}
}
