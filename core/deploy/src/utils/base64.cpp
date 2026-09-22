#include "base64.h"
#include <cstdint>
#include <vector>

using namespace std;

vector<char> base64Chars = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

string base64::encode(vector<unsigned char> unEncoded) {
  string encodedStr;
  encodedStr.reserve((unEncoded.size() / 3 + 1) * 4);

  // handles everything except the last 3 that need trail bits.
  int i;
  for (i = 0; i < (unEncoded.size() / 3 * 3); i += 3) {
    uint32_t bitHold =
        (unEncoded[i] << 16) | (unEncoded[i + 1] << 8) | unEncoded[i + 2];
    encodedStr.push_back(base64Chars[((bitHold >> 18) & 0x3F)]);
    encodedStr.push_back(base64Chars[((bitHold >> 12) & 0x3F)]);
    encodedStr.push_back(base64Chars[((bitHold >> 6) & 0x3F)]);
    encodedStr.push_back(base64Chars[(bitHold & 0x3F)]);
  }
  i -= 3; // remove the last run that it didn't do.

  if (unEncoded.size() - i == 2) {
    uint32_t bitHold = (unEncoded[i] << 16) | (unEncoded[i + 1] << 8);
    encodedStr.push_back(base64Chars[((bitHold >> 18) & 0x3F)]);
    encodedStr.push_back(base64Chars[((bitHold >> 12) & 0x3F)]);
    encodedStr.push_back(base64Chars[((bitHold >> 6) &)]);
  }
  if (unEncoded.size() - i == 1) {
    uint32_t bitHold = (unEncoded[i] << 16);
    encodedStr.push_back(base64Chars[((bitHold >> 18) & 0x3F)]);
    encodedStr.push_back(base64Chars[((bitHold >> 12) &)]);
  }

  return encodedStr;
}
