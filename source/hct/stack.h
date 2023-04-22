#ifndef _HCT_STACK_H_
#define _HCT_STACK_H_

#include "dynarray.h"

namespace hct
{

	template <typename T, typename Container = hct::dynarray<T>>
	class stack
	{
	public:

		// -------------------------------------------------------------------------------------------------------------------------
		// Public Typedefs
		// -------------------------------------------------------------------------------------------------------------------------

		using container_type = Container;
		using value_type = typename Container::value_type;
		using size_type = typename Container::size_type;
		using pointer = typename Container::pointer;
		using const_pointer = typename Container::const_pointer;
		using reference = typename Container::reference;
		using const_reference = typename Container::const_reference;

		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;
		using reverse_iterator = typename Container::reverse_iterator;
		using const_reverse_iterator = typename Container::const_reverse_iterator;

		static_assert(std::is_same_v<T, value_type>, "it is required that the stack's and it's wrapped container's stored value types are matched");

	private:

		// -------------------------------------------------------------------------------------------------------------------------
		// Member Data
		// -------------------------------------------------------------------------------------------------------------------------

		Container container{};

	public:

		// -------------------------------------------------------------------------------------------------------------------------
		// Constructors
		// -------------------------------------------------------------------------------------------------------------------------

		stack() = default;

		stack(const stack& arg_other) : 
			container(arg_other.container) {}

		stack(stack&& arg_other) noexcept : 
			container(std::move(arg_other.container)) {}

		explicit stack(const Container& arg_container) :
			container(arg_container) {}

		explicit stack(Container&& arg_container) noexcept(std::is_nothrow_move_constructible_v<Container>) : 
			container(std::move(arg_container)) {}

		template <class Iter>
		stack(Iter arg_iBegin, Iter arg_iEnd)
		{
			for (; arg_iBegin != arg_iEnd; ++arg_iBegin)
				container.push_back(*arg_iBegin);
		}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		explicit stack(const Alloc& arg_allocator) noexcept(std::is_nothrow_constructible_v<Container, const Alloc&>) : 
			container(arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		stack(const Container& arg_container, const Alloc& arg_allocator) :
			container(arg_container, arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		stack(Container&& arg_container, const Alloc& arg_allocator) noexcept(std::is_nothrow_constructible_v<Container, Container, const Alloc&>) :
			container(arg_container, arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		stack(const stack& arg_other, const Alloc& arg_allocator) : 
			container(arg_other.container, arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		stack(stack&& arg_other, const Alloc& arg_allocator) noexcept(std::is_nothrow_constructible_v<Container, Container, const Alloc&>) : 
			container(std::move(arg_other.container), arg_allocator) {}

		template <class Iter, class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		stack(Iter arg_iBegin, Iter arg_iEnd, const Alloc& arg_allocator) : 
			container(arg_allocator)
		{
			for (; arg_iBegin != arg_iEnd; ++arg_iBegin)
				container.push_back(*arg_iBegin);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Assignment Operators
		// -------------------------------------------------------------------------------------------------------------------------

		stack& operator=(const stack& other)
		{
			container.clear();
			container = other.container;
		}

		stack& operator=(stack&& other)
		{
			container.clear();
			container = std::move(other.container);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Element & state access
		// -------------------------------------------------------------------------------------------------------------------------

		reference top() noexcept(noexcept(container.back()))
		{
			return container.back();
		}

		const_reference top() const noexcept(noexcept(container.back()))
		{
			return container.back();
		}

		size_type size() const noexcept(noexcept(container.size()))
		{
			return container.size();
		}

		bool empty() const noexcept(noexcept(container.empty()))
		{
			return container.empty();
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Modifiers
		// -------------------------------------------------------------------------------------------------------------------------

		void push(const T& value)
		{
			container.push_back(value);
		}

		void push(T&& value)
		{
			container.push_back(std::move(value));
		}

		template <typename... Args>
		decltype(auto) emplace(Args&&... args)
		{
			return container.emplace_back(std::forward<Args>(args)...);
		}

		void pop() noexcept(noexcept(container.pop_back()))
		{
			container.pop_back();
		}

		void clear() noexcept(noexcept(container.clear()))
		{
			container.clear();
		}

		void swap(stack& other) noexcept(noexcept(container.swap(other.container)))
		{
			container.swap(other.container);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Comparison Operators
		// -------------------------------------------------------------------------------------------------------------------------

		friend bool operator<(const stack& lhs, const stack& rhs) noexcept
		{
			return lhs.container < rhs.container;
		}

		friend bool operator>(const stack& lhs, const stack& rhs) noexcept
		{
			return lhs.container > rhs.container;
		}

		friend bool operator<=(const stack& lhs, const stack& rhs) noexcept
		{
			return lhs.container <= rhs.container;
		}

		friend bool operator>=(const stack& lhs, const stack& rhs) noexcept
		{
			return lhs.container >= rhs.container;
		}

		friend bool operator==(const stack& lhs, const stack& rhs) noexcept
		{
			return lhs.container == rhs.container;
		}

		friend bool operator!=(const stack& lhs, const stack& rhs) noexcept
		{
			return lhs.container != rhs.container;
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Iterators
		// -------------------------------------------------------------------------------------------------------------------------

		iterator begin() noexcept
		{
			return container.begin();
		}

		iterator end() noexcept
		{
			return container.end();
		}

		const_iterator begin() const noexcept
		{
			return container.begin();
		}

		const_iterator end() const noexcept
		{
			return container.end();
		}

		const_iterator cbegin() const noexcept
		{
			return container.cbegin();
		}

		const_iterator cend() const noexcept
		{
			return container.cend();
		}

		reverse_iterator rbegin() noexcept
		{
			return container.rbegin();
		}

		reverse_iterator rend() noexcept
		{
			return container.rend();
		}

		const_reverse_iterator rbegin() const noexcept
		{
			return container.rbegin();
		}

		const_reverse_iterator rend() const noexcept
		{
			return container.rend();
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return container.crbegin();
		}

		const_reverse_iterator crend() const noexcept
		{
			return container.crend();
		}

	};

	template <typename T, typename Container>
	inline void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

};

#endif // _HCT_STACK_H_