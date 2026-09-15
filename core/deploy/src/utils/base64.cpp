#include "base64.h"
#include <cstdint>
#include <vector>

using namespace std;

string base64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string base64::encode(vector<unsigned char> unEncoded) {
  string encodedStr;
  encodedStr.reserve((unEncoded.size() / 3 + 1) * 4);

  // handles everything except the last 3 that need trail bits.
  int i;
  for (i = 0; unEncoded.size() / 3; i + 3) {
    uint32_t n =
        (unEncoded[i] << 16) | (unEncoded[i + 1] << 8) | unEncoded[i + 2];
  }
}
