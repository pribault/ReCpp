#include "fixtures/EventLoopBasedTest.h"

// recpp
#include <recpp/async/EventLoop.h>

using namespace recpp::async;
using namespace std;

EventLoopBasedTest::~EventLoopBasedTest() = default;

void EventLoopBasedTest::SetUp()
{
	m_eventLoop = make_unique<EventLoop>();
}

void EventLoopBasedTest::TearDown()
{
	m_eventLoop.reset();
}
