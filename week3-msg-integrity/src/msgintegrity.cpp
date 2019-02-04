/*
 * msgintegrity.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#include "msgintegrity.hpp"
#include <fstream>
#include <cstddef>
#include <sstream>
#include <filesystem>
#include <iomanip>


#define LOG(s) BOOST_LOG_SEV(mLoggerChannel, s)
using namespace boost::log::trivial;



MsgIntegrity::MsgIntegrity(const std::string& aFilename) :
  mLoggerChannel(Logger::getInstance().addChannel("MSG_INTEGRITY")),
  mFileHandle(aFilename, std::ios::binary)
{
  if (mFileHandle.is_open()) {
    LOG(severity_level::info) << "Opened file " << aFilename; 
    mFileSize = std::filesystem::file_size(aFilename);
    LOG(severity_level::info) << "File size: " << mFileSize; 
  } else {
    LOG(severity_level::info) << "Cannot open file " << aFilename; 
  }
}


std::string MsgIntegrity::getHash()
{
  return blockToString(mHash);  
}


void MsgIntegrity::processFile()
{
  readLastBlock();
  readPrevBlock();
}


void MsgIntegrity::readLastBlock()
{
  auto lastBlockSize = (mFileSize % KBYTE);
  auto lastBlockPos = mFileSize - lastBlockSize;
  LOG(severity_level::debug) << "Last block size " << lastBlockSize
                              << " at position " << lastBlockPos;
  
  mFileHandle.seekg(lastBlockPos);
  readBlock(lastBlockSize);
  mFileHandle.seekg(lastBlockPos);
}


void MsgIntegrity::readPrevBlock()
{
  std::uintmax_t currPos = mFileHandle.tellg();
  std::uintmax_t newPos = currPos - KBYTE;
  if (newPos >= 0) {
    mFileHandle.seekg(newPos);
  } else {
    LOG(severity_level::error) << "Invalid end position " << newPos;
    return;
  }
  readBlock();
  mFileHandle.seekg(newPos);
  if (newPos != 0) {
    readPrevBlock();
  } else {
    LOG(severity_level::info) << "END: 0 position reached";
  }
}


void MsgIntegrity::readBlock(uint aSize)
{
  if (Bytes block {aSize};
      mFileHandle.read(reinterpret_cast<char*>(block.data()), aSize)) {
    if (aSize == KBYTE) {
      block.insert(block.end(), mHash.begin(), mHash.end());
    }
    hashBlock(block);
  } else {
    LOG(severity_level::error) << "Cannot read a block";
  }
}


void MsgIntegrity::hashBlock(Bytes& aBlock)
{
  CryptoPP::SHA256().CalculateDigest(
    reinterpret_cast<unsigned char*>(mHash.data()),
    reinterpret_cast<const unsigned char*>(aBlock.data()),
    aBlock.size());
  printBlock(mHash);
}


template<typename T>
void MsgIntegrity::printBlock(const T& aBlock)
{
  LOG(severity_level::debug) << blockToString(aBlock);
}


template<typename T>
std::string MsgIntegrity::blockToString(const T& aBlock)
{
  std::stringstream ss;
  for (auto& b : aBlock) {
    ss << std::right << std::setfill('0') << std::setw(2)
        << std::hex << std::to_integer<uint>(b);
  }
  return ss.str();
}


