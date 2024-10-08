#pragma once

// gtest
#include <gtest/gtest.h>

// stl
#include <memory>

namespace recpp::async
{
	class WorkerThread;
}

class WorkerThreadBasedTest : public testing::Test
{
public:
	~WorkerThreadBasedTest() override;

protected:
	void SetUp() override;
	void TearDown() override;

	std::unique_ptr<recpp::async::WorkerThread> m_worker;
	std::thread::id								m_mainThreadId;
	std::thread::id								m_workerThreadId;
};
