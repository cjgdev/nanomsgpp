/*
 * Copyright (c) the libnanomsgpp contributors. All rights reserved.
 *
 * This file is a part of libnanomsgpp, distributed under the MIT
 * license. For full terms see the included COPYING file.
 */

#include "CppUTest/TestHarness.h"

TEST_GROUP(message)
{
};

TEST(message, can_construct_and_destruct)
{
	message m;
}

TEST(message, can_copy_construct)
{
	message m;
	message k(m);
}

TEST(message, can_move_construct)
{
	message m;
	message k(std::move(m));
}

TEST(message, can_copy_assign)
{
	message m;
	message k = m;
}

TEST(message, can_move_assign)
{
	message m;
	message k = std::move(m);
}

TEST(message, can_write_using_stream_operator)
{
	message m;
	int i = 10;
	m << i;
}

TEST(message, can_write_using_cascading_stream_operator)
{
	message m;
	int i = 10, j = 20;
	m << i << j;
}

TEST(message, can_read_using_stream_operator)
{
	message m;
	int i;
	m >> i;
}

TEST(message, can_read_using_cascading_stream_operator)
{
	message m;
	int i, j;
	m >> i >> j;
}

TEST(message, can_write_and_read_using_stream_operator)
{
	message m;
	m << 10;
	int i;
	m >> i;
}