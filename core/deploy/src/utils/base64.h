#pragma once

#include <string>
#include <vector>
using namespace std;

namespace base64 {

string encode(vector<unsigned char> unEncoded);
vector<unsigned char> decode(string encoded);

} // namespace base64
