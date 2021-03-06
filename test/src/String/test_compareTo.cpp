/*
 * Copyright (c) 2020 Arduino.  All rights reserved.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <catch.hpp>

#include <WString.h>
#include <Common.h>

// version of strcmp that returns only -1, 0, or 1
int clamped_strcmp(const char* left, const char* right) {
  return arduino::min(1, arduino::max(-1,strcmp(left, right)));
}

/**************************************************************************************
 * TEST CODE
 **************************************************************************************/

TEST_CASE ("Testing String::compareTo(const String &)", "[String-compareTo-01]")
{
  WHEN ("Strings are equal")
  {
    arduino::String str1("Hello"), str2("Hello");
    REQUIRE(str1.compareTo(str2) == 0);
  }

  WHEN ("str2 is empty")
  {
    arduino::String str1("Hello"), str2;
    REQUIRE(str1.compareTo(str2) == clamped_strcmp(str1.c_str(), str2.c_str()));
  }

  WHEN ("str1 is empty")
  {
    arduino::String str1, str2("Hello");
    REQUIRE(str1.compareTo(str2) == clamped_strcmp(str1.c_str(), str2.c_str()));
  }
}

TEST_CASE ("Testing String::compareTo(const char *)", "[String-compareTo-02]")
{
  WHEN ("Strings are equal")
  {
    arduino::String str("Hello");
    REQUIRE(str.compareTo("Hello") == 0);
  }

  WHEN ("Passed string is empty")
  {
    arduino::String str1("Hello"), str2("");
    REQUIRE(str1.compareTo("") == clamped_strcmp(str1.c_str(), str2.c_str()));
  }

  WHEN ("Passed string is compared with empty string")
  {
    arduino::String str1, str2("Hello");
    REQUIRE(str1.compareTo("Hello") == clamped_strcmp(str1.c_str(), str2.c_str()));
  }
}

TEST_CASE ("Testing String::compareTo(const char *) with empty buffer", "[String-compareTo-03]")
{
  WHEN ("First string has a valid buffer")
  {
    char *buffer = NULL;

    arduino::String str1("Hello");
    REQUIRE(str1.compareTo(buffer) != 0);
  }

  WHEN ("First string does NOT have a valid buffer")
  {
    char *buffer1 = NULL;
    char *buffer2 = NULL;

    arduino::String str1(buffer1);
    REQUIRE(str1.compareTo(buffer2) == 0);
  }
}


TEST_CASE ("Testing String::compareTo(const String &) with empty buffer", "[String-compareTo-04]")
{
  WHEN ("First string has a valid buffer")
  {
    char *buffer = NULL;

    arduino::String str1("Hello");
    arduino::String str2(buffer);
    REQUIRE(str1.compareTo(str2) != 0);
  }

  WHEN ("First string does NOT have a valid buffer")
  {
    char *buffer1 = NULL;
    char *buffer2 = NULL;

    arduino::String str1(buffer1);
    arduino::String str2(buffer2);
    REQUIRE(str1.compareTo(str2) == 0);
  }
}

TEST_CASE("Testing String::compareTo(const std::string &)", "[String-compareTo-05]")
{
    WHEN("Strings are equal")
    {
        arduino::String str1("Hello");
        std::string str2("Hello");
        REQUIRE(str1.compareTo(str2) == 0);
    }

    WHEN("str2 is empty")
    {
        arduino::String str1("Hello");
        std::string str2;
        REQUIRE(str1.compareTo(str2) == clamped_strcmp(str1.c_str(), str2.c_str()));
    }

    WHEN("str1 is empty")
    {
        arduino::String str1;
        std::string str2("Hello");
        REQUIRE(str1.compareTo(str2) == clamped_strcmp(str1.c_str(), str2.c_str()));
    }
}

