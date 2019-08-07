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
		auto callback = 
			std::bind(std::forward<F>(i_f), std::forward<Args>(i_args)...);

		m_callback = std::function<void()> {
			// move callback to the lambda capture,
			// since we don't need it in this scope anymore
			[callback = std::move(callback)] { callback(); }
		};
	}

	~ScopedCaller()
	{
		if (m_uncaught_exceptions == std::uncaught_exceptions())
			m_callback();
	}
};
