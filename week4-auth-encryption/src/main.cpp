#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include "message.hpp"

constexpr auto url =
  "http://crypto-class.appspot.com/po?er=";

constexpr auto ciphertext =
  "f20bdba6ff29eed7b046d1df9fb70000"
  "58b1ffb4210a580f748b4ac714c001bd"
  "4a61044426fb515dad3f21f18aa577c0"
  "bdf302936266926ff37dbf7035d5eeb2";

int main(void)
{
  Logger::getInstance().init();

  Message m("f20bdba6ff29eed7b046d1df9fb70000"
            "58b1ffb4210a580f748b4ac714c001bd"
            "4a61044426fb515dad3f21f18aa577c0"
            "bdf302936266926ff37dbf7035d5eeb2");

  return 0;
}
