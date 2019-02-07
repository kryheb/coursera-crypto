

#include "message.hpp"
#include <charconv>
#include <sstream>
#include <iomanip>

#define LOG(s) BOOST_LOG_SEV(mLoggerChannel, s)
using namespace boost::log::trivial;


Message::Message(std::string_view aMsgText) :
  mLoggerChannel(Logger::getInstance().addChannel("MESSAGE"))
{
  splitInputMsg(aMsgText);
}


namespace {
  constexpr uint CHARS_IN_BLOCK = 32;
  constexpr uint VI_BLOCK_INDEX = 0;
}


void Message::splitInputMsg(std::string_view aMsgText)
{
  for (auto i=0; i<aMsgText.length(); i+=2) {
    int byte;
    if (auto [p, ec] = std::from_chars(aMsgText.data()+i, aMsgText.data()+i+2, byte, 16);
        ec == std::errc()) {
          uint blkIndex = i/CHARS_IN_BLOCK;
          uint pos = i%CHARS_IN_BLOCK/2; 
          if (blkIndex == VI_BLOCK_INDEX) {
            mVi.at(pos) = static_cast<std::byte>(byte);
          } else {
            if (mMsgBlocks.size() < blkIndex) {
              mMsgBlocks.emplace_back(Block{});
            }
            mMsgBlocks.at(blkIndex-1).at(pos) = static_cast<std::byte>(byte);
          }
    }
  }

  LOG(severity_level::debug) << "VI:";
  printBlock(mVi);
  for (auto& b : mMsgBlocks) {
    LOG(severity_level::debug) << "msg block:";
    printBlock(b);
  }

}


void Message::printBlock(const Block& aBlock)
{
  std::stringstream ss;
  for (auto& b : aBlock) {
    ss << std::right << std::setfill('0') << std::setw(2)
        << std::hex << std::to_integer<uint>(b) << " ";
  }
  LOG(severity_level::debug) << ss.str();
}