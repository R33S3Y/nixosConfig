#pragma once

#include <string>
#include <vector>
using namespace std;

namespace base64 {

string encode(vector<unsigned char>);
vector<unsigned char> decode(string);

} // namespace base64
