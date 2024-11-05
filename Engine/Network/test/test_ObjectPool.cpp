#include "Network/ObjectPool.hpp"

#include <gtest/gtest.h>

using namespace Stone::Network;

using PoolId = Stone::Network::ObjectPool<int>::Id;

TEST(ObjectPool, AddObject) {
	ObjectPool<int> pool;
	auto object = std::make_shared<int>(42);
	PoolId id = pool.add(object);
	auto result = pool.get(id);
	ASSERT_EQ(*result, 42);
}

TEST(ObjectPool, AddMultipleObjects) {
	ObjectPool<int> pool;
	auto object1 = std::make_shared<int>(42);
	auto object2 = std::make_shared<int>(43);
	PoolId id1 = pool.add(object1);
	PoolId id2 = pool.add(object2);
	auto result1 = pool.get(id1);
	auto result2 = pool.get(id2);
	ASSERT_EQ(*result1, 42);
	ASSERT_EQ(*result2, 43);
}

TEST(ObjectPool, SetObject) {
	ObjectPool<int> pool;
	auto object = std::make_shared<int>(42);
	PoolId id = pool.add(object);
	auto result = pool.get(id);
	ASSERT_EQ(*result, 42);

	auto newObject = std::make_shared<int>(43);
	pool.set(id, newObject);
	auto newResult = pool.get(id);
	ASSERT_EQ(*newResult, 43);
}

TEST(ObjectPool, SetMultipleObjects) {
	ObjectPool<int> pool;
	auto object1 = std::make_shared<int>(42);
	auto object2 = std::make_shared<int>(43);
	PoolId id1 = pool.add(object1);
	PoolId id2 = pool.add(object2);
	auto result1 = pool.get(id1);
	auto result2 = pool.get(id2);
	ASSERT_EQ(*result1, 42);
	ASSERT_EQ(*result2, 43);

	auto newObject1 = std::make_shared<int>(44);
	auto newObject2 = std::make_shared<int>(45);
	pool.set(id1, newObject1);
	pool.set(id2, newObject2);
	auto newResult1 = pool.get(id1);
	auto newResult2 = pool.get(id2);
	ASSERT_EQ(*newResult1, 44);
	ASSERT_EQ(*newResult2, 45);
}

TEST(ObjectPool, RefreshId) {
	ObjectPool<int> pool;

	auto object1 = std::make_shared<int>(42);
	auto object2 = std::make_shared<int>(43);
	PoolId id1 = pool.add(object1);
	PoolId id2 = pool.add(object2);
	ASSERT_EQ(id1, 1);
	ASSERT_EQ(id2, 2);

	{
		auto result1 = pool.get(id1);
		auto result2 = pool.get(id2);
		ASSERT_EQ(*result1, 42);
		ASSERT_EQ(*result2, 43);
	}

	object1.reset();
	object2.reset();
	ASSERT_EQ(pool.get(id1), nullptr);
	ASSERT_EQ(pool.get(id2), nullptr);

	auto object3 = std::make_shared<int>(44);
	PoolId id3 = pool.add(object3);
	ASSERT_EQ(id3, 3);

	{
		auto result3 = pool.get(id3);
		ASSERT_EQ(*result3, 44);
	}

	pool.refreshId();
	auto newObject1 = std::make_shared<int>(44);
	auto newObject2 = std::make_shared<int>(45);
	PoolId newId1 = pool.add(newObject1);
	PoolId newId2 = pool.add(newObject2);
	ASSERT_EQ(newId1, 1);
	ASSERT_EQ(newId2, 2);
}
