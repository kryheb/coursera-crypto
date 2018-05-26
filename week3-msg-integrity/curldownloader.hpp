/*
 * curldownloader.hpp
 *
 *  Created on: May 26, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#ifndef DEBUG_CURLDOWNLOADER_HPP_
#define DEBUG_CURLDOWNLOADER_HPP_

#include <curl/curl.h>
#include <string>


class CurlDownloader
{
    std::string url;
    std::string fileName;
    CURL *curlHandle;

  public:
    CurlDownloader(const std::string& aUrl, const std::string& aFileName);
    ~CurlDownloader();

    void download();
    void configure();
};



#endif /* DEBUG_CURLDOWNLOADER_HPP_ */
