/*
 * msgintegrity.hpp
 *
 *  Created on: May 26, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#pragma once

#include <string>
#include "utils/log/log.hpp"
#include <fstream>
#include <array>
#include <crypto++/cryptlib.h>
#include <crypto++/sha.h>


using Bytes = std::vector<std::byte>;
constexpr size_t KBYTE = 1024;

class MsgIntegrity {

  LoggerChannel mLoggerChannel;

  std::ifstream mFileHandle;
  std::uintmax_t mFileSize{0};

  std::array<std::byte, CryptoPP::SHA256::DIGESTSIZE> mHash;

  public:
  MsgIntegrity(const std::string& aFilename);
  void processFile();
  std::string getHash();

  private:
  void readLastBlock();
  void readPrevBlock();
  void readBlock(uint aSize=1024);
  void hashBlock(Bytes& aBlock);
  template<typename T>
  void printBlock(const T& aBlock);
  template<typename T>
  static std::string blockToString(const T& aBlock);



};