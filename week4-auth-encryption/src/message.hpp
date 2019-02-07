
#pragma once

#include <vector>
#include <array>
#include <string_view>
#include "utils/log/log.hpp"


using Block = std::array<std::byte, 16>;

class Message 
{
  LoggerChannel mLoggerChannel;

  Block mVi;
  std::vector<Block> mMsgBlocks;
public:
  Message(std::string_view aMsgTxt);
  void splitInputMsg(std::string_view aMsgText);
  void printBlock(const Block& aBlock);
};