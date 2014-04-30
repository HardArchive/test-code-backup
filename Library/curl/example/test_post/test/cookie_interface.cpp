/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2012, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* This example shows usage of simple cookie interface. */
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

//#include <curl/curl.h>
#include "curl.h"

#pragma comment(lib, "libcurl.lib")

#define POSTFIELDS "vb_login_username=irc&cookieuser=1&vb_login_password=&s=&securitytoken=guest&do=login&vb_login_md5password=cec9b1c9fe98858a7fd9830fee256e3e&vb_login_md5password_utf=cec9b1c9fe98858a7fd9830fee256e3e"

static void
print_cookies(CURL *curl)
{
    CURLcode res;
    struct curl_slist *cookies;
    struct curl_slist *nc;
    int i;

    printf("Cookies, curl knows:\n");
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
        exit(1);
    }
    nc = cookies, i = 1;
    while (nc)
    {
        printf("[%d]: %s\n", i, nc->data);
        nc = nc->next;
        i++;
    }
    if (i == 1)
    {
        printf("(none)\n");
    }
    curl_slist_free_all(cookies);
}

int main(void)
{
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl)
    {
        char nline[256];

        curl_easy_setopt(curl, CURLOPT_URL, "http://bbs.pediy.com/login.php?do=login");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "abce.cookie"); /* just to start the cookie engine *///会使curl下一次发请求时从指定的文件中读取cookie。
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTFIELDS);                // CURLOPT_POSTFIELDS: POST参数
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        print_cookies(curl);

        printf("Erasing curl's knowledge of cookies!\n");
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, "ALL");  
		//CURLOPT_COOKIELIST 会把指定的cookie字符串列表加入easy handle维护的cookie列表中。
		// 每个cookie字符串要么符合HTTP response header的"Set-Cookie: NAME=VALUE;..."格式，要么符合Netscape cookie格式。

        print_cookies(curl);

        printf("-----------------------------------------------\n"
               "Setting a cookie \"PREF\" via cookie interface:\n");
#ifdef WIN32
#define snprintf sprintf_s
#endif
        /* Netscape format cookie */
        snprintf(nline, sizeof(nline), "%s\t%s\t%s\t%s\t%lu\t%s\t%s",
                 ".google.com", "TRUE", "/", "FALSE", (unsigned long)time(NULL) + 31337UL, "PREF", "hello google, i like you very much!");
        res = curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Curl curl_easy_setopt failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        /* HTTP-header style cookie */
        snprintf(nline, sizeof(nline),
                 "Set-Cookie: OLD_PREF=3d141414bf4209321; "
                 "expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.google.com");
        res = curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Curl curl_easy_setopt failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        print_cookies(curl);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }

    curl_global_cleanup();
    return 0;
}
