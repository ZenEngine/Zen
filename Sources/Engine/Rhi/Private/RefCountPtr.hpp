#pragma once
#include <Core/Misc/Assert.hpp>

namespace zen
{
	template<typename T>
	concept Releasable = requires(T & x)
	{
		x.Release();
	};

	template<Releasable T>
	void safeRelease(T*& ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	template<typename T>
	concept RefCountable = requires(T & x)
	{
		x.AddRef();
		x.Release();
	};

	template<typename T>
	class TRefCountPtr final
	{
	public:
		TRefCountPtr() noexcept
			: _ptr{ nullptr }
		{
		}

		template<typename U>
		TRefCountPtr(U* ptr, bool addRef = true) noexcept
			: _ptr{ ptr }
		{
			if (_ptr && addRef)
			{
				_ptr->AddRef();
			}
		}

		constexpr TRefCountPtr(decltype(nullptr)) noexcept
			: _ptr{ nullptr }
		{
		}


		constexpr TRefCountPtr(const TRefCountPtr& other) noexcept
			: _ptr{ other._ptr }
		{
			addRef();
		}

		template<typename U>
		TRefCountPtr(const TRefCountPtr<U>& other) noexcept
			: _ptr{ other._ptr }
		{
			addRef();
		}

		template<typename U>
		TRefCountPtr(TRefCountPtr<U>&& other) noexcept
			: _ptr{ other._ptr }
		{
			other._ptr = nullptr;
		}

		~TRefCountPtr() noexcept
		{
			release();
		}

		TRefCountPtr& operator=(decltype(nullptr)) noexcept
		{
			release();
			return *this;
		}

		TRefCountPtr& operator=(const TRefCountPtr& other) noexcept
		{
			if (_ptr != other._ptr)
			{
				TRefCountPtr(other).swap(*this);
			}
			return *this;
		}

		template<typename U>
		TRefCountPtr& operator=(const TRefCountPtr<U>& other) noexcept
		{
			TRefCountPtr(other).swap(*this);
			return *this;
		}

		template<typename U>
		TRefCountPtr& operator=(TRefCountPtr<U>&& other) noexcept
		{
			TRefCountPtr(other).swap(*this);
			return *this;
		}

		[[nodiscard]] bool isValid() const noexcept
		{
			return _ptr != nullptr;
		}

		[[nodiscard]] T* operator->() const noexcept
		{
			ZEN_ASSERT_SLOW(isValid());
			return _ptr;
		}

		[[nodiscard]] explicit operator bool() const noexcept
		{
			return _ptr != nullptr;
		}

		[[nodiscard]] T* get() const noexcept
		{
			return _ptr;
		}

		void reset()
		{
			release();
		}

		[[nodiscard]] T* const* getAddress() const noexcept
		{
			return &_ptr;
		}

		[[nodiscard]] T** getAddress() noexcept
		{
			return &_ptr;
		}

		[[nodiscard]] T** getInitAddress() noexcept
		{
			*this = nullptr;
			return &_ptr;
		}

		[[nodiscard]] T* detach() noexcept
		{
			T* ptr{ _ptr };
			_ptr = nullptr;
			return ptr;
		}

		void swap(TRefCountPtr& ptr) noexcept
		{
			T* temp{ _ptr };
			_ptr = ptr._ptr;
			ptr._ptr = temp;
		}

	private:
		void addRef()
		{
			if (_ptr != nullptr)
			{
				_ptr->AddRef();
			}
		}

		void release()
		{
			T* temp = _ptr;
			if (temp != nullptr)
			{
				_ptr = nullptr;
				temp->Release();
			}
		}

		template<typename U>
		friend class TRefCountPtr;

		T* _ptr;
	};

	template<typename T, typename U>
	[[nodiscard]] inline bool operator==(const TRefCountPtr<T>& lhs, const TRefCountPtr<U>& rhs) noexcept
	{
		return lhs.get() == rhs.get();
	}

	template<typename T, typename U>
	[[nodiscard]] inline bool operator!=(const TRefCountPtr<T>& lhs, const TRefCountPtr<U>& rhs) noexcept
	{
		return lhs.get() != rhs.get();
	}

	template<typename T>
	[[nodiscard]] inline bool operator==(const TRefCountPtr<T>& lhs, decltype(nullptr)) noexcept
	{
		return lhs.get() == nullptr;
	}

	template<typename T>
	[[nodiscard]] inline bool operator!=(const TRefCountPtr<T>& lhs, decltype(nullptr)) noexcept
	{
		return lhs.get() != nullptr;
	}

	template<typename T>
	[[nodiscard]] inline bool operator==(decltype(nullptr), const TRefCountPtr<T>& rhs) noexcept
	{
		return nullptr == rhs.get();
	}

	template<typename T>
	[[nodiscard]] inline bool operator!=(decltype(nullptr), const TRefCountPtr<T>& rhs) noexcept
	{
		return nullptr != rhs.get();
	}

	template<typename T>
	[[nodiscard]] inline bool operator==(const TRefCountPtr<T>& lhs, T* rhs) noexcept
	{
		return lhs.get() == rhs;
	}

	template<typename T, typename U>
	[[nodiscard]] inline bool operator!=(const TRefCountPtr<T>& lhs, T* rhs) noexcept
	{
		return lhs.get() != rhs;
	}

	template<typename T>
	[[nodiscard]] inline bool operator==(T* lhs, const TRefCountPtr<T>& rhs) noexcept
	{
		return lhs == rhs.get();
	}

	template<typename T>
	[[nodiscard]] inline bool operator!=(T* lhs, const TRefCountPtr<T>& rhs) noexcept
	{
		return lhs != rhs.get();
	}

	template<typename T, typename U>
	inline TRefCountPtr<T> staticPointerCast(const TRefCountPtr<U>& rhs) noexcept
	{
		return static_cast<T*>(rhs.get());
	}
}