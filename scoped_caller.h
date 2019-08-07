#pragma once

#include <functional>
#include <future>

class ScopedCaller
{
private:
	std::function<void()> m_callback;
	int m_uncaught_exceptions;

public:

	ScopedCaller()
		: m_uncaught_exceptions{ std::uncaught_exceptions() }
	{ }

	template<typename F, typename... Args>
	decltype(auto) Do(F&& i_f, Args&&... i_args)
	{
		using return_type = typename std::result_of<F(Args...)>::type;

		auto callback = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(i_f), std::forward<Args>(i_args)...)
			);

		std::future<return_type> result = callback->get_future();

		m_callback = [callback = std::move(callback)] { (*callback)(); };

		return result;
	}

	void Reset()
	{
		m_callback = [] {};
	}

	void Release()
	{
		if (m_uncaught_exceptions == std::uncaught_exceptions())
		{
			m_callback();
			m_callback = [] {};
		}
	}

	~ScopedCaller()
	{
		if (m_uncaught_exceptions == std::uncaught_exceptions())
			m_callback();
	}
};