/*
 * main.cpp
 *
 *  Created on: May 26, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#include "utils/log/log.hpp"
#include "curldownloader.hpp"
#include "msgintegrity.hpp"

#define LOG(s) BOOST_LOG_SEV(mainChannel, s)
using namespace boost::log::trivial;


static const std::string URL = "https://crypto.stanford.edu/~dabo/onlineCrypto/6.1.intro.mp4_download";
static const std::string FILE_NAME = "my-file";
static const std::string TEST_URL = "https://crypto.stanford.edu/~dabo/onlineCrypto/6.2.birthday.mp4_download";
static const std::string TEST_FILE_NAME = "my-test-file";
static const std::string EXPECTED_TEST_HASH = "03c08f4ee0b576fe319338139c045c89c3e8e9409633bea29442e21425006ea8";

int main()
{
  Logger::getInstance().init();
  auto mainChannel = Logger::getInstance().addChannel("MAIN");

  {
    LOG(severity_level::info) << "Verifying msg integrity checker with test file";
    LOG(severity_level::info) << "Downloading...";
    CurlDownloader downloader(TEST_URL, TEST_FILE_NAME);

    downloader.configure();
    downloader.download();

    MsgIntegrity miTest(TEST_FILE_NAME);
    miTest.processFile();
    auto testHash = miTest.getHash();
    LOG(severity_level::info) << "Calcutaled test hash:";
    LOG(severity_level::info) << testHash;
    LOG(severity_level::info) << "Expected test hash:";
    LOG(severity_level::info) << EXPECTED_TEST_HASH;

    if (testHash != EXPECTED_TEST_HASH) {
      LOG(severity_level::info) << "Invalid test hash, exiting...";
      return 1;
    }
  }
  {
    LOG(severity_level::info) << "Verifying file";
    LOG(severity_level::info) << "Downloading...";
    CurlDownloader downloader(URL, FILE_NAME);

    downloader.configure();
    downloader.download();
    LOG(severity_level::info) << "Calculating...";
    MsgIntegrity miTest(FILE_NAME);
    miTest.processFile();
    LOG(severity_level::info) << "Result:";
    LOG(severity_level::info) << miTest.getHash();
  }


  LOG(severity_level::info) << "Exiting...";

  return 0;
}
