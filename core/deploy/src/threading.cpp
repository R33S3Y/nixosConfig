
#include "threading.h"
#include <any>
#include <functional>
#include <thread>
#include <vector>
using namespace std;

vector<any> threading::parallelise(vector<any> items, function<any(any)> func,
                                   const int maxThreads) {
  vector<thread> threads;
}
