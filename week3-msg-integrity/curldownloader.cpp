/*
 * curldownloader.cpp
 *
 *  Created on: May 26, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */




#include "curldownloader.hpp"

CurlDownloader::CurlDownloader(const std::string& aUrl, const std::string& aFileName) :
  url(aUrl),
  fileName(aFileName),
  curlHandle(nullptr)
{

}


CurlDownloader::~CurlDownloader()
{
  curl_easy_cleanup(curlHandle);
  curl_global_cleanup();
}


void CurlDownloader::configure()
{
  curl_global_init(CURL_GLOBAL_ALL);

  curlHandle = curl_easy_init();

  curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 0L);
  curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, fwrite);
}


void CurlDownloader::download()
{
  FILE *file;

  file = fopen(fileName.c_str(), "wb");

  if (file) {
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, file);
    curl_easy_perform(curlHandle);
    fclose(file);
  }
}
