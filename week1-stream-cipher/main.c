/*
 ============================================================================
 Name        : main.c
 Author      : Krystian Heberlein
 Email       : krystianheberlein@gmail.com
 Description : Coursera Cryptography I Programming Assignment - Week 1
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CT_ARR_SIZE         11
#define MAX_PAD_LEN         200
#define BYTE_STR_LEN        2

const char* ciphertexts[CT_ARR_SIZE] = {
 "315c4eeaa8b5f8aaf9174145bf43e1784b8fa00dc71d885a804e5ee9fa40b163"
 "49c146fb778cdf2d3aff021dfff5b403b510d0d0455468aeb98622b137dae857"
 "553ccd8883a7bc37520e06e515d22c954eba5025b8cc57ee59418ce7dc6bc415"
 "56bdb36bbca3e8774301fbcaa3b83b220809560987815f65286764703de0f3d5"
 "24400a19b159610b11ef3e",
 "234c02ecbbfbafa3ed18510abd11fa724fcda2018a1a8342cf064bbde548b12b"
 "07df44ba7191d9606ef4081ffde5ad46a5069d9f7f543bedb9c861bf29c7e205"
 "132eda9382b0bc2c5c4b45f919cf3a9f1cb74151f6d551f4480c82b2cb24cc5b"
 "028aa76eb7b4ab24171ab3cdadb8356f",
 "32510ba9a7b2bba9b8005d43a304b5714cc0bb0c8a34884dd91304b8ad40b62b"
 "07df44ba6e9d8a2368e51d04e0e7b207b70b9b8261112bacb6c866a232dfe257"
 "527dc29398f5f3251a0d47e503c66e935de81230b59b7afb5f41afa8d661cb",
 "32510ba9aab2a8a4fd06414fb517b5605cc0aa0dc91a8908c2064ba8ad5ea06a"
 "029056f47a8ad3306ef5021eafe1ac01a81197847a5c68a1b78769a37bc8f457"
 "5432c198ccb4ef63590256e305cd3a9544ee4160ead45aef520489e7da7d8354"
 "02bca670bda8eb775200b8dabbba246b130f040d8ec6447e2c767f3d30ed81ea"
 "2e4c1404e1315a1010e7229be6636aaa",
 "3f561ba9adb4b6ebec54424ba317b564418fac0dd35f8c08d31a1fe9e24fe568"
 "08c213f17c81d9607cee021dafe1e001b21ade877a5e68bea88d61b93ac5ee0d"
 "562e8e9582f5ef375f0a4ae20ed86e935de81230b59b73fb4302cd95d770c65b"
 "40aaa065f2a5e33a5a0bb5dcaba43722130f042f8ec85b7c2070",
 "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd2061bbde24eb76a"
 "19d84aba34d8de287be84d07e7e9a30ee714979c7e1123a8bd9822a33ecaf512"
 "472e8e8f8db3f9635c1949e640c621854eba0d79eccf52ff111284b4cc61d119"
 "02aebc66f2b2e436434eacc0aba938220b084800c2ca4e693522643573b2c4ce"
 "35050b0cf774201f0fe52ac9f26d71b6cf61a711cc229f77ace7aa88a2f19983"
 "122b11be87a59c355d25f8e4",
 "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd90f1fa6ea5ba47b"
 "01c909ba7696cf606ef40c04afe1ac0aa8148dd066592ded9f8774b529c7ea12"
 "5d298e8883f5e9305f4b44f915cb2bd05af51373fd9b4af511039fa2d96f8341"
 "4aaaf261bda2e97b170fb5cce2a53e675c154c0d9681596934777e2275b381ce"
 "2e40582afe67650b13e72287ff2270abcf73bb028932836fbdecfecee0a3b894"
 "473c1bbeb6b4913a536ce4f9b13f1efff71ea313c8661dd9a4ce",
 "315c4eeaa8b5f8bffd11155ea506b56041c6a00c8a08854dd21a4bbde54ce568"
 "01d943ba708b8a3574f40c00fff9e00fa1439fd0654327a3bfc860b92f89ee04"
 "132ecb9298f5fd2d5e4b45e40ecc3b9d59e9417df7c95bba410e9aa2ca24c547"
 "4da2f276baa3ac325918b2daada43d6712150441c2e04f6565517f317da9d3",
 "271946f9bbb2aeadec111841a81abc300ecaa01bd8069d5cc91005e9fe4aad6e"
 "04d513e96d99de2569bc5e50eeeca709b50a8a987f4264edb6896fb537d0a716"
 "132ddc938fb0f836480e06ed0fcd6e9759f40462f9cf57f4564186a2c1778f15"
 "43efa270bda5e933421cbe88a4a52222190f471e9bd15f652b653b7071aec59a"
 "2705081ffe72651d08f822c9ed6d76e48b63ab15d0208573a7eef027",
 "466d06ece998b7a2fb1d464fed2ced7641ddaa3cc31c9941cf110abbf409ed39"
 "598005b3399ccfafb61d0315fca0a314be138a9f32503bedac8067f03adbf357"
 "5c3b8edc9ba7f537530541ab0f9f3cd04ff50d66f1d559ba520e89a2cb2a83",
 "32510ba9babebbbefd001547a810e67149caee11d945cd7fc81a05e9f85aac65"
 "0e9052ba6a8cd8257bf14d13e6f0a803b54fde9e77472dbff89d71b57bddef12"
 "1336cb85ccb8f3315f4b52e301d16e9f52f904"
};


int get_string_size(const char* str)
{
  return strlen(str);
}

unsigned char* string_to_bytes(const char* str)
{
  int i, di;

  int str_size = get_string_size(str);
  unsigned char *b_array = malloc((str_size*2)*sizeof(unsigned char));
  memset(b_array, 0, str_size/2);

  for (i=0, di=0; i<str_size; i=i+BYTE_STR_LEN, di++) {
    char sub_str[BYTE_STR_LEN];
    strncpy(sub_str, str+i, BYTE_STR_LEN);
    b_array[di] = strtoul(sub_str, NULL, 16) & 0xFF;
  }

  return b_array;
}


unsigned char is_character_valid(unsigned char _c)
{
  return ((_c >= 'A' && _c <= 'Z') || (_c >= 'a' && _c <= 'z'));
}

struct PAD {
    unsigned char* pad;
    unsigned char* _subst1;
    unsigned char* _subst2;
};


void xor_strings(
    struct PAD* _pad,
    const char* _str1,
    const char* _str2
)
{
  int i;
  unsigned char xored_byte, pad_byte, cpb1, cpb2, ppb1, ppb2;
  unsigned char *str1 = string_to_bytes(_str1);
  int str_size1 = get_string_size(_str1);
  unsigned char *str2 = string_to_bytes(_str2);
  int str_size2 = get_string_size(_str2);
  int shorter_len = (str_size1 >= str_size2) ? str_size2 : str_size1;

  for (i=0; i<shorter_len; i++) {

    if (str1[i] == 0 || str2[i] == 0) {
      break;
    }

    pad_byte = 0;

     /* If encrypted character is xored with encrypted space,
      * then result is decrypted character changed to lower or upper case,
      * because space in ascii is 0x20.
      * For an example 'a' 0x61 xored with 0x20 equals 0x41 'A'
      * Pad is ignored, because it xores itself. */

    xored_byte = (str1[i]^str2[i]);

    if (is_character_valid(xored_byte)) {

      /* Recover pad byte, but there are always two possibilities,
       * because we are not sure, which of encrypted bytes
       * was encrypted space character.*/

      cpb1 = str1[i]^' ';
      cpb2 = str2[i]^' ';

      if (_pad->pad[i] == 0) {
        /* To find the right pad byte, look for value which has been obtained at least twice */
        if (_pad->_subst1[i]==0 && _pad->_subst2[i]==0) {
          _pad->_subst1[i] = cpb1;
          _pad->_subst2[i] = cpb2;
        } else {
          ppb1 = _pad->_subst1[i];
          ppb2 = _pad->_subst2[i];
          /* Compare pair of current pad byte with previously obtained values.
           * It is highly probable, that repeated value is the right pad byte.*/
          if (ppb1 == cpb1 && ppb1 != cpb2 && ppb2 != cpb1 && ppb2 != cpb2) {
            pad_byte = cpb1;
          } else if (ppb1 != cpb1 && ppb1 == cpb2 && ppb2 != cpb1 && ppb2 != cpb2) {
            pad_byte = cpb2;
          } else if (ppb1 != cpb1 && ppb1 != cpb2 && ppb2 == cpb1 && ppb2 != cpb2) {
            pad_byte = cpb1;
          } else if (ppb1 != cpb1 && ppb1 != cpb2 && ppb2 != cpb1 && ppb2 == cpb2) {
            pad_byte = cpb2;
          }

          if (pad_byte != 0) {
            _pad->pad[i] = pad_byte;
          }
        }
      }

    }
  }
}


void print_str(unsigned char* _str)
{
  int i;
  for (i=0; i<MAX_PAD_LEN; i++) {
    printf("%02x ", _str[i]);
  }
  printf("\n");
}

void decrypt_message(const char* _msg, unsigned char* _pad)
{
  int i;
  unsigned char derypted_byte;
  unsigned char *str1 = string_to_bytes(_msg);

  for (i=0; i<MAX_PAD_LEN; i++) {
    derypted_byte = (str1[i]^_pad[i]);
    if (i!=0) printf("  ");
    if (_pad[i] == 0) {
      putchar('?');
    } else {
      putchar(derypted_byte);
    }
  }
  printf("\n");
}





int main(void) {

  struct PAD* pad = malloc(sizeof(struct PAD));

  pad->pad = calloc(MAX_PAD_LEN, sizeof(unsigned char));
  pad->_subst1 = calloc(MAX_PAD_LEN, sizeof(unsigned char));
  pad->_subst2 = calloc(MAX_PAD_LEN, sizeof(unsigned char));

  int i, j;

  /* XOR each cipher text with the others to recover pad */
  for (i=0; i<CT_ARR_SIZE; i++) {
    for (j=i+1; j<CT_ARR_SIZE; j++) {
      xor_strings(pad, ciphertexts[i], ciphertexts[j]);
    }
  }

  free(pad->_subst1);
  free(pad->_subst2);

  /* We can predict the right value of missing pad bytes
   * by reading partly decrypted messages.
   * Those values has been calculated by hand. */

  pad->pad[7] = 0xcc;
  pad->pad[10] = 0x35;
  pad->pad[18] = 0xce;
  pad->pad[25] = 0x7f;
  pad->pad[26] = 0x6b;
  pad->pad[31] = 0x0b;
  pad->pad[49] = 0x63;
  pad->pad[50] = 0xfe;
  pad->pad[56] = 0xd8;
  pad->pad[61] = 0xa9;
  pad->pad[72] = 0x3a;
  pad->pad[33] = 0xb0;
  pad->pad[34] = 0x33;
  pad->pad[35] = 0x9a;
  pad->pad[36] = 0x19;

  pad->pad[65] = 0x5d;
  pad->pad[86] = 0x30;
  pad->pad[92] = 0xb8;

  pad->pad[82] = 0x61;
  pad->pad[83] = 0x10;
  pad->pad[86] = 0x3e;


  pad->pad[99] = 0x02;
  pad->pad[101] = 0x16;
  pad->pad[93] = 0x04;

  pad->pad[94] = 0xa3;
  pad->pad[101] = 0xc6;
  pad->pad[109] = 0xca;
  pad->pad[110] = 0x50;
  pad->pad[113] = 0x61;


  printf("Recovered pad: \n");
  print_str(pad->pad);

  /* Decrypt all cipher texts */
  for (i=0; i<CT_ARR_SIZE; i++) {
    unsigned char *ciph = string_to_bytes(ciphertexts[i]);
    printf("Decrypted message %d: \n", i);
    print_str(ciph);
    decrypt_message(ciphertexts[i], pad->pad);
  }

  free(pad->pad);
  free(pad);

  return EXIT_SUCCESS;
}
