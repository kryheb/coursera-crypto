/*
 * main.cpp
 *
 *  Created on: Feb 14, 2018
 *      Author: Krystian Heberlein
 *      Email:  krystianheberlein@gmail.com
 */

#include <vector>

#include "block_cipher.hpp"

const std::string keyCbc = "140b41b22a29beb4061bda66b6747e14";
const std::string keyCtr = "36f18357be4dbd77f050515c73fcf9f2";

const std::vector<std::string> ctsCbc = {

    "4ca00ff4c898d61e1edbf1800618fb28"
    "28a226d160dad07883d04e008a7897ee"
    "2e4b7465d5290d0c0e6c6822236e1daa"
    "fb94ffe0c5da05d9476be028ad7c1d81",

    "5b68629feb8606f9a6667670b75b38a5"
    "b4832d0f26e1ab7da33249de7d4afc48"
    "e713ac646ace36e872ad5fb8a512428a"
    "6e21364b0c374df45503473c5242a253"

};

const std::vector<std::string> ctsCtr = {

    "69dda8455c7dd4254bf353b773304eec"
    "0ec7702330098ce7f7520d1cbbb20fc3"
    "88d1b0adb5054dbd7370849dbf0b88d3"
    "93f252e764f1f5f7ad97ef79d59ce29f"
    "5f51eeca32eabedd9afa9329",

    "770b80259ec33beb2561358a9f2dc617"
    "e46218c0a53cbeca695ae45faa8952aa"
    "0e311bde9d4e01726d3184c34451"

};



int main(int argc, char* argv[]) {

  CBC cbc;

  for (auto &ct : ctsCbc) {
    cbc.decrypt(keyCbc, ct);
  }

  CTR ctr;
  for (auto &ct : ctsCtr) {
    ctr.decrypt(keyCtr, ct);
  }



  return 1;
}
