#pragma once

// gtest
#include <gtest/gtest.h>

// stl
#include <memory>

namespace recpp::async
{
	class EventLoop;
}

class EventLoopBasedTest : public testing::Test
{
public:
	~EventLoopBasedTest() override;

protected:
	void SetUp() override;
	void TearDown() override;

	std::unique_ptr<recpp::async::EventLoop> m_eventLoop;
};
