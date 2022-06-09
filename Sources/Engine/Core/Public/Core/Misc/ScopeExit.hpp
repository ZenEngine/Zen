#pragma once
#include <utility>

namespace zen
{
	template<typename Func>
	class TScopeExit
	{
	public:
		explicit TScopeExit(Func&& function)
			: _function{ std::forward<Func>(function) }
			, _executeOnDestruction{ true }
		{
		}

		TScopeExit(TScopeExit&& other)
			: _function{ std::move(other._function) }
			, _executeOnDestruction{ other._executeOnDestruction }
		{
			other.release();
		}

		TScopeExit(const TScopeExit&) = delete;
		TScopeExit& operator=(const TScopeExit&) = delete;
		TScopeExit& operator=(TScopeExit&&) = delete;

		~TScopeExit() noexcept
		{
			if (_executeOnDestruction)
			{
				_function();
			}
		}

		void release() noexcept
		{
			_executeOnDestruction = false;
		}

	private:
		Func _function;
		bool _executeOnDestruction;
	};

	template<typename Func>
	TScopeExit<Func> makeScopeExit(Func&& function)
	{
		return TScopeExit<Func>(std::forward<Func>(function));
	}
}
