#pragma once

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

  template <typename keyType, typename valueIn, typename valueOut,
            typename argIn>
  static map<keyType, valueOut> paralleliseMap(map<keyType, valueIn> items,
                                               argIn func,
                                               const int maxThreads = 8);

private:
  template <typename typeIn, typename typeOut, typename argIn>
  static vector<typeOut> workerVector(int id, vector<typeIn> items, argIn func);
  template <typename keyType, typename valueIn, typename valueOut,
            typename argIn>
  static map<keyType, valueOut> workerMap(int id, map<keyType, valueIn> items,
                                          argIn func);
};

#include "threading.tpp"
