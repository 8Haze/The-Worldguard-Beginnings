#ifndef _HCT_DYNARRAY_H_
#define _HCT_DYNARRAY_H_

#include <memory>
#include <type_traits>
#include <iterator>
#include <stdexcept>

namespace hct
{

	template <typename T, typename Allocator = std::allocator<T>>
	class dynarray
	{
	private:

		// -------------------------------------------------------------------------------------------------------------------------
		// Private Typedefs
		// -------------------------------------------------------------------------------------------------------------------------

		using allocator_traits = std::allocator_traits<Allocator>;

	public:

		// -------------------------------------------------------------------------------------------------------------------------
		// Public Typedefs
		// -------------------------------------------------------------------------------------------------------------------------

		using allocator_type = Allocator;
		using value_type = T;
		using size_type = typename allocator_traits::size_type;
		using difference_type = typename allocator_traits::difference_type;
		using pointer = typename allocator_traits::pointer;
		using const_pointer = typename allocator_traits::const_pointer;
		using reference = T&;
		using const_reference = const T&;

		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	private:

		// -------------------------------------------------------------------------------------------------------------------------
		// Member Data
		// -------------------------------------------------------------------------------------------------------------------------

		pointer data = nullptr;
		size_type idx_end = 0;
		size_type data_size = 0;
		allocator_type allocator;

		// -------------------------------------------------------------------------------------------------------------------------
		// Private Helper Functions
		// -------------------------------------------------------------------------------------------------------------------------

		pointer allocate(const size_t& desired_size)
		{
			return allocator_traits::allocate(allocator, desired_size);
		}

		void transfer(pointer& dest)
		{
			pointer ptr_dest = dest;
			try
			{
				if constexpr (std::is_trivially_copyable_v<value_type>)
					memcpy(ptr_dest, data, sizeof(value_type) * idx_end);
				else
					for (pointer ptr_data = data, ptr_data_end = data + idx_end; ptr_data < ptr_data_end; ++ptr_data, ++ptr_dest)
						allocator_traits::construct(allocator, ptr_dest, std::move_if_noexcept(*ptr_data));
			}
			catch (...)
			{
				if constexpr (!std::is_trivially_destructible_v<value_type>)
					for (pointer ptr_dest_destroy = dest; ptr_dest_destroy < ptr_dest; ++ptr_dest_destroy)
						allocator_traits::destroy(allocator, ptr_dest_destroy);
				throw;
			}
			if constexpr (!std::is_trivially_destructible_v<value_type>)
				for (pointer ptr_data = data, ptr_data_end = data + idx_end; ptr_data < ptr_data_end; ++ptr_data)
					allocator_traits::destroy(allocator, ptr_data);
		}

		void move_elements_backwards(iterator iBegin, iterator iEnd, typename std::iterator_traits<iterator>::difference_type offset)
		{
			auto distance = std::distance(iBegin, iEnd);

			idx_end += offset;

			iterator iLast = iBegin + distance;
			iterator iDest = iLast + offset;
			auto remaining_constructions = offset;

			while (iLast > iBegin)
			{
				--iDest;
				if (remaining_constructions > 0)
				{
					allocator_traits::construct(allocator, std::addressof(*iDest), std::move(*(--iLast)));
					--remaining_constructions;
				}
				else
					*iDest = std::move(*(--iLast));
			}
		}

		void reallocate(size_type desired_size = 0)
		{
			const size_type new_size = (desired_size == 0 || desired_size < idx_end) ? (data_size == 0 ? 1 : data_size + (data_size + 1) / 2) : desired_size; // fix to *2
			pointer temp = allocate(new_size);

			try
			{
				transfer(temp);
			}
			catch (...)
			{
				allocator_traits::deallocate(allocator, temp, new_size);
				throw;
			}

			allocator_traits::deallocate(allocator, data, data_size);
			data = temp;
			data_size = new_size;
		}

		void reset() noexcept
		{
			data = nullptr;
			idx_end = 0;
			data_size = 0;
		}

	public:

		// -------------------------------------------------------------------------------------------------------------------------
		// Constructors
		// -------------------------------------------------------------------------------------------------------------------------

		dynarray() noexcept(std::is_nothrow_default_constructible_v<Allocator>) :
			allocator(Allocator{}) {}

		explicit dynarray(const allocator_type& arg_allocator) noexcept :
			allocator(arg_allocator) {}

		explicit dynarray(size_type arg_n, const value_type& arg_value, const allocator_type& arg_allocator = Allocator{}) :
			allocator(arg_allocator)
		{
			resize(arg_n, arg_value);
		}

		explicit dynarray(size_type arg_n, const allocator_type& arg_allocator = Allocator{}) :
			allocator(arg_allocator)
		{
			resize(arg_n);
		}

		template <typename Iter, std::enable_if_t<std::input_iterator<Iter>, bool> = true>
		explicit dynarray(Iter arg_iBegin, Iter arg_iEnd, const allocator_type& arg_allocator = Allocator{}) :
			allocator(arg_allocator)
		{
			if constexpr (std::random_access_iterator<Iter>)
				reserve(std::distance(arg_iBegin, arg_iEnd));
			try
			{
				for (; arg_iBegin != arg_iEnd; ++arg_iBegin)
					emplace_back(*arg_iBegin);
			}
			catch (...)
			{
				clear();
				throw;
			}
		}

		dynarray(const dynarray& arg_other)
		{
			allocator = std::allocator_traits<allocator_type>::select_on_container_copy_construction(arg_other.allocator);
			reserve(arg_other.data_size);
			for (const auto& x : arg_other)
				push_back(x);
		}

		dynarray(const dynarray& arg_other, const allocator_type& arg_allocator) :
			allocator(arg_allocator)
		{
			reserve(arg_other.data_size);
			for (const auto& x : arg_other)
				push_back(x);
		}

		dynarray(dynarray&& arg_other) noexcept : 
			allocator(std::move(arg_other.allocator)),
			data(arg_other.data),
			idx_end(arg_other.idx_end), 
			data_size(arg_other.data_size)
		{
			arg_other.reset();
		}

		dynarray(dynarray&& arg_other, const allocator_type& arg_allocator) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value) :
			allocator(arg_allocator)
		{
			if constexpr (!std::allocator_traits<allocator_type>::is_always_equal::value)
				if (!(allocator == arg_other.allocator))
				{
					// Must perform full copy
					reserve(arg_other.data_size);
					for (const auto& x : arg_other)
						push_back(x);
					return;
				}

			// Matching allocators
			data = arg_other.data;
			idx_end = arg_other.idx_end;
			data_size = arg_other.data_size;
			arg_other.reset();

		}

		dynarray(std::initializer_list<T> arg_il, const allocator_type& arg_allocator = Allocator{}) : 
			allocator(arg_allocator)
		{
			reserve(arg_il.size());
			try
			{
				for (auto& x : arg_il)
					emplace_back(std::move_if_noexcept(x));
			}
			catch (...)
			{
				clear();
				throw;
			}
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Assignment operators
		// -------------------------------------------------------------------------------------------------------------------------

		dynarray& operator=(const dynarray& other)
		{
			if (this != std::addressof(other))
			{
				if constexpr (allocator_traits::propagate_on_container_copy_assignment::value)
					allocator = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.allocator);

				dynarray temp(std::move(*this));
				try
				{
					clear();
					reallocate(other.data_size);
					for (const auto& x : other)
						push_back(x);
				}
				catch (...)
				{
					*this = std::move(temp);
					throw;
				}

			}
			return *this;
		}

		dynarray& operator=(dynarray&& other) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value)
		{
			if (this != std::addressof(other))
			{
				if constexpr (allocator_traits::propagate_on_container_move_assignment::value)
					allocator = std::move(other.allocator);
				else if (!(allocator == other.allocator))
				{
					// Must perform full copy
					clear();
					reallocate(other.data_size);
					for (const auto& x : other)
						push_back(x);
					other.clear();
					return *this;
				}

				// Movable allocator or matching allocators
				clear();
				data = other.data;
				idx_end = other.idx_end;
				data_size = other.data_size;
				other.reset();
			}

			return *this;
		}

		dynarray& operator=(std::initializer_list<T> il)
		{
			dynarray temp(std::move(*this));
			try
			{
				reallocate(il.size());
				for (auto x : il)
					emplace_back(std::move_if_noexcept(x));
			}
			catch (...)
			{
				*this = std::move(temp);
				throw;
			}
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Element & state access
		// -------------------------------------------------------------------------------------------------------------------------

		reference operator[](size_type idx)
		{
			return data[idx];
		}

		const_reference operator[](size_type idx) const
		{
			return data[idx];
		}

		reference at(size_type idx)
		{
			if (idx >= idx_end)
				throw (std::out_of_range("hct::dynarray::at: requested index was out of range"));

			return this->operator[](idx);
		}

		const_reference at(size_type idx) const
		{
			if (idx >= idx_end)
				throw (std::out_of_range("hct::dynarray::at: requested index was out of range"));

			return this->operator[](idx);
		}

		reference front() noexcept
		{
			return *data;
		}

		const_reference front() const noexcept
		{
			return *data;
		}

		reference back() noexcept
		{
			return data[idx_end - 1];
		}

		const_reference back() const noexcept
		{
			return data[idx_end - 1];
		}

		pointer get_data() noexcept
		{
			return data;
		}

		const_pointer get_data() const noexcept
		{
			return data;
		}

		allocator_type get_allocator() const noexcept
		{
			return allocator;
		}

		bool empty() const noexcept
		{
			return idx_end == 0;
		}

		size_type capacity() const noexcept
		{
			return data_size - idx_end;
		}

		size_type size() const noexcept
		{
			return idx_end;
		}

		size_type max_size() const noexcept
		{
			return std::min(static_cast<size_type>((std::numeric_limits<difference_type>::max)()), allocator_traits::max_size(allocator));
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Modifiers
		// -------------------------------------------------------------------------------------------------------------------------

		void assign(size_type n, const T& value)
		{
			dynarray temp(std::move(*this));
			try
			{
				clear();
				resize(n, value);
			}
			catch (...)
			{
				*this = std::move(temp);
				throw;
			}
		}

		template <typename Iter>
		void assign(Iter iBegin, Iter iEnd)
		{
			dynarray temp(std::move(*this));
			try
			{
				clear();
				if constexpr (std::random_access_iterator<Iter>)
					reserve(std::distance(iBegin, iEnd));
				for (auto iter = iBegin; iter < iEnd; ++iter)
					emplace_back(*iter);
			}
			catch (...)
			{
				*this = std::move(temp);
				throw;
			}
		}

		void assign(std::initializer_list<T> il)
		{
			dynarray temp(std::move(*this));
			try
			{
				clear();
				reserve(il.size());
				for (auto& x : il)
					emplace_back(std::move_if_noexcept(x));
			}
			catch (...)
			{
				*this = std::move(temp);
				throw;
			}
		}

		iterator insert(const_iterator pos, const T& value)
		{
			return emplace(pos, value);
		}

		iterator insert(const_iterator pos, T&& value)
		{
			return emplace(pos, std::move(value));
		}

		iterator insert(const_iterator pos, size_type n, const T& value)
		{
			if (pos < begin() || pos >= end())
				throw (std::out_of_range("hct::dynarray::insert: pos iterator out of range"));

			auto offset = pos - begin();
			auto existing_elements = std::distance(const_cast<iterator>(pos), end());

			if (n < 1)
				return begin() + offset;

			if (capacity() < n)
				reallocate();

			move_elements_backwards(begin() + offset, end(), n);

			auto gap = std::addressof(*begin()) + offset;
			for (size_type a = 0; a < n; ++a, ++gap)
			{
				if (a < existing_elements)
					allocator_traits::destroy(allocator, gap);
				allocator_traits::construct(allocator, gap, value);
			}

			return begin() + offset;
		}

		template <typename Iter, std::enable_if_t<std::input_iterator<Iter>, bool> = true>
		iterator insert(const_iterator pos, Iter iBegin, Iter iEnd)
		{
			if (pos < begin() || pos >= end())
				throw (std::out_of_range("hct::dynarray::insert: pos iterator out of range"));
			if (iBegin > iEnd)
				throw (std::invalid_argument("hct::dynarray::insert: invalid iterator range"));

			auto offset = pos - begin();
			auto existing_elements = static_cast<size_type>(std::distance(const_cast<iterator>(pos), end()));
			auto n = static_cast<size_type>(std::distance(iBegin, iEnd));

			if (n < 1)
				return begin() + offset;

			if (capacity() < n)
				reallocate();

			move_elements_backwards(begin() + offset, end(), n);

			auto gap = std::addressof(*begin()) + offset;
			for (size_type a = 0; a < n; ++a, ++gap)
			{
				if (a < existing_elements)
					allocator_traits::destroy(allocator, gap);
				allocator_traits::construct(allocator, gap, *iBegin++);
			}

			return begin() + offset;
		}

		iterator insert(const_iterator pos, std::initializer_list<T> il)
		{
			return insert(pos, il.begin(), il.end());
		}

		template <typename... Args>
		iterator emplace(const_iterator pos, Args&&... args)
		{
			if (pos < begin() || pos >= end())
				throw (std::out_of_range("hct::dynarray::emplace: pos iterator out of range"));

			auto offset = pos - begin();

			if (capacity() == 0)
				reallocate();

			move_elements_backwards(begin() + offset, end(), 1);

			auto gap = std::addressof(*begin()) + offset;
			allocator_traits::destroy(allocator, gap);
			allocator_traits::construct(allocator, gap, std::forward<Args>(args)...);

			return begin() + offset;
		}

		iterator erase(const_iterator pos)
		{
			return erase(pos, pos + 1);
		}

		iterator erase(const_iterator iBegin, const_iterator iEnd)
		{
			auto iB = const_cast<iterator>(iBegin);
			auto iE = const_cast<iterator>(iEnd);

			if (iB < begin() || iE > end() || iB > iE)
				throw (std::out_of_range("hct::dynarray::erase: at least one iterator out of range or invalid range"));

			auto offset = iB - begin();
			auto n = iE - iB;

			if (n == 0)
				return iB;

			std::move(iE, end(), iE - n);
			for (decltype(n) a = 0; a < n; ++a)
				pop_back();

			return begin() + offset;
		}

		void push_back(const T& value)
		{
			emplace_back(value);
		}

		void push_back(T&& value)
		{
			emplace_back(std::move(value));
		}

		template <typename... Args>
		reference emplace_back(Args&&... args)
		{
			if (idx_end == data_size)
				reallocate();

			allocator_traits::construct(allocator, data + idx_end, std::forward<Args>(args)...);
			++idx_end;

			return back();
		}

		void pop_back() noexcept
		{
			--idx_end;
			if constexpr (!std::is_trivially_destructible_v<value_type>)
				allocator_traits::destroy(allocator, data + idx_end);
		}

		void resize(size_type n, const T& value)
		{
			if (n < idx_end)
				for (size_type a = 0, b = idx_end - n; a < b; ++a)
					pop_back();
			else
			{
				reserve(n);
				size_type pushed = 0;
				try
				{
					for (size_type a = 0, b = n - idx_end; a < b; ++a, ++pushed)
						push_back(value);
				}
				catch (...)
				{
					for (size_type a = 0; a < pushed; ++a)
						pop_back();
					throw;
				}
			}
		}

		void resize(size_type n)
		{
			resize(n, value_type{});
		}

		void shrink_to_fit()
		{
			if (capacity() > 0)
				reallocate(idx_end == 0 ? 1 : idx_end);
		}

		void reserve(size_type desired_size)
		{
			if (desired_size > data_size)
				reallocate(desired_size);
		}

		void clear() noexcept
		{
			if (data)
			{
				if constexpr (!std::is_trivially_destructible_v<value_type>)
					for (size_type a = 0; a < idx_end; ++a)
						allocator_traits::destroy(allocator, data + a);
				allocator_traits::deallocate(allocator, data, data_size);
			}
			reset();
		}

		void swap(dynarray& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
		{
			if (this != std::addressof(other))
			{
				using std::swap;

				if constexpr (allocator_traits::propagate_on_container_swap::value)
					swap(allocator, other.allocator);
				else if (!(allocator == other.allocator))
				{
					// Must perform full copy
					dynarray temp(*this);
					*this = other;
					other = temp;
					return;
				}

				swap(data, other.data);
				swap(idx_end, other.idx_end);
				swap(data_size, other.data_size);
			}
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Comparison Operators
		// -------------------------------------------------------------------------------------------------------------------------

		friend bool operator<(const dynarray& lhs, const dynarray& rhs) noexcept
		{
			for (size_type a = 0; a < lhs.idx_end; ++a)
			{
				if (lhs.data[a] < rhs.data[a])
					return true;
				if (rhs.data[a] < lhs.data[a])
					return false;
			}

			return lhs.idx_end < rhs.idx_end;
		}

		friend bool operator>(const dynarray& lhs, const dynarray& rhs) noexcept
		{
			return rhs < lhs;
		}

		friend bool operator<=(const dynarray& lhs, const dynarray& rhs) noexcept
		{
			return !(rhs < lhs);
		}

		friend bool operator>=(const dynarray& lhs, const dynarray& rhs) noexcept
		{
			return !(lhs < rhs);
		}

		friend bool operator==(const dynarray& lhs, const dynarray& rhs) noexcept
		{
			for (size_type a = 0; a < lhs.idx_end; ++a)
				if (lhs.data[a] != rhs.data[a])
					return false;

			return lhs.idx_end == rhs.idx_end;
		}

		friend bool operator!=(const dynarray& lhs, const dynarray& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Iterators
		// -------------------------------------------------------------------------------------------------------------------------

		iterator begin() noexcept
		{
			return data;
		}

		iterator end() noexcept
		{
			return data + idx_end;
		}

		const_iterator begin() const noexcept
		{
			return data;
		}

		const_iterator end() const noexcept
		{
			return data + idx_end;
		}

		const_iterator cbegin() const noexcept
		{
			return begin();
		}

		const_iterator cend() const noexcept
		{
			return end();
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(cbegin());
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Destructor
		// -------------------------------------------------------------------------------------------------------------------------

		~dynarray()
		{
			clear();
		}

	};

	template <typename T, typename Allocator>
	inline void swap(dynarray<T, Allocator>& lhs, dynarray<T, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

}

#endif // _HCT_DECK_H_