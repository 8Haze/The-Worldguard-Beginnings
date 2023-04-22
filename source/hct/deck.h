#ifndef _HCT_DECK_H_
#define _HCT_DECK_H_

#include <memory>
#include <type_traits>
#include <iterator>
#include <stdexcept>

namespace hct
{

	template <typename T, typename Allocator>
	class deck;

	// MD - My Deck
	template <typename MD>
	class deck_const_iterator
	{
	private:

		template <typename T, typename Allocator>
		friend class deck;

	public:

		using self_type = deck_const_iterator;
		using value_type = typename MD::value_type;
		using reference = typename MD::const_reference;
		using pointer = typename MD::const_pointer;
		using difference_type = typename MD::difference_type;
		using iterator_category = std::random_access_iterator_tag;

	protected:

		using data_pointer = typename MD::pointer;

		data_pointer ptr = nullptr;
		data_pointer lim_first = nullptr;
		data_pointer lim_last = nullptr;
		bool switched = true;

		inline void increment() noexcept
		{
			++ptr;
			if (ptr > lim_last)
			{
				ptr = lim_first;
				switched = true;
			}
		}

		inline void decrement() noexcept
		{
			--ptr;
			if (lim_first > ptr)
			{
				ptr = lim_last;
				switched = false;
			}
		}

	public:

		deck_const_iterator() = default;

		deck_const_iterator(data_pointer arg_ptr, data_pointer arg_lim_first, data_pointer arg_lim_last, bool arg_switched) :
			ptr(arg_ptr), 
			lim_first(arg_lim_first), 
			lim_last(arg_lim_last), 
			switched(arg_switched) {}

		deck_const_iterator(const deck_const_iterator&) noexcept = default;

		deck_const_iterator& operator=(const deck_const_iterator&) noexcept = default;

		self_type& operator++() noexcept
		{
			increment();
			return *this;
		}

		self_type operator++(int) noexcept
		{
			self_type temp = *this;
			increment();
			return temp;
		}

		self_type& operator--() noexcept
		{
			decrement();
			return *this;
		}

		self_type operator--(int) noexcept
		{
			self_type temp = *this;
			decrement();
			return temp;
		}

		self_type operator+(difference_type offset) const noexcept
		{
			self_type temp(*this);
			temp.ptr = temp.ptr + offset;
			if (temp.ptr > temp.lim_last)
			{
				temp.ptr -= (temp.lim_last - temp.lim_first + 1);
				temp.switched = true;
			}
			return temp;
		}

		self_type operator-(difference_type offset) const noexcept
		{
			self_type temp(*this);
			temp.ptr = temp.ptr - offset;
			if (temp.ptr < temp.lim_first)
			{
				temp.ptr += (temp.lim_last - temp.lim_first + 1);
				temp.switched = false;
			}
			return temp;
		}

		difference_type operator-(const self_type& it) const noexcept
		{
			if (switched == it.switched)
				return ptr - it.ptr;
			else
				return ptr + (switched ? lim_last - lim_first : lim_first - lim_last) - it.ptr + 1;
		}

		self_type& operator+=(difference_type offset) noexcept
		{
			ptr += offset;
			if (ptr > lim_last)
			{
				ptr -= (lim_last - lim_first + 1);
				switched = true;
			}
			return *this;
		}

		self_type& operator-=(difference_type offset) noexcept
		{
			ptr -= offset;
			if (ptr < lim_first)
			{
				ptr += (lim_last - lim_first + 1);
				switched = false;
			}
			return *this;
		}

		reference operator[](difference_type offset) const noexcept
		{
			if (ptr + offset > lim_last)
				return *(ptr + offset - (lim_last - lim_first + 1));
			else return *(ptr + offset);
		}

		reference operator*() const noexcept
		{
			return *ptr;
		}

		pointer operator->() const noexcept
		{
			return ptr;
		}

		bool operator<(const self_type& it) const noexcept
		{
			if (switched == it.switched)
				return ptr < it.ptr;
			else
				return it.switched;
		}

		bool operator>(const self_type& it) const noexcept
		{
			return it < *this;
		}

		bool operator<=(const self_type& it) const noexcept
		{
			return !(it < *this);
		}

		bool operator>=(const self_type& it) const noexcept
		{
			return !(*this < it);
		}

		bool operator==(const self_type& it) const noexcept
		{
			return switched == it.switched && ptr == it.ptr;
		}

		bool operator!=(const self_type& it) const noexcept
		{
			return !(*this == it);
		}

		~deck_const_iterator() = default;

	};

	template <typename MD>
	deck_const_iterator<MD> operator+(typename deck_const_iterator<MD>::
		difference_type offset, deck_const_iterator<MD> iter) noexcept
	{
		return iter += offset;
	}

	// MD - My Deck
	template <typename MD>
	class deck_iterator : public deck_const_iterator<MD>
	{
	private:

		template <typename T, typename Allocator>
		friend class deck;

	public:

		using self_type = deck_iterator;
		using value_type = typename MD::value_type;
		using reference = typename MD::reference;
		using pointer = typename MD::pointer;
		using difference_type = typename MD::difference_type;
		using iterator_category = std::random_access_iterator_tag;

	private:

		using base = deck_const_iterator<MD>;
		using base::base;

		using base::ptr;
		using base::lim_first;
		using base::lim_last;
		using base::switched;

		using base::increment;
		using base::decrement;

	public:

		deck_iterator() = default;

		deck_iterator(const deck_iterator&) = default;

		deck_iterator& operator=(const deck_iterator&) = default;

		self_type& operator++() noexcept
		{
			increment();
			return *this;
		}

		self_type operator++(int) noexcept
		{
			self_type temp = *this;
			increment();
			return temp;
		}

		self_type& operator--() noexcept
		{
			decrement();
			return *this;
		}

		self_type operator--(int) noexcept
		{
			self_type temp = *this;
			decrement();
			return temp;
		}

		self_type operator+(difference_type offset) const noexcept
		{
			self_type temp(*this);
			temp.ptr = temp.ptr + offset;
			if (temp.ptr > temp.lim_last)
			{
				temp.ptr -= (temp.lim_last - temp.lim_first + 1);
				temp.switched = true;
			}
			return temp;
		}

		self_type operator-(difference_type offset) const noexcept
		{
			self_type temp(*this);
			temp.ptr = temp.ptr - offset;
			if (temp.ptr < temp.lim_first)
			{
				temp.ptr += (temp.lim_last - temp.lim_first + 1);
				temp.switched = false;
			}
			return temp;
		}

		difference_type operator-(const self_type& it) const noexcept
		{
			return base::operator-(it);
		}

		self_type& operator+=(difference_type offset) noexcept
		{
			base::operator+=(offset);
			return *this;
		}

		self_type& operator-=(difference_type offset) noexcept
		{
			base::operator-=(offset);
			return *this;
		}

		reference operator[](difference_type offset) const noexcept
		{
			return const_cast<reference>(base::operator[](offset));
		}

		reference operator*() const noexcept
		{
			return const_cast<reference>(base::operator*());
		}

		pointer operator->() const noexcept
		{
			return const_cast<pointer>(base::operator->());
		}

		~deck_iterator() = default;

	};

	template <typename MD>
	deck_iterator<MD> operator+(typename deck_iterator<MD>::
		difference_type offset, deck_iterator<MD> iter) noexcept
	{
		return iter += offset;
	}

	template <typename T, typename Allocator = std::allocator<T>>
	class deck
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

		using iterator = deck_iterator<deck<T>>;
		using const_iterator = deck_const_iterator<deck<T>>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	private:

		// -------------------------------------------------------------------------------------------------------------------------
		// Member Data
		// -------------------------------------------------------------------------------------------------------------------------

		pointer data = nullptr;
		size_type idx_start = 0;	// points to the first element
		size_type idx_end = 0;		// points to the first-past-last element
		size_type data_size = 0;
		size_type count = 0;
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
			size_type a = 0;
			try
			{
				for (pointer ptr_data = data + idx_start, ptr_data_end = data + data_size; a < count; ++ptr_data, ++ptr_dest, ++a)
				{
					if (ptr_data == ptr_data_end)
						ptr_data = data;

					allocator_traits::construct(allocator, ptr_dest, std::move_if_noexcept(*ptr_data));
				}
			}
			catch (...)
			{
				if constexpr (!std::is_trivially_destructible_v<value_type>)
					for (pointer ptr_dest_destroy = dest; ptr_dest_destroy < ptr_dest; ++ptr_dest_destroy)
						allocator_traits::destroy(allocator, ptr_dest_destroy);
				throw;
			}
			if constexpr (!std::is_trivially_destructible_v<value_type>)
			{
				a = 0;
				for (pointer ptr_data = data + idx_start, ptr_data_end = data + data_size; a < count; ++ptr_data, ++a)
				{
					if (ptr_data == ptr_data_end)
						ptr_data = data;

					allocator_traits::destroy(allocator, ptr_data);
				}
			}
		}

		void reallocate(size_type desired_size = 0)
		{
			size_type new_size = (desired_size == 0 || desired_size < count) ? (data_size == 0 ? 1 : data_size << 1) : desired_size;
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
			idx_start = 0;
			idx_end = count == new_size ? 0 : count;
			data_size = new_size;
		}

		void reset() noexcept
		{
			idx_start = 0;
			idx_end = 0;
			data_size = 0;
			count = 0;
			data = nullptr;
		}

		void move_elements_backwards(iterator iBegin, iterator iEnd, typename std::iterator_traits<iterator>::difference_type offset)
		{
			auto distance = std::distance(iBegin, iEnd);

			count += offset;
			idx_end += offset;
			if (idx_end >= data_size)
				idx_end -= data_size;

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

		iterator make_mutable_iterator(const_iterator it)
		{
			return { it.ptr, it.lim_first, it.lim_last, it.switched };
		}

	public:

		// -------------------------------------------------------------------------------------------------------------------------
		// Constructors
		// -------------------------------------------------------------------------------------------------------------------------

		deck() noexcept(std::is_nothrow_default_constructible_v<Allocator>) :
			allocator(Allocator{}) {}

		explicit deck(const allocator_type& arg_allocator) noexcept :
			allocator(arg_allocator) {}

		explicit deck(size_type arg_n, const value_type& arg_value, const allocator_type& arg_allocator = Allocator{}) :
			allocator(arg_allocator)
		{
			resize(arg_n, arg_value);
		}

		explicit deck(size_type arg_n, const allocator_type& arg_allocator = Allocator{}) :
			allocator(arg_allocator)
		{
			resize(arg_n);
		}

		template <typename Iter, std::enable_if_t<std::input_iterator<Iter>, bool> = true>
		explicit deck(Iter arg_iBegin, Iter arg_iEnd, const allocator_type& arg_allocator = Allocator{}) :
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

		deck(const deck& arg_other)
		{
			allocator = std::allocator_traits<allocator_type>::select_on_container_copy_construction(arg_other.allocator);
			reserve(arg_other.data_size);
			for (const auto& x : arg_other)
				push_back(x);
		}

		deck(const deck& arg_other, const allocator_type& arg_allocator) :
			allocator(arg_allocator)
		{
			reserve(arg_other.data_size);
			for (const auto& x : arg_other)
				push_back(x);
		}

		deck(deck&& arg_other) noexcept : 
			allocator(std::move(arg_other.allocator)),
			data(arg_other.data),
			idx_start(arg_other.idx_start),
			idx_end(arg_other.idx_end),
			data_size(arg_other.data_size),
			count(arg_other.count)
		{
			arg_other.reset();
		}

		deck(deck&& arg_other, const allocator_type& arg_allocator) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value) :
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
			idx_start = arg_other.idx_start;
			idx_end = arg_other.idx_end;
			data_size = arg_other.data_size;
			count = arg_other.count;
			arg_other.reset();
		}

		deck(std::initializer_list<T> arg_il, const allocator_type& arg_allocator = Allocator{}) :
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

		deck& operator=(const deck& other)
		{
			if (this != std::addressof(other))
			{
				if constexpr (allocator_traits::propagate_on_container_copy_assignment::value)
					allocator = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.allocator);

				deck temp(std::move(*this));
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

		deck& operator=(deck&& other) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value)
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
				idx_start = other.idx_start;
				idx_end = other.idx_end;
				data_size = other.data_size;
				count = other.count;
				other.reset();
			}

			return *this;
		}

		deck& operator=(std::initializer_list<T> il)
		{
			deck temp(std::move(*this));
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

		reference operator[](const size_type idx) noexcept
		{
			size_type dest = idx_start + idx;
			if (dest >= data_size)
				dest -= data_size;
			return data[dest];
		}

		const_reference operator[](const size_type idx) const noexcept
		{
			size_type dest = idx_start + idx;
			if (dest >= data_size)
				dest -= data_size;
			return data[dest];
		}

		reference at(const size_type idx)
		{
			if (idx >= count)
				throw (std::out_of_range("hct::deck::at: requested index was out of range"));

			return this->operator[](idx);
		}

		const_reference at(const size_type idx) const
		{
			if (idx >= count)
				throw (std::out_of_range("hct::deck::at: requested index was out of range"));

			return this->operator[](idx);
		}

		reference front() noexcept
		{
			return data[idx_start];
		}

		const_reference front() const noexcept
		{
			return data[idx_start];
		}

		reference back() noexcept
		{
			return data[(idx_end == 0 ? data_size - 1 : idx_end - 1)];
		}

		const_reference back() const noexcept
		{
			return data[(idx_end == 0 ? data_size - 1 : idx_end - 1)];
		}

		allocator_type get_allocator() const noexcept
		{
			return allocator;
		}

		bool empty() const noexcept
		{
			return count == 0;
		}

		size_type capacity() const noexcept
		{
			return data_size - count;
		}

		size_type size() const noexcept
		{
			return count;
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
			deck temp(std::move(*this));
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
			deck temp(std::move(*this));
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
			deck temp(std::move(*this));
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
				throw (std::out_of_range("hct::deck::insert: pos iterator out of range"));

			auto offset = pos - begin();
			auto existing_elements = std::distance(make_mutable_iterator(pos), end());

			if (n < 1)
				return begin() + offset;

			if (capacity() < n)
				reallocate();

			move_elements_backwards(begin() + offset, end(), n);

			auto gap = std::addressof(*begin()) + offset;
			for (size_type a = 0; a < n; ++a, ++gap)
			{
				if (gap >= data + data_size)
					gap -= data_size;

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
				throw (std::out_of_range("hct::deck::insert: pos iterator out of range"));
			if (iBegin > iEnd)
				throw (std::invalid_argument("hct::deck::insert: invalid iterator range"));

			auto offset = pos - begin();
			auto existing_elements = static_cast<size_type>(std::distance(make_mutable_iterator(pos), end()));
			auto n = static_cast<size_type>(std::distance(iBegin, iEnd));

			if (n < 1)
				return begin() + offset;

			if (capacity() < n)
				reallocate();

			move_elements_backwards(begin() + offset, end(), n);

			auto gap = std::addressof(*begin()) + offset;
			for (size_type a = 0; a < n; ++a, ++gap)
			{
				if (gap >= data + data_size)
					gap -= data_size;

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
				throw (std::out_of_range("hct::deck::emplace: pos iterator out of range"));

			auto offset = pos - begin();

			if (capacity() == 0)
				reallocate();

			move_elements_backwards(begin() + offset, end(), 1);

			auto gap = std::addressof(*begin()) + offset;
			if (gap >= data + data_size)
				gap -= data_size;
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
			auto iB = make_mutable_iterator(iBegin);
			auto iE = make_mutable_iterator(iEnd);

			if (iB < begin() || iE > end() || iB > iE)
				throw (std::out_of_range("hct::deck::erase: at least one iterator out of range or invalid range"));

			auto offset = iB - begin();
			auto n = iE - iB;

			if (n == 0)
				return iB;

			if (static_cast<size_type>(offset) < count - static_cast<size_type>(offset) - static_cast<size_type>(n)) // closer to front
			{
				std::move_backward(begin(), iB, iB + n);
				for (decltype(n) a = 0; a < n; ++a)
					pop_front();
			}
			else // closer to back
			{
				std::move(iE, end(), iE - n);
				for (decltype(n) a = 0; a < n; ++a)
					pop_back();
			}

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
			if (capacity() == 0)
				reallocate();

			pointer dest = data + idx_end;

			allocator_traits::construct(allocator, dest, std::forward<Args>(args)...);

			if (++idx_end == data_size)
				idx_end = 0;
			++count;

			return *dest;
		}

		void push_front(const T& value)
		{
			emplace_front(value);
		}

		void push_front(T&& value)
		{
			emplace_front(std::move(value));
		}

		template <typename... Args>
		reference emplace_front(Args&&... args)
		{
			if (capacity() == 0)
				reallocate();

			size_type new_idx_start = (idx_start == 0 ? data_size - 1 : idx_start - 1);

			allocator_traits::construct(allocator, data + new_idx_start, std::forward<Args>(args)...);

			idx_start = new_idx_start;
			++count;

			return front();
		}

		void pop_back() noexcept
		{
			--count;

			if constexpr (!std::is_trivially_destructible_v<value_type>)
				allocator_traits::destroy(allocator, data + (idx_end == 0 ? data_size - 1 : idx_end - 1));

			if (idx_end == 0)
				idx_end = data_size - 1;
			else
				--idx_end;
		}

		void pop_front() noexcept
		{
			--count;

			if constexpr (!std::is_trivially_destructible_v<value_type>)
				allocator_traits::destroy(allocator, data + idx_start);

			if (++idx_start == data_size)
				idx_start = 0;
		}

		void resize(size_type n, const T& value)
		{
			if (n < count)
				for (size_type a = 0, b = count - n; a < b; ++a)
					pop_back();
			else
			{
				reserve(n);
				size_type pushed = 0;
				try
				{
					for (size_type a = 0, b = n - count; a < b; ++a, ++pushed)
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
				reallocate(count == 0 ? 1 : count);
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
					for (size_type a = 0, b = idx_start; a < count; ++a, ++b)
					{
						if (b == data_size)
							b = 0;

						allocator_traits::destroy(allocator, data + b);
					}
				allocator_traits::deallocate(allocator, data, data_size);
			}
			reset();
		}

		void swap(deck& other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
		{
			if (this != std::addressof(other))
			{
				using std::swap;

				if constexpr (allocator_traits::propagate_on_container_swap::value)
					swap(allocator, other.allocator);
				else if (!(allocator == other.allocator))
				{
					// Must perform full copy
					deck temp(*this);
					*this = other;
					other = temp;
					return;
				}

				swap(data, other.data);
				swap(idx_start, other.idx_start);
				swap(idx_end, other.idx_end);
				swap(data_size, other.data_size);
				swap(count, other.count);
			}
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Comparison Operators
		// -------------------------------------------------------------------------------------------------------------------------

		friend bool operator<(const deck& lhs, const deck& rhs) noexcept
		{
			for (size_type a = 0; a < lhs.count; ++a)
			{
				if (lhs.data[a] < rhs.data[a])
					return true;
				if (rhs.data[a] < lhs.data[a])
					return false;
			}

			return lhs.count < rhs.count;
		}

		friend bool operator>(const deck& lhs, const deck& rhs) noexcept
		{
			return rhs < lhs;
		}

		friend bool operator<=(const deck& lhs, const deck& rhs) noexcept
		{
			return !(rhs < lhs);
		}

		friend bool operator>=(const deck& lhs, const deck& rhs) noexcept
		{
			return !(lhs < rhs);
		}

		friend bool operator==(const deck& lhs, const deck& rhs) noexcept
		{
			for (size_type a = 0; a < lhs.count; ++a)
				if (lhs.data[a] != rhs.data[a])
					return false;

			return lhs.count == rhs.count;
		}

		friend bool operator!=(const deck& lhs, const deck& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		// -------------------------------------------------------------------------------------------------------------------------
		// Iterators
		// -------------------------------------------------------------------------------------------------------------------------

		iterator begin() noexcept
		{
			return { data + idx_start, data, data + data_size - 1, idx_start < idx_end || count == 0 };
		}

		iterator end() noexcept
		{
			return { data + idx_end, data, data + data_size - 1, true };
		}

		const_iterator begin() const noexcept
		{
			return { data + idx_start, data, data + data_size - 1, idx_start < idx_end || count == 0 };
		}

		const_iterator end() const noexcept
		{
			return { data + idx_end, data, data + data_size - 1, true };
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

		~deck()
		{
			clear();
		}

	};

	template <typename T, typename Allocator>
	inline void swap(deck<T, Allocator>& lhs, deck<T, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
	{
		lhs.swap(rhs);
	}

}

#endif // _HCT_DECK_H_