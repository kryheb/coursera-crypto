/*
 * block_cipher.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#ifndef BLOCK_CIPHER_CPP_
#define BLOCK_CIPHER_CPP_

#include <iostream>
#include "block_cipher.hpp"



/* =================== BlockCipher ==================== */


bytes BlockCipher::stringToBytes(const std::string& aString)
{
  bytes retBytes;

  for (auto i=0u; i<aString.size(); i=i+BYTE_STRING_SIZE) {
    auto byteString = aString.substr(i, BYTE_STRING_SIZE);
    try {
      auto byteHex = std::strtoul(byteString.c_str(), NULL, 16);
      retBytes.push_back(byteHex & 0xFF);
    } catch (std::exception& e) {
      std::cout<< __FUNCTION__ << " exception: " << e.what() << '\n';
    }
  }

  return retBytes;
}


bytes BlockCipher::getBlock(const bytes& aCTBytes, const uint aBlockIndex)
{
  bytes retBlock;
  auto begin = aCTBytes.begin() + aBlockIndex * BLOCK_SIZE;

  if (begin < aCTBytes.end() ) {
    auto end = begin + BLOCK_SIZE;
    if (end > aCTBytes.end()) {
      end = aCTBytes.end();
    }
    std::copy(begin, end, std::back_inserter(retBlock));
  }

  return retBlock;
}


void BlockCipher::clearData()
{
  key.clear();
  cipherTextMsg.clear();
  iv.clear();
  plainText.clear();
}


void BlockCipher::setKey(const std::string& aKey)
{
  key = stringToBytes(aKey);
}


void BlockCipher::setCipherTextAndIV(const std::string& aCT)
{
  auto cipherText = stringToBytes(aCT);
  iv = getBlock(cipherText, 0);
  std::copy(cipherText.begin() + BLOCK_SIZE, cipherText.end(), std::back_inserter(cipherTextMsg));
}


/* =================== CTR ==================== */


void CTR::incrementCounter()
{
   /*
    * iv contains 8-byte nonce and 8-byte counter
    * increment last counter byte and update the rest if overflowed
    */

  for (size_t i=COUNTER_LS_BYTE; i>=COUNTER_MS_BYTE; i--) {
    if (i>iv.size()) break;
    iv.at(i)++;
    if (iv.at(i) != 0) {
      break;
    }
    if (iv.at(i)==0 && i==COUNTER_MS_BYTE) {
      for (size_t j=COUNTER_LS_BYTE; j>COUNTER_MS_BYTE; j--) {
        iv.at(j)=0;
      }
      break;
    }
  }
}


void CTR::decrypt(const std::string& aAESKey, const std::string& aCT)
{
  clearData();
  setKey(aAESKey);
  setCipherTextAndIV(aCT);

  for (auto i=0u; i<=(cipherTextMsg.size()/BLOCK_SIZE); i++) {
    bytes block = getBlock(cipherTextMsg, i);
    bytes decryptedBlock(BLOCK_SIZE);

    CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption e;
    e.SetKey(key.data(), BLOCK_SIZE);
    e.ProcessData(decryptedBlock.data(), iv.data(), BLOCK_SIZE);

    for (auto j=0u; j<BLOCK_SIZE; j++) {
      if (j == block.size()) break;
      plainText.push_back(decryptedBlock.at(j)^block.at(j));
    }

    incrementCounter();
  }

  plainText.push_back('\0');
  std::cout<<plainText.data()<<'\n';
}


/* =================== CBC ==================== */


void CBC::clearPadding()
{
  /*
   * look up for padding and clear it
   * trailing bytes with values equal to number
   * padding example: AB C6 D0 00 A5 AA 98 7E 08 08 08 08 08 08 08 08
   */

  byte prev;
  byte cnt;

  for (auto it=plainText.begin(); it!=plainText.end(); it++) {
    if (*it >= PADDING_MIN && *it <= PADDING_MAX) {
      if (cnt == 0) {
        prev = *it;
      }

      if(*it!=prev) {
        cnt = 0;
      } else {
        cnt++;
      }
      prev = *it;
      if (std::next(it) == plainText.end() && cnt == *it ) {
        std::replace(plainText.begin(), plainText.end(), cnt, (unsigned char)0);
      }
    }
  }

}


void CBC::decrypt(const std::string& aAESKey, const std::string& aCT)
{
  clearData();
  setKey(aAESKey);
  setCipherTextAndIV(aCT);

  for (auto i=0u; i<(cipherTextMsg.size()/BLOCK_SIZE); i++) {
    auto block = getBlock(cipherTextMsg, i);
    bytes decryptedBlock(BLOCK_SIZE);

    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption d;
    d.SetKey(key.data(), BLOCK_SIZE);
    d.ProcessData(decryptedBlock.data(), block.data(), BLOCK_SIZE);

    for (auto j=0u; j<BLOCK_SIZE; j++) {
      plainText.push_back(decryptedBlock.at(j)^iv.at(j));
    }

    iv.insert(iv.begin(), block.begin(), block.end());
  }

  clearPadding();

  plainText.push_back('\0');
  std::cout<<plainText.data()<<'\n';
}




#endif /* BLOCK_CIPHER_CPP_ */
