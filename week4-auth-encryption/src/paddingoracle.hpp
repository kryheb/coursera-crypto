
#pragma once

#include <vector>
#include <array>
#include <string_view>
#include <string>
#include "utils/log/log.hpp"
#include "utils/httpclient/simplehttpclient.hpp"


using Block = std::array<std::byte, 16>;

class PaddingOracle 
{
  LoggerChannel mLoggerChannel;

  std::vector<Block> mMsgBlocks;
  std::vector<Block> plainText;
  http::SimpleHttpClient mHttpClient;
  std::string mUrl;
public:
  PaddingOracle();
  void setCipherText(std::string_view aCtx);
  void setUrl(const std::string& aUrl) { mUrl = aUrl; }

  void paddingAttack();

  void decryptBlock(size_t aPrevCtxIdx, size_t aCtxIdx);
  void decryptLastBlock(size_t aPrevCtxIdx, size_t aCtxIdx);
  void fillPadding(size_t aIndex, Block& aCtx, Block aPlainTxt);
  std::byte guessValue(size_t aIndex, Block& aPrevCtx, size_t aCtxIdx);
  http::ErrorCode paddingAttackRequest(Block& aPrevCtx, size_t aCtxIdx);


  private:
  void splitCipherText(std::string_view aCtx);
  std::string blockToString(const Block& aBlock) const;
  void printBlock(const Block& aBlock);
  template<typename... Args>
  std::string buildMsg(Args...Msgs);
  std::string buildMsg(Block& aPrevCtx, size_t aCtxIdx);


};