/*
    __ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++ (test suite)
|  |  |__   |  |  | | | |  version 3.7.3
|_____|_____|_____|_|___|  https://github.com/nlohmann/json

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2013-2019 Niels Lohmann <http://nlohmann.me>.

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

#include "doctest_compatibility.h"

#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

using nlohmann::json;

namespace
{
bool wchar_is_bit8();
bool wchar_is_bit8()
{
    return (sizeof(wchar_t) == 1);
}

bool wchar_is_bit16();
bool wchar_is_bit16()
{
    return (sizeof(wchar_t) == 2);
}

bool wchar_is_bit32();
bool wchar_is_bit32()
{
    return (sizeof(wchar_t) == 4);
}

bool wstring_is_utf16();
bool wstring_is_utf16()
{
    return (std::wstring(L"💩") == std::wstring(L"\U0001F4A9"));
}
}

TEST_CASE("wide stream")
{
    SECTION ("wstring is utf16")
    {
        if (wstring_is_utf16())
        {
            SECTION("wchar is bit-8")
            {
              if (wchar_is_bit8())
              {
                wchar_t ch[] = { L"你好，世界" };

                std::wstring wstr;
                std::wstringstream wss;
                wss.str(ch);
                wss >> wstr;
                json j = json::parse(wstr);
                CHECK(j.dump() == "你好，世界");
              }
            }

            SECTION("wchar is bit-16")
            {
              if (wchar_is_bit16())
              {
                wchar_t ch[] = { L"你好，世界" };

                std::wstring wstr;
                std::wstringstream wss;
                wss.str(ch);
                wss >> wstr;
                json j = json::parse(wstr);
                CHECK(j.dump() == "你好，世界");
              }
            }

            SECTION("wchar is bit-32")
            {
              if (wchar_is_bit32())
              {
                wchar_t ch[] = { L"你好，世界" };

                std::wstring wstr;
                std::wstringstream wss;
                wss.str(ch);
                wss >> wstr;
                json j = json::parse(wstr);
                CHECK(j.dump() == "你好，世界");
              }
            }

            SECTION("operator >>")
            {
              if (wchar_is_bit16())
              {
                std::wstringstream wss;
                wss  << L"你好，世界";
                json j;
                wss >> j;
                CHECK(j == json({"你好，世界"}));
              }
            }

        }
    }
}
