/*
    __ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++ (test suite)
|  |  |__   |  |  | | | |  version 2.0.2
|_____|_____|_____|_|___|  https://github.com/nlohmann/json

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
Copyright (c) 2013-2016 Niels Lohmann <http://nlohmann.me>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "catch.hpp"

#define private public
#include "json.hpp"
using nlohmann::json;

// special test case to check if memory is leaked if constructor throws

template<class T>
struct bad_allocator : std::allocator<T>
{
    template<class... Args>
    void construct(T*, Args&& ...)
    {
        throw std::bad_alloc();
    }
};

TEST_CASE("bad_alloc")
{
    SECTION("bad_alloc")
    {
        // create JSON type using the throwing allocator
        using bad_json = nlohmann::basic_json<std::map,
              std::vector,
              std::string,
              bool,
              std::int64_t,
              std::uint64_t,
              double,
              bad_allocator>;

        // creating an object should throw
        CHECK_THROWS_AS(bad_json j(bad_json::value_t::object), std::bad_alloc);
    }
}

bool next_allocation_fails = false;

template<class T>
struct my_allocator : std::allocator<T>
{
    template<class... Args>
    void construct(T* p, Args&& ... args)
    {
        if (next_allocation_fails)
        {
            throw std::bad_alloc();
        }
        else
        {
            ::new(reinterpret_cast<void*>(p)) T(std::forward<Args>(args)...);
        }
    }
};

TEST_CASE("controlled bad_alloc")
{
    // create JSON type using the throwing allocator
    using my_json = nlohmann::basic_json<std::map,
          std::vector,
          std::string,
          bool,
          std::int64_t,
          std::uint64_t,
          double,
          my_allocator>;

    SECTION("class json_value")
    {
        SECTION("json_value(value_t)")
        {
            SECTION("object")
            {
                next_allocation_fails = false;
                auto t = my_json::value_t::object;
                CHECK_NOTHROW(my_json::json_value j(t));
                next_allocation_fails = true;
                CHECK_THROWS_AS(my_json::json_value j(t), std::bad_alloc);
                next_allocation_fails = false;
            }
            SECTION("array")
            {
                next_allocation_fails = false;
                auto t = my_json::value_t::array;
                CHECK_NOTHROW(my_json::json_value j(t));
                next_allocation_fails = true;
                CHECK_THROWS_AS(my_json::json_value j(t), std::bad_alloc);
                next_allocation_fails = false;
            }
            SECTION("string")
            {
                next_allocation_fails = false;
                auto t = my_json::value_t::string;
                CHECK_NOTHROW(my_json::json_value j(t));
                next_allocation_fails = true;
                CHECK_THROWS_AS(my_json::json_value j(t), std::bad_alloc);
                next_allocation_fails = false;
            }
        }

        SECTION("json_value(const string_t&)")
        {
            next_allocation_fails = false;
            my_json::string_t v("foo");
            CHECK_NOTHROW(my_json::json_value j(v));
            next_allocation_fails = true;
            CHECK_THROWS_AS(my_json::json_value j(v), std::bad_alloc);
            next_allocation_fails = false;
        }

        /*
                SECTION("json_value(const object_t&)")
                {
                    next_allocation_fails = false;
                    my_json::object_t v {{"foo", "bar"}};
                    CHECK_NOTHROW(my_json::json_value j(v));
                    next_allocation_fails = true;
                    CHECK_THROWS_AS(my_json::json_value j(v), std::bad_alloc);
                    next_allocation_fails = false;
                }
        */
        /*
                SECTION("json_value(const array_t&)")
                {
                    next_allocation_fails = false;
                    my_json::array_t v = {"foo", "bar", "baz"};
                    CHECK_NOTHROW(my_json::json_value j(v));
                    next_allocation_fails = true;
                    CHECK_THROWS_AS(my_json::json_value j(v), std::bad_alloc);
                    next_allocation_fails = false;
                }
        */
    }

    SECTION("class basic_json")
    {
        SECTION("basic_json(const CompatibleObjectType&)")
        {
            next_allocation_fails = false;
            std::map<std::string, std::string> v {{"foo", "bar"}};
            CHECK_NOTHROW(my_json j(v));
            next_allocation_fails = true;
            CHECK_THROWS_AS(my_json j(v), std::bad_alloc);
            next_allocation_fails = false;
        }

        SECTION("basic_json(const CompatibleArrayType&)")
        {
            next_allocation_fails = false;
            std::vector<std::string> v {"foo", "bar", "baz"};
            CHECK_NOTHROW(my_json j(v));
            next_allocation_fails = true;
            CHECK_THROWS_AS(my_json j(v), std::bad_alloc);
            next_allocation_fails = false;
        }

        SECTION("basic_json(const typename string_t::value_type*)")
        {
            next_allocation_fails = false;
            CHECK_NOTHROW(my_json v("foo"));
            next_allocation_fails = true;
            CHECK_THROWS_AS(my_json v("foo"), std::bad_alloc);
            next_allocation_fails = false;
        }

        SECTION("basic_json(const typename string_t::value_type*)")
        {
            next_allocation_fails = false;
            std::string s("foo");
            CHECK_NOTHROW(my_json v(s));
            next_allocation_fails = true;
            CHECK_THROWS_AS(my_json v(s), std::bad_alloc);
            next_allocation_fails = false;
        }
    }
}
