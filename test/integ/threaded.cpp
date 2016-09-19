/*
     This file is part of libhttpserver
     Copyright (C) 2011, 2012, 2013, 2014, 2015 Sebastiano Merlino

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
     USA
*/

#include "littletest.hpp"
#include <curl/curl.h>
#include <string>
#include <map>
#include "httpserver.hpp"

using namespace httpserver;
using namespace std;

class ok_resource : public http_resource
{
    public:
        const http_response render_GET(const http_request& req)
        {
            return http_response(http_response_builder("OK", 200, "text/plain").string_response());
        }
};

LT_BEGIN_SUITE(threaded_suite)

    webserver* ws;

    void set_up()
    {
        ws = new webserver(create_webserver(8080).start_method(http::http_utils::INTERNAL_SELECT).max_threads(5));
        ws->start(false);
    }

    void tear_down()
    {
        ws->stop();
        delete ws;
    }
LT_END_SUITE(threaded_suite)

LT_BEGIN_AUTO_TEST(threaded_suite, base)
    ok_resource* resource = new ok_resource();
    ws->register_resource("base", resource);
    curl_global_init(CURL_GLOBAL_ALL);
    std::string s;
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:8080/base");
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    res = curl_easy_perform(curl);
    LT_ASSERT_EQ(res, 0);
    curl_easy_cleanup(curl);
LT_END_AUTO_TEST(base)

LT_BEGIN_AUTO_TEST_ENV()
    AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
