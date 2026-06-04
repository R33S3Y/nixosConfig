#pragma once

#include <any>
#include <functional>
#include <vector>
using namespace std;

class threading {
public:
  static vector<any> parallelise(vector<any> items, function<any(any)> func,
                                 const int maxThreads = 10);
};
