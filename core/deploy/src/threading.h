#pragma once

#include "threading.tpp"
#include <complex>
#include <functional>
#include <map>
#include <vector>
using namespace std;

class threading {
public:
  template <typename typeIn, typename typeOut, typename argIn>
  static vector<typeOut> paralleliseVector(vector<typeIn> items, argIn func,
                                           const int maxThreads = 8);

  template <typename key, typename valueIn, typename valueOut, typename argIn>
  static map<key, valueOut> paralleliseMap(map<key, valueIn> items, argIn func,
                                           const int maxThreads = 8);

private:
  template <typename typeIn, typename typeOut, typename argIn>
  static vector<typeOut> workerVector(int id, vector<typeIn> items, argIn func);
  template <typename key, typename valueIn, typename valueOut, typename argIn>
  static map<key, valueOut> workerMap(int id, map<key, valueIn> items,
                                      argIn func);
};
