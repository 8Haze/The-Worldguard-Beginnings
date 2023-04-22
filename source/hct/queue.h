#ifndef _HCT_QUEUE_H_
#define _HCT_QUEUE_H_

#include "deck.h"

namespace hct
{

	template <typename T, typename Container = hct::deck<T>>
	class queue
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

		static_assert(std::is_same_v<T, value_type>, "it is required that the queue's and it's wrapped container's stored value types are matched");

	private:

		// -------------------------------------------------------------------------------------------------------------------------
		// Member Data
		// -------------------------------------------------------------------------------------------------------------------------

		Container container{};

	public:

		// -------------------------------------------------------------------------------------------------------------------------
		// Constructors
		// -------------------------------------------------------------------------------------------------------------------------

		queue() = default;

		queue(const queue& arg_other) :
			container(arg_other.container) {}

		queue(queue&& arg_other) noexcept : 
			container(std::move(arg_other.container)) {}

		explicit queue(const Container& arg_container) :
			container(arg_container) {}

		explicit queue(Container&& arg_container) noexcept(std::is_nothrow_move_constructible_v<Container>) :
			container(std::move(arg_container)) {}

		template <class Iter>
		queue(Iter arg_iBegin, Iter arg_iEnd)
		{
			for (; arg_iBegin != arg_iEnd; ++arg_iBegin)
				container.push_back(*arg_iBegin);
		}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		explicit queue(const Alloc& arg_allocator) noexcept(std::is_nothrow_constructible_v<Container, const Alloc&>) : 
			container(arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		queue(const Container& arg_container, const Alloc& arg_allocator) : 
			container(arg_container, arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		queue(Container&& arg_container, const Alloc& arg_allocator) noexcept(std::is_nothrow_constructible_v<Container, Container, const Alloc&>) :
			container(arg_container, arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		queue(const queue& arg_other, const Alloc& arg_allocator) : 
			container(arg_other.container, arg_allocator) {}

		template <class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		queue(queue&& arg_other, const Alloc& arg_allocator) noexcept(std::is_nothrow_constructible_v<Container, Container, const Alloc&>) :
			container(std::move(arg_other.container), arg_allocator) {}

		template <class Iter, class Alloc, std::enable_if_t<std::uses_allocator_v<Container, Alloc>, int> = 0>
		queue(Iter arg_iBegin, Iter arg_iEnd, const Alloc& arg_allocator) : 
			container(arg_allocator)
		{
			for (; arg_iBegin != arg_iEnd; ++arg_iBegin)
				container.push_back(*arg_iBegin);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Assignment Operators
		// -------------------------------------------------------------------------------------------------------------------------

		queue& operator=(const queue& other)
		{
			container.clear();
			container = other.container;
		}

		queue& operator=(queue&& other)
		{
			container.clear();
			container = std::move(other.container);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Element & state access
		// -------------------------------------------------------------------------------------------------------------------------

		reference front() noexcept(noexcept(container.front()))
		{
			return container.front();
		}

		const_reference front() const noexcept(noexcept(container.front()))
		{
			return container.front();
		}

		reference back() noexcept(noexcept(container.back()))
		{
			return container.back();
		}

		const_reference back() const noexcept(noexcept(container.back()))
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

		void pop() noexcept(noexcept(container.pop_front()))
		{
			container.pop_front();
		}

		void clear() noexcept(noexcept(container.clear()))
		{
			container.clear();
		}

		void swap(queue& other) noexcept(noexcept(container.swap(other.container)))
		{
			container.swap(other.container);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Comparison Operators
		// -------------------------------------------------------------------------------------------------------------------------

		friend bool operator<(const queue& lhs, const queue& rhs) noexcept
		{
			return lhs.container < rhs.container;
		}

		friend bool operator>(const queue& lhs, const queue& rhs) noexcept
		{
			return lhs.container > rhs.container;
		}

		friend bool operator<=(const queue& lhs, const queue& rhs) noexcept
		{
			return lhs.container <= rhs.container;
		}

		friend bool operator>=(const queue& lhs, const queue& rhs) noexcept
		{
			return lhs.container >= rhs.container;
		}

		friend bool operator==(const queue& lhs, const queue& rhs) noexcept
		{
			return lhs.container == rhs.container;
		}

		friend bool operator!=(const queue& lhs, const queue& rhs) noexcept
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
	inline void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

};

#endif // _HCT_QUEUE_H_