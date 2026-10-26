#pragma once
#include <optional>
#include <string>
#include <vector>
using namespace std;

namespace tarHelper {
template <typename T> struct result {
  optional<T> output;
  int exitCode;
  optional<string> error;
};
template <> struct result<void> {
  int exitCode;
  optional<string> error;
};
struct tarItem {
  string realPath;
  string tarPath;
};

result<void> package(const string tarPath, const vector<tarItem> items);
result<void> unpackage(const string tarPath, const string savePath);
} // namespace tarHelper
