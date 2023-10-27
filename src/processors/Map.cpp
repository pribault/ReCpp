export module recpp.processors.Map;

import rscpp.Processor;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename R>
	class Map : public Processor<T, R>
	{
	};
} // namespace recpp
