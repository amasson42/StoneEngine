#include "Core/Assets/Bundle.hpp"

#include <gtest/gtest.h>

using namespace Stone::Core;

class MockResource : public Assets::Resource {

public:
	MockResource(const std::shared_ptr<Assets::Bundle> &bundle, const std::string &filename)
		: Resource(bundle, filename) {
	}

	const char *getClassName() const override {
		return "MockResource";
	}
};

TEST(Bundle, RootDirectory) {
	auto bundle = std::make_shared<Assets::Bundle>("test");
	EXPECT_EQ(bundle->getRootDirectory(), "test/");

	auto bundle2 = std::make_shared<Assets::Bundle>("/test");
	EXPECT_EQ(bundle2->getRootDirectory(), "/test/");

	auto bundle3 = std::make_shared<Assets::Bundle>("");
	EXPECT_EQ(bundle3->getRootDirectory(), "./");

	auto bundle4 = std::make_shared<Assets::Bundle>();
	EXPECT_EQ(bundle4->getRootDirectory(), "./");
}

TEST(Bundle, LoadFile) {
	auto bundle = std::make_shared<Assets::Bundle>();

	auto fileMock = bundle->loadResource<MockResource>("file.txt");

	EXPECT_EQ(fileMock->getFilename(), "file.txt");
	EXPECT_EQ(fileMock->getBundle(), bundle);
	EXPECT_EQ(fileMock->getSubDirectory(), "./");
	EXPECT_EQ(bundle->getResource("file.txt"), fileMock);
}

TEST(Bundle, LoadMultipleFiles) {
	auto bundle = std::make_shared<Assets::Bundle>();

	auto fileMock = bundle->loadResource<MockResource>("file.txt");
	auto fileMock2 = bundle->loadResource<MockResource>("file2.png");

	EXPECT_EQ(fileMock->getFilename(), "file.txt");
	EXPECT_EQ(fileMock->getBundle(), bundle);
	EXPECT_EQ(fileMock->getSubDirectory(), "./");
	EXPECT_EQ(bundle->getResource("file.txt"), fileMock);

	EXPECT_EQ(fileMock2->getFilename(), "file2.png");
	EXPECT_EQ(fileMock2->getBundle(), bundle);
	EXPECT_EQ(fileMock2->getSubDirectory(), "./");
	EXPECT_EQ(bundle->getResource("file2.png"), fileMock2);
}

TEST(Bundle, LoadFileWithSubDirectory) {
	auto bundle = std::make_shared<Assets::Bundle>();

	auto fileMock = bundle->loadResource<MockResource>("subdir/file.txt");

	EXPECT_EQ(fileMock->getFilename(), "file.txt");
	EXPECT_EQ(fileMock->getBundle(), bundle);
	EXPECT_EQ(fileMock->getSubDirectory(), "subdir/");
	EXPECT_EQ(bundle->getResource("subdir/file.txt"), fileMock);
}

TEST(Bundle, LoadMultipleFilesWithSubDirectory) {
	auto bundle = std::make_shared<Assets::Bundle>();

	auto fileMock = bundle->loadResource<MockResource>("subdir/file.txt");
	auto fileMock2 = bundle->loadResource<MockResource>("subdir/file2.png");

	EXPECT_EQ(fileMock->getFilename(), "file.txt");
	EXPECT_EQ(fileMock->getBundle(), bundle);
	EXPECT_EQ(fileMock->getSubDirectory(), "subdir/");
	EXPECT_EQ(bundle->getResource("subdir/file.txt"), fileMock);

	EXPECT_EQ(fileMock2->getFilename(), "file2.png");
	EXPECT_EQ(fileMock2->getBundle(), bundle);
	EXPECT_EQ(fileMock2->getSubDirectory(), "subdir/");
	EXPECT_EQ(bundle->getResource("subdir/file2.png"), fileMock2);
}

TEST(Bundle, ReducePath) {
	auto reduced = Assets::Bundle::reducePath("subdir/file.txt");
	EXPECT_EQ(Assets::Bundle::reducePath("subdir//./file.txt"), reduced);
	EXPECT_EQ(Assets::Bundle::reducePath("subdir/nowhere/../file.txt"), reduced);
	EXPECT_EQ(Assets::Bundle::reducePath("./subdir/nowhere/../file.txt"), reduced);
	EXPECT_EQ(Assets::Bundle::reducePath("././././/././//subdir/file.txt"), reduced);
}

TEST(Bundle, LoadSameFileFromDifferentPath) {
	auto bundle = std::make_shared<Assets::Bundle>();

	auto fileMock = bundle->loadResource<MockResource>("subdir/file.txt");
	auto fileMock2 = bundle->getResource("subdir//./file.txt");
	auto fileMock3 = bundle->getResource("subdir/nowhere/../file.txt");

	EXPECT_EQ(fileMock.get(), fileMock2.get());
	EXPECT_EQ(fileMock.get(), fileMock3.get());
}
