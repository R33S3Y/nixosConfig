#include "utils.h"
#include <map>
#include <vector>

using namespace std;

// template <typename type>
vector<vector<type>> utils::splitVector(vector<type> vec, int splits) {
  vector<vector<type>> chunks;
  int size = vec.size() / splits;
  int remainder = vec.size() % splits;

  auto it = vec.begin();
  for (int i = 0; i < splits; i++) {
    int chunkSize = size + (i < remainder ? 1 : 0);
    chunks.emplace_back(it, it + chunkSize);
    it += chunkSize;
  }
  return chunks;
}
// template <typename key, typename value>
vector<map<key, value>> utils::splitMap(map<key, value> inputMap, int splits) {
  vector<map<key, value>> chunks;
  int size = inputMap.size() / splits;
  int remainder = inputMap.size() % splits;

  auto it = inputMap.begin();
  for (int i = 0; i < splits; i++) {
    int chunkSize = size + (i < remainder ? 1 : 0);
    map<key, value> hold;
    for (int j = 0; j < chunkSize && it != inputMap.end(); j++, it++) {
      hold.insert(*it);
    }
    chunks.push_back(hold);
  }
  return chunks;
}
