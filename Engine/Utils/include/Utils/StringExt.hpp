// Copyright 2024 Stone-Engine

#pragma once

#include <string>
#include <vector>

bool string_ends_with(const std::string &str, const std::string &suffix);
bool string_starts_with(const std::string &str, const std::string &prefix);
bool string_contains_non_printable(const std::string &str);
std::string int_to_hex(unsigned char c, int width = 2);

void bytes_to_base64(const u_int8_t *bytes, std::size_t size, std::string &out);
void base64_to_bytes(const std::string &base64, std::vector<u_int8_t> &out);
