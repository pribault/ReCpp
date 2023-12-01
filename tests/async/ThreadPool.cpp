// gtest
#include <gtest/gtest.h>

// recpp
#include <recpp/async/ThreadPool.h>

// stl
#include <iostream>

using namespace recpp::async;
using namespace std;

TEST(ThreadPool, defaultConcurrency)
{
	ThreadPool threadPool;

	EXPECT_GE(thread::hardware_concurrency(), 0u);
	EXPECT_EQ(threadPool.size(), thread::hardware_concurrency());
}
