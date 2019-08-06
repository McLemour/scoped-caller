#pragma once

#include <functional>
#include <future>


class ScopedCaller
{
private:
	std::function<void()> m_callback;
	int m_uncaught_exceptions;

public:

	template<typename F, typename... Args>
	ScopedCaller(F&& i_f, Args&&... i_args)
		: m_uncaught_exceptions{ std::uncaught_exceptions() }
	{
		auto callback = std::make_shared < std::function<std::result_of<F(Args...)>::type()> >(
			std::bind(std::forward<F>(i_f), std::forward<Args>(i_args)...)
			);

		m_callback = [callback] { (*callback)(); };
	}

	~ScopedCaller()
	{
		if (m_uncaught_exceptions == std::uncaught_exceptions())
			m_callback();
	}
};