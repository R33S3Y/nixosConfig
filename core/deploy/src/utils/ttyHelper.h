#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

class ttyHelper {
public:
  static string error(string message);
  static string warning(string message);
  static string progressBar(const int progress, const int total,
                            const int chars);
};
