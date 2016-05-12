//=====================================================================
// Copyright (c) 2015-2016 Ryooooooga.
// https://github.com/Ryooooooga
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=====================================================================

#pragma once

#include <array>
#include <vector>
#include <boost/utility/string_ref.hpp>

namespace Bell {

	template <typename Type>
	class ArrayView
	{
		const Type* ptr_;
		std::size_t size_;

	public:
		using value_type				= Type;
		using pointer					= Type*;
		using const_pointer				= const Type*;
		using reference					= Type&;
		using const_reference			= const Type&;
		using iterator					= const Type*;
		using const_iterator			= iterator;
		using reverse_iterator			= std::reverse_iterator<const_iterator>;
		using const_reverse_iterator	= reverse_iterator;
		using size_type					= std::size_t;
		using pos_type					= std::size_t;
		using difference_type			= std::ptrdiff_t;

		//	ctor
		constexpr ArrayView() noexcept
			: ptr_(nullptr), size_(0) {}

		constexpr ArrayView(const Type* data, size_type length) noexcept
			: ptr_(data), size_(ptr_ ? length : 0) {}

		template <std::size_t N>
		constexpr ArrayView(const Type (&a)[N]) noexcept
			: ptr_(a), size_(N) {}

		template <std::size_t N>
		constexpr ArrayView(const std::array<Type, N>& a) noexcept
			: ptr_(a.data()), size_(N) {}

		template <typename Allocator>
		ArrayView(const std::vector<Type, Allocator>& v) noexcept
			: ptr_(v.data()), size_(v.size()) {}

		template <typename Traits, typename Allocator>
		ArrayView(const std::basic_string<Type, Traits, Allocator>& s) noexcept
			: ptr_(s.data()), size_(s.size()) {}

		template <typename Traits>
		constexpr ArrayView(const boost::basic_string_ref<Type, Traits>& s) noexcept
			: ptr_(s.data()), size_(s.size()) {}

		//	operators
		bool operator ==(const ArrayView& a) const noexcept
		{
			return equals(a);
		}

		bool operator !=(const ArrayView& a) const noexcept
		{
			return !equals(a);
		}

		constexpr const Type& operator [](pos_type pos) const noexcept
		{
			return assert(pos < size_), ptr_[pos];
		}

		//	range
		constexpr const_iterator begin() const noexcept
		{
			return ptr_;
		}

		constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}

		constexpr const_iterator end() const noexcept
		{
			return ptr_ + size_;
		}

		constexpr const_iterator cend() const noexcept
		{
			return end();
		}

		const_reverse_iterator rbegin() const noexcept
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}

		const_reverse_iterator rend() const noexcept
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator crend() const noexcept
		{
			return rend();
		}

		//	container
		constexpr size_type size() const noexcept
		{
			return size_;
		}

		constexpr bool empty() const noexcept
		{
			return size_ == 0;
		}

		constexpr const Type* data() const noexcept
		{
			return ptr_;
		}

		//	element access
		constexpr const Type& front() const noexcept
		{
			return assert(size_ > 0), ptr_[0];
		}

		constexpr const Type& back() const noexcept
		{
			return assert(size_ > 0), ptr_[size_ - 1];
		}

		constexpr const Type& at(pos_type pos) const
		{
			return (void)(pos < size_ ? 0 : throw std::out_of_range("ArrayView::at out_of_range")), ptr_[pos];	//	:anger: コンパイラバグ回避
			//return pos < size_
			//	? ptr_[pos]
			//	: throw std::out_of_range("ArrayView::at out_of_range");
		}

		//	comparison
		bool equals(const ArrayView& a) const noexcept
		{
			return std::equal(begin(), end(), a.begin(), a.end());
		}

		template <typename Pred>
		bool equals(const ArrayView& a, Pred&& pred) const noexcept
		{
			return std::equal(begin(), end(), a.begin(), a.end(), std::forward<Pred>(pred));
		}

		//	conversion
		template <typename Allocator = std::allocator<Type>>
		std::vector<Type, Allocator> toVector() const
		{
			return std::vector<Type, Allocator>(begin(), end());
		}

		template <typename Traits = std::char_traits<Type>, typename Allocator = std::allocator<Type>>
		std::basic_string<Type, Traits, Allocator> toString() const
		{
			return std::basic_string<Type, Traits, Allocator>(ptr_, size_);
		}

		template <typename Traits = std::char_traits<Type>>
		constexpr boost::basic_string_ref<Type, Traits> toStringRef() const noexcept
		{
			return boost::basic_string_ref<Type, Traits>(ptr_, size_);
		}

		ArrayView& clear() noexcept
		{
			ptr_ = nullptr;
			size_ = 0;

			return *this;
		}

		ArrayView& removePrefix(size_type n) noexcept
		{
			n = std::min(n, size_);
			ptr_ += n;
			size_ -= n;

			return *this;
		}
		
		ArrayView& removeSuffix(size_type n) noexcept
		{
			n = std::min(n, size_);
			size_ -= n;

			return *this;
		}

		ArrayView& popFront() noexcept
		{
			return removePrefix(1);
		}

		ArrayView& popBack() noexcept
		{
			return removeSuffix(1);
		}

		constexpr ArrayView substring(pos_type pos, size_type len = std::numeric_limits<size_type>::max()) const noexcept
		{
			return pos >= size_
				? ArrayView {}
				: ArrayView { ptr_ + pos, std::min(size_ - pos, len) };
		}

		constexpr ArrayView slice(pos_type begin, pos_type end) const noexcept
		{
			return end > begin
				? substring(begin, end - begin)
				: ArrayView {};
		}
	};

}	//	namespace Bell
