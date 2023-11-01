export module recpp.async.Schedulable;

import <functional>;

using namespace std;

export namespace recpp
{
	class Schedulable
	{
	public:
		Schedulable(const function<void(void)> &function)
			: m_task(function)
		{
		}

		void operator()()
		{
			if (m_task && !m_canceled)
				m_task();
		}

		void cancel()
		{
			m_canceled = true;
		}

	private:
		function<void(void)> m_task;
		bool				 m_canceled = false;
	};
} // namespace recpp
