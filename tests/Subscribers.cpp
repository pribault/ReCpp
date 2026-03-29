#include "fixtures/SubscriberBasedTest.h"

#include <recpp/async/Scheduler.h>
#include <recpp/processors/AllOf.h>
#include <recpp/processors/AndThen.h>
#include <recpp/processors/AnyOf.h>
#include <recpp/processors/Count.h>
#include <recpp/processors/DefaultIfEmpty.h>
#include <recpp/processors/Delay.h>
#include <recpp/processors/ElementAt.h>
#include <recpp/processors/Filter.h>
#include <recpp/processors/First.h>
#include <recpp/processors/FlatMap.h>
#include <recpp/processors/IgnoreElements.h>
#include <recpp/processors/Last.h>
#include <recpp/processors/Map.h>
#include <recpp/processors/Max.h>
#include <recpp/processors/Min.h>
#include <recpp/processors/NoneOf.h>
#include <recpp/processors/Reduce.h>
#include <recpp/processors/SwitchOnError.h>
#include <recpp/processors/Take.h>
#include <recpp/processors/TakeWhile.h>
#include <recpp/processors/Tap.h>
#include <recpp/publishers/JustPublisher.h>
#include <recpp/subscribers/DefaultSubscriber.h>

using namespace std;
using namespace recpp::async;
using namespace recpp::processors;
using namespace recpp::publishers;
using namespace recpp::subscribers;
using namespace rscpp;
using namespace testing;

class DefaultSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return DefaultSubscriber<int>(nullptr, nullptr, nullptr);
	}
};

TEST_F(DefaultSubscriberTest, checkAll)
{
	checkAll();
}

class AllOfSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return AllOf<int, function<bool(int)>>({}, nullptr);
	}
};

TEST_F(AllOfSubscriberTest, checkAll)
{
	checkAll();
}

class AndThenSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return AndThen<int, int>({}, {});
	}
};

TEST_F(AndThenSubscriberTest, checkAll)
{
	checkAll();
}

class AnyOfSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return AnyOf<int, function<bool(int)>>({}, nullptr);
	}
};

TEST_F(AnyOfSubscriberTest, checkAll)
{
	checkAll();
}

class CountSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Count<int>({});
	}
};

TEST_F(CountSubscriberTest, checkAll)
{
	checkAll();
}

class DefaultIfEmptySubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return DefaultIfEmpty<int>({}, {});
	}
};

TEST_F(DefaultIfEmptySubscriberTest, checkAll)
{
	checkAll();
}

class DelaySubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Delay<int, long, ratio<1>>({}, *m_scheduler, {}, true);
	}

private:
	unique_ptr<Scheduler> m_scheduler = make_unique<Scheduler>();
};

TEST_F(DelaySubscriberTest, checkAll)
{
	checkAll();
}

class ElementAtSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return ElementAt<int>({}, {});
	}
};

TEST_F(ElementAtSubscriberTest, checkAll)
{
	checkAll();
}

class FilterSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Filter<int>({}, {});
	}
};

TEST_F(FilterSubscriberTest, checkAll)
{
	checkAll();
}

class FirstSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return First<int>({});
	}
};

TEST_F(FirstSubscriberTest, checkAll)
{
	checkAll();
}

class FlatMapSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return FlatMap<int, function<Publisher<int>(int)>>({}, nullptr);
	}
};

TEST_F(FlatMapSubscriberTest, checkAll)
{
	checkAll();
}

class IgnoreElementsSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return IgnoreElements<int, int>({});
	}
};

TEST_F(IgnoreElementsSubscriberTest, checkAll)
{
	checkAll();
}

class LastSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Last<int>({});
	}
};

TEST_F(LastSubscriberTest, checkAll)
{
	checkAll();
}

class MapSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Map<int, function<int(int)>>({}, nullptr);
	}
};

TEST_F(MapSubscriberTest, checkAll)
{
	checkAll();
}

class MaxSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Max<int, function<int(int, int)>>({}, nullptr);
	}
};

TEST_F(MaxSubscriberTest, checkAll)
{
	checkAll();
}

class MinSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Min<int, function<int(int, int)>>({}, nullptr);
	}
};

TEST_F(MinSubscriberTest, checkAll)
{
	checkAll();
}

class NoneOfSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return NoneOf<int, function<bool(int)>>({}, nullptr);
	}
};

TEST_F(NoneOfSubscriberTest, checkAll)
{
	checkAll();
}

class ReduceSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Reduce<int, function<int(int, int)>>({}, {}, nullptr);
	}
};

TEST_F(ReduceSubscriberTest, checkAll)
{
	checkAll();
}

class SwitchOnErrorSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return SwitchOnError<int>({}, {});
	}
};

TEST_F(SwitchOnErrorSubscriberTest, checkAll)
{
	checkAll();
}

class TakeSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Take<int>({}, {});
	}
};

TEST_F(TakeSubscriberTest, checkAll)
{
	checkAll();
}

class TakeWhileSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return TakeWhile<int>({}, nullptr);
	}
};

TEST_F(TakeWhileSubscriberTest, checkAll)
{
	checkAll();
}

class TapSubscriberTest : public SubscriberBasedTest
{
public:
	Subscriber<int> subscriber() const override
	{
		return Tap<int>({}, nullptr, nullptr, nullptr);
	}
};

TEST_F(TapSubscriberTest, checkAll)
{
	checkAll();
}
