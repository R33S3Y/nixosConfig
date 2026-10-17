#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

namespace systemHelper {

template <typename T> struct result {
  optional<T> output;
  int exitCode;
  optional<string> error;
};
template <> struct result<void> {
  int exitCode;
  optional<string> error;
};

result<string> runCommand(string cmd);
result<string> readFileToStr(const string &path);
result<vector<unsigned char>> readFile(const string &path);
result<void> saveFileFromStr(const string &path, const string &content);

string getProccessUser();
}; // namespace systemHelper
