/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

//DISTANCE

TEST(GuesserTest, correct_guess)
{
	Guesser g("hello");
	ASSERT_EQ(0, g.distance("hello"));
}

TEST(GuesserTest, one_off)
{
	Guesser g("hello");
	ASSERT_EQ(1, g.distance("hallo"));
}

TEST(GuesserTest, all_diff)
{
	Guesser g("abcde");
	ASSERT_EQ(5, g.distance("vwxyz"));
}

TEST(GuesserTest, too_long)
{
	Guesser g("abc");
	ASSERT_EQ(3, g.distance("abcdef")); // capped at secret length
}

TEST(GuesserTest, right_but_missing)
{
	Guesser g("abcdef");
	ASSERT_EQ(3, g.distance("abc"));
}

TEST(GuesserTest, both_empty)
{
	Guesser g("");
	ASSERT_EQ(0, g.distance(""));
}

TEST(GuesserTest, empty_guess_3)
{
	Guesser g("abc");
	ASSERT_EQ(3, g.distance(""));
}

TEST(GuesserTest, diff_len_and_chars)
{
	Guesser g("abcd");
	ASSERT_EQ(3, g.distance("abXYZ"));
}

TEST(GuesserTest, all_wrong_too_long)
{
	Guesser g("abc");
	ASSERT_EQ(3, g.distance("^&*(^())")); // should cap at 3
}

TEST(GuesserTest, thirty_two_test)
{
	Guesser g("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"); //52 long
	ASSERT_EQ(0, g.distance("abcdefghijklmnopqrstuvwxyzabcdef")); // should cap at 32
}

//MATCH

TEST(GuesserTest, good_match)
{
	Guesser g("secret");
	ASSERT_TRUE(g.match("secret"));
}

TEST(GuesserTest, bad_match)
{
	Guesser g("secret");
	ASSERT_FALSE(g.match("secrex"));
}

TEST(GuesserTest, locking_abc)
{
	Guesser g("abc");
	ASSERT_FALSE(g.match("abd")); // distance = 1
	ASSERT_FALSE(g.match("abe")); // distance = 1
	ASSERT_FALSE(g.match("abf")); // LOCK
	ASSERT_FALSE(g.match("abc")); // locked, false
}

//LOCKING

TEST(GuesserTest, too_wrong_lock)
{
	Guesser g("abc");
	ASSERT_FALSE(g.match("xyz")); // distance > 2, lock
	ASSERT_FALSE(g.match("abc")); // still locked
}

//RESET

TEST(GuesserTest, no_lock_right)
{
	Guesser g("abc");

	ASSERT_FALSE(g.match("abd"));
	ASSERT_TRUE(g.match("abc")); // correct guess
	ASSERT_FALSE(g.match("abd")); // should still allow attempts
}

//EDGE CASES

TEST(GuesserTest, empty_match)
{
	Guesser g("");
	ASSERT_TRUE(g.match(""));
}

TEST(GuesserTest, match_special_chars)
{
	Guesser g("!@#");
	ASSERT_FALSE(g.match("!@$"));
	ASSERT_TRUE(g.match("!@#"));
}