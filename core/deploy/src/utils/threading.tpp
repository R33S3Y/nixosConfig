#include "split.h"
#include "threading.h"
#include <functional>
#include <map>
#include <thread>
#include <vector>
using namespace std;

template <typename typeIn, typename typeOut, typename argIn>
vector<typeOut> threading::paralleliseVector(vector<typeIn> items, argIn func,
                                             int maxThreads) {
  int totalThreads = maxThreads;
  if (items.size() < maxThreads)
    totalThreads = items.size();

  vector<vector<typeIn>> threadWorkIn = split::splitVector(items, totalThreads);
  vector<vector<typeOut>> threadWorkOut(totalThreads);

  vector<thread> threads;

  for (int i = 0; i < totalThreads; i++) {
    threads.emplace_back([i, &threadWorkIn, &threadWorkOut, func]() {
      threadWorkOut[i] = threading::workerVector<typeIn, typeOut, argIn>(
          i, threadWorkIn[i], func);
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  vector<typeOut> output;
  for (int i = 0; i < totalThreads; i++) {
    output.insert(output.end(), threadWorkOut[i].begin(),
                  threadWorkOut[i].end());
  }
  return output;
}
template <typename typeIn, typename typeOut, typename argIn>
vector<typeOut> threading::workerVector(int id, vector<typeIn> items,
                                        argIn func) {
  vector<typeOut> output;
  for (int i = 0; i < items.size(); i++) {
    output.push_back(func(items[i]));
  }
  return output;
}

template <typename keyType, typename valueIn, typename valueOut, typename argIn>
map<keyType, valueOut> threading::paralleliseMap(map<keyType, valueIn> items,
                                                 argIn func, int maxThreads) {
  int totalThreads = maxThreads;
  if (items.size() < maxThreads)
    totalThreads = items.size();

  vector<map<keyType, valueIn>> threadWorkIn =
      split::splitMap(items, totalThreads);
  vector<map<keyType, valueOut>> threadWorkOut(totalThreads);
  vector<thread> threads;

  for (int i = 0; i < totalThreads; i++) {
    threads.emplace_back([i, &threadWorkIn, &threadWorkOut, func]() {
      threadWorkOut[i] =
          threading::workerMap<keyType, valueIn, valueOut, argIn>(
              i, threadWorkIn[i], func);
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  map<keyType, valueOut> output;
  for (int i = 0; i < totalThreads; i++) {
    output.insert(threadWorkOut[i].begin(), threadWorkOut[i].end());
  }
  return output;
}
template <typename keyType, typename valueIn, typename valueOut, typename argIn>
map<keyType, valueOut> threading::workerMap(int id, map<keyType, valueIn> items,
                                            argIn func) {
  map<keyType, valueOut> output;
  for (const auto &[key, val] : items) {
    output[key] = func(val);
  }
  return output;
}
