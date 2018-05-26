/*
 * main.cpp
 *
 *  Created on: May 26, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#include "curldownloader.hpp"


static const std::string URL = "https://crypto.stanford.edu/~dabo/onlineCrypto/6.1.intro.mp4_download";
static const std::string TEST_URL = "https://crypto.stanford.edu/~dabo/onlineCrypto/6.2.birthday.mp4_download";
static const std::string FILE_NAME = "my-file";

int main()
{
  CurlDownloader downloader(URL, FILE_NAME);

  downloader.configure();
  downloader.download();

  return 0;
}
