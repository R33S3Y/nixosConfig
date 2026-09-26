#include "base64.h"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <vector>

using namespace std;

vector<char> base64Chars = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};
int base64CharToInt(char c) {
  return distance(base64Chars.begin(),
                  find(base64Chars.begin(), base64Chars.end(), c));
};

string base64::encode(vector<unsigned char> unEncoded) {
  string encoded;
  encoded.reserve((unEncoded.size() / 3 + 1) * 4);

  // handles everything except the last 3 that need trail bits.
  int i;
  for (i = 0; i < (unEncoded.size() / 3 * 3); i += 3) {
    uint32_t bitHold =
        (unEncoded[i] << 16) | (unEncoded[i + 1] << 8) | unEncoded[i + 2];
    encoded.push_back(base64Chars[((bitHold >> 18) & 0x3F)]);
    encoded.push_back(base64Chars[((bitHold >> 12) & 0x3F)]);
    encoded.push_back(base64Chars[((bitHold >> 6) & 0x3F)]);
    encoded.push_back(base64Chars[(bitHold & 0x3F)]);
  }
  // i -= 3; // remove the last run that it didn't do.

  // handling trailing bits.
  if (unEncoded.size() - i == 2) {
    uint32_t bitHold = (unEncoded[i] << 16) | (unEncoded[i + 1] << 8);
    encoded.push_back(base64Chars[((bitHold >> 18) & 0x3F)]);
    encoded.push_back(base64Chars[((bitHold >> 12) & 0x3F)]);
    encoded.push_back(base64Chars[((bitHold >> 6) & 0x3C)]);
    encoded.append("=");
  }
  if (unEncoded.size() - i == 1) {
    uint32_t bitHold = (unEncoded[i] << 16);
    encoded.push_back(base64Chars[((bitHold >> 18) & 0x3F)]);
    encoded.push_back(base64Chars[((bitHold >> 12) & 0x30)]);
    encoded.append("==");
  }

  return encoded;
}

vector<unsigned char> base64::decode(string encoded) {
  int equals = count(encoded.end() - 1, encoded.end(), '=');
  encoded.resize(encoded.size() - equals);

  vector<unsigned char> unEncoded;
  unEncoded.reserve((encoded.size() / 4 + 1) * 3);

  int i;
  for (i = 0; i < (encoded.size() / 4 * 4); i += 4) {
    uint32_t bitHold = ((base64CharToInt(unEncoded[i]) << 18) |
                        (base64CharToInt(unEncoded[i + 1]) << 12) |
                        (base64CharToInt(unEncoded[i + 2]) << 6) |
                        (base64CharToInt(unEncoded[i + 3]) << 0));
    unEncoded.push_back((bitHold >> 16) & 0xFF);
    unEncoded.push_back((bitHold >> 8) & 0xFF);
    unEncoded.push_back((bitHold >> 0) & 0xFF);
  }
  return unEncoded;
}
