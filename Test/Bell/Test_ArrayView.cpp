//=====================================================================
// Copyright (c) 2015-2016 Ryooooooga.
// https://github.com/Ryooooooga
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=====================================================================

#include <gtest/gtest.h>
#include "../Bell/ArrayView.hpp"

using namespace std;
using namespace boost;
using namespace Bell;

namespace
{
	static_assert(is_same<ArrayView<int>::value_type			, int							>::value, "");
	static_assert(is_same<ArrayView<int>::pointer				, int*							>::value, "");
	static_assert(is_same<ArrayView<int>::const_pointer			, const int*					>::value, "");
	static_assert(is_same<ArrayView<int>::reference				, int&							>::value, "");
	static_assert(is_same<ArrayView<int>::const_reference		, const int&					>::value, "");
	static_assert(is_same<ArrayView<int>::iterator				, const int*					>::value, "");
	static_assert(is_same<ArrayView<int>::const_iterator		, const int*					>::value, "");
	static_assert(is_same<ArrayView<int>::reverse_iterator		, reverse_iterator<const int*>	>::value, "");
	static_assert(is_same<ArrayView<int>::const_reverse_iterator, reverse_iterator<const int*>	>::value, "");
	static_assert(is_same<ArrayView<int>::size_type				, size_t						>::value, "");
	static_assert(is_same<ArrayView<int>::pos_type				, size_t						>::value, "");
	static_assert(is_same<ArrayView<int>::difference_type		, ptrdiff_t						>::value, "");
}

TEST(Bell_ArrayView, Constructor_ConstExpr)
{
	constexpr int a[] = { 0, 1, 2, 3 };

	constexpr ArrayView<int> x1;
	constexpr ArrayView<int> x2 = a;
	constexpr ArrayView<int> x3 = ArrayView<int>(a, 2);
	constexpr ArrayView<int> x4 = ArrayView<int>(nullptr, 4);
	constexpr ArrayView<int> x5 = x2;

	static_assert(x1.data() == nullptr, "");
	static_assert(x1.size() == 0, "");

	static_assert(x2.data() == a, "");
	static_assert(x2.size() == 4, "");

	static_assert(x3.data() == a, "");
	static_assert(x3.size() == 2, "");

	static_assert(x4.data() == nullptr, "");
	static_assert(x4.size() == 0, "");

	static_assert(x5.data() == x2.data(), "");
	static_assert(x5.size() == x2.size(), "");
}

TEST(Bell_ArrayView, Constructor)
{
	const array<int, 4>	a = {{ 0, 1, 2, 3 }};
	const vector<int>	b = { 0, 1, 2, 3 };
	const string		c = u8"Test";
	const string_ref	d = c;

	const ArrayView<int>  x0 = a;
	const ArrayView<int>  x1 = b;
	const ArrayView<char> x2 = c;
	const ArrayView<char> x3 = d;

	EXPECT_EQ(a.data(), x0.data());
	EXPECT_EQ(a.size(), x0.size());

	EXPECT_EQ(b.data(), x1.data());
	EXPECT_EQ(b.size(), x1.size());

	EXPECT_EQ(c.data(), x2.data());
	EXPECT_EQ(c.size(), x2.size());

	EXPECT_EQ(d.data(), x3.data());
	EXPECT_EQ(d.size(), x3.size());
}

TEST(Bell_ArrayView, Operators)
{
	constexpr int a[] = { 0, 1, 2, 3 };
	constexpr int b[] = { 0, 1, 2 };

	constexpr ArrayView<int> x0 = a;
	constexpr ArrayView<int> x1 = b;
	constexpr ArrayView<int> x2 = ArrayView<int>(a, 3);

	//	==
	EXPECT_TRUE (x0 == x0);
	EXPECT_FALSE(x0 == x1);
	EXPECT_FALSE(x0 == x2);
	EXPECT_TRUE (x1 == x1);
	EXPECT_TRUE (x1 == x2);
	EXPECT_TRUE (x2 == x2);

	//	!=
	EXPECT_FALSE(x0 != x0);
	EXPECT_TRUE (x0 != x1);
	EXPECT_TRUE (x0 != x2);
	EXPECT_FALSE(x1 != x1);
	EXPECT_FALSE(x1 != x2);
	EXPECT_FALSE(x2 != x2);

	//	[]
	static_assert(x0[0] == 0, "");
	static_assert(x0[1] == 1, "");
	static_assert(x0[2] == 2, "");
	static_assert(x0[3] == 3, "");
	//static_assert(x0[4], "");

	static_assert(x1[0] == 0, "");
	static_assert(x1[1] == 1, "");
	static_assert(x1[2] == 2, "");
	//static_assert(x1[3], "");
}

TEST(Bell_ArrayView, Range)
{
	constexpr int a[] = { 0, 1, 2, 3 };
	constexpr ArrayView<int> x0 = a;

	static_assert(x0.begin	() == x0.data()				, "");
	static_assert(x0.end	() == x0.data() + x0.size()	, "");
	static_assert(x0.cbegin	() == x0.data()				, "");
	static_assert(x0.cend	() == x0.data() + x0.size()	, "");

	EXPECT_EQ(reverse_iterator<const int*>(x0.end()), x0.rbegin());
	EXPECT_EQ(reverse_iterator<const int*>(x0.end()), x0.crbegin());
	EXPECT_EQ(reverse_iterator<const int*>(x0.begin()), x0.rend());
	EXPECT_EQ(reverse_iterator<const int*>(x0.begin()), x0.crend());
}

TEST(Bell_ArrayView, Container)
{
	constexpr int a[] = { 0, 1, 2, 3 };

	constexpr ArrayView<int> x0 = a;
	constexpr ArrayView<int> x1;

	static_assert(x0.empty() == false, "");
	static_assert(x1.empty() == true , "");
}

TEST(Bell_ArrayView, Access)
{
	constexpr int a[] = { 0, 1, 2, 3 };

	constexpr ArrayView<int> x0 = a;
	constexpr ArrayView<int> x1;

	static_assert(x0.front() == 0, "");
	//static_assert(x1.front(), "");

	static_assert(x0.back() == 3, "");
	//static_assert(x1.back(), "");

	static_assert(x0.at(0) == 0, "");
	static_assert(x0.at(1) == 1, "");
	static_assert(x0.at(2) == 2, "");
	static_assert(x0.at(3) == 3, "");
	//static_assert(x0.at(4), "");

	EXPECT_THROW(x0.at(4), std::out_of_range);
}

TEST(Bell_ArrayView, Equals)
{
	constexpr int a[] = { 0, 1, 2, 3 };
	constexpr int b[] = { 0, 1, 2, 3 };
	constexpr int c[] = { -2, 0, 1, -2 };

	constexpr ArrayView<int> x0 = a;
	constexpr ArrayView<int> x1 = b;
	constexpr ArrayView<int> x2 = c;

	EXPECT_TRUE (x0.equals(x1));
	EXPECT_FALSE(x0.equals(x2));
	EXPECT_FALSE(x0.equals(x1, [](int a, int b) { return a > b; }));
	EXPECT_TRUE (x0.equals(x2, [](int a, int b) { return a > b; }));
}

TEST(Bell_ArrayView, Conversion)
{
	const int			a[]	= { 0, 1, 2, 3 };
	const vector<int>	b	= { 0, 1, 2, 3 };
	const string		c	= u8"test";

	const ArrayView<int>	x0 = a;
	const ArrayView<char>	x1 = c;

	static_assert(is_same<decltype(x0.toVector()	), vector<int>	>::value, "");
	static_assert(is_same<decltype(x1.toString()	), string		>::value, "");
	static_assert(is_same<decltype(x1.toStringRef()	), string_ref	>::value, "");

	EXPECT_EQ(b, x0.toVector());
	EXPECT_EQ(u8"test", x1.toString());
	EXPECT_EQ(u8"test", x1.toStringRef());
}

TEST(Bell_ArrayView, Clear)
{
	const int a[] = { 0, 1, 2, 3 };
	ArrayView<int> x = a;

	x.clear();

	EXPECT_EQ(nullptr, x.data());
	EXPECT_EQ(0, x.size());
}

TEST(Bell_ArrayView, RemovePrefix)
{
	const int a[] = { 0, 1, 2, 3 };
	ArrayView<int> x = a;

	x.removePrefix(2);

	EXPECT_EQ(a+2, x.data());
	EXPECT_EQ(2, x.size());
}

TEST(Bell_ArrayView, RemoveSuffix)
{
	const int a[] = { 0, 1, 2, 3 };
	ArrayView<int> x = a;

	x.removeSuffix(2);

	EXPECT_EQ(a, x.data());
	EXPECT_EQ(2, x.size());
}

TEST(Bell_ArrayView, PopFront)
{
	const int a[] = { 0, 1, 2, 3 };
	ArrayView<int> x = a;

	x.popFront();

	EXPECT_EQ(a+1, x.data());
	EXPECT_EQ(3, x.size());
}

TEST(Bell_ArrayView, PopBack)
{
	const int a[] = { 0, 1, 2, 3 };
	ArrayView<int> x = a;

	x.popBack();

	EXPECT_EQ(a, x.data());
	EXPECT_EQ(3, x.size());
}

TEST(Bell_ArrayView, Substring)
{
	constexpr int				a[]	= { 0, 1, 2, 3 };
	constexpr ArrayView<int>	x0	= a;
	constexpr ArrayView<int>	x1	= x0.substring(1, 2);
	constexpr ArrayView<int>	x2	= x0.substring(5, 2);
	constexpr ArrayView<int>	x3	= x0.substring(1, 8);

	EXPECT_EQ(a+1, x1.data());
	static_assert(x1.size() == 2, "");

	static_assert(x2.empty(), "");

	EXPECT_EQ(a+1, x3.data());
	static_assert(x3.size() == 3, "");
}

TEST(Bell_ArrayView, Slice)
{
	constexpr int				a[]	= { 0, 1, 2, 3 };
	constexpr ArrayView<int>	x0	= a;
	constexpr ArrayView<int>	x1	= x0.slice(1, 3);
	constexpr ArrayView<int>	x2	= x0.slice(5, 2);
	constexpr ArrayView<int>	x3	= x0.slice(1, 8);

	EXPECT_EQ(a+1, x1.data());
	static_assert(x1.size() == 2, "");

	static_assert(x2.empty(), "");

	EXPECT_EQ(a+1, x3.data());
	static_assert(x3.size() == 3, "");
}

