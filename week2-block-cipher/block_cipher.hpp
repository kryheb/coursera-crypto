/*
 * block_cipher.hpp
 *
 *  Created on: Feb 19, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#ifndef BLOCK_CIPHER_HPP_
#define BLOCK_CIPHER_HPP_

#include <crypto++/modes.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>


using byte = unsigned char;
using bytes = std::vector<byte>;
const uint BLOCK_SIZE = CryptoPP::AES::BLOCKSIZE;
const uint BYTE_STRING_SIZE = 2;


/* Block Cipher base class */

class BlockCipher {

  protected:
    bytes key;
    bytes cipherTextMsg;
    bytes iv;
    bytes plainText;

    virtual ~BlockCipher(){}

    bytes stringToBytes(const std::string& aString);
    bytes getBlock(const bytes& aCTBytes, const uint aBlockIndex);
    void clearData();
    void setKey(const std::string& aKey);
    void setCipherTextAndIV(const std::string& aCT);

  public:
    virtual void decrypt(const std::string& aAESKey, const std::string& aCT)=0;
};


/* Counter Mode */

class CTR : public BlockCipher {

    static const uint COUNTER_MS_BYTE = 8;
    static const uint COUNTER_LS_BYTE = 15;
  public:
    CTR(){}
    virtual ~CTR(){}

    virtual void decrypt(const std::string& aAESKey, const std::string& aCT) override;

  private:
    void incrementCounter();
};


/* Cipher Block Chaining Mode */

class CBC : public BlockCipher {

    static const uint PADDING_MIN = 1;
    static const uint PADDING_MAX = 16;

  public:
    CBC(){}
    virtual ~CBC(){}

    virtual void decrypt(const std::string& aAESKey, const std::string& aCT) override;

  private:
    void clearPadding();
};


#endif /* BLOCK_CIPHER_HPP_ */
