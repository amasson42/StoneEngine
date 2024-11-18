#include "Utils/StringExt.hpp"

#include <gtest/gtest.h>


TEST(StringExt, StringEndsWith) {
	std::string input = "This is a test string";

	EXPECT_TRUE(string_ends_with(input, "string"));
	EXPECT_FALSE(string_ends_with(input, "test"));

	EXPECT_TRUE(string_ends_with(input, "g"));
	EXPECT_FALSE(string_ends_with(input, "g "));

	EXPECT_TRUE(string_ends_with(input, ""));
}

TEST(StringExt, StingStartsWith) {
	std::string input = "This is a test string";

	EXPECT_TRUE(string_starts_with(input, "This"));
	EXPECT_FALSE(string_starts_with(input, "test"));

	EXPECT_TRUE(string_starts_with(input, "T"));
	EXPECT_FALSE(string_starts_with(input, " T"));

	EXPECT_TRUE(string_starts_with(input, ""));
}

TEST(StringExt, StringContainsNonPrintable) {
	std::string input = "This is a test string";

	EXPECT_FALSE(string_contains_non_printable(input));

	input.push_back('\0');
	EXPECT_TRUE(string_contains_non_printable(input));
}

TEST(StringExt, IntToHex) {
	EXPECT_EQ(int_to_hex(0), "00");
	EXPECT_EQ(int_to_hex(1), "01");
	EXPECT_EQ(int_to_hex(10), "0A");
	EXPECT_EQ(int_to_hex(15), "0F");
	EXPECT_EQ(int_to_hex(16), "10");
	EXPECT_EQ(int_to_hex(255), "FF");
}

TEST(StringExt, EscapeString) {
	std::string input = R"(This
is a\test "string")";
	std::string expected = R"(This\nis a\\test \"string\")";

	EXPECT_EQ(escape_string(input), expected);
}

TEST(StringExt, BytesToBase64String) {
	auto test = [](std::string input, std::string expected) {
		std::string output;
		bytes_to_base64(reinterpret_cast<const u_int8_t *>(input.data()), input.size(), output);
		EXPECT_EQ(output, expected);
	};

	test("This is a test string", "VGhpcyBpcyBhIHRlc3Qgc3RyaW5n");
	test("\x1F\x8A", "H4o=");
}

TEST(StringExt, Base64StringToBytes) {
	auto test = [](std::string expected, std::string input) {
		std::vector<u_int8_t> output;
		base64_to_bytes(input, output);

		EXPECT_EQ(std::string(reinterpret_cast<const char *>(output.data()), output.size()), expected);
	};

	test("This is a test string", "VGhpcyBpcyBhIHRlc3Qgc3RyaW5n");
	test("\x1F\x8A", "H4o=");
}
