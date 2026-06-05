#include "utils.h"
#include <map>
#include <vector>

using namespace std;

template <typename type>
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
template <typename keyType, typename valueType>
vector<map<keyType, valueType>>
utils::splitMap(map<keyType, valueType> inputMap, int splits) {
  vector<map<keyType, valueType>> chunks;
  int size = inputMap.size() / splits;
  int remainder = inputMap.size() % splits;

  auto it = inputMap.begin();
  for (int i = 0; i < splits; i++) {
    int chunkSize = size + (i < remainder ? 1 : 0);
    map<keyType, valueType> hold;
    for (int j = 0; j < chunkSize && it != inputMap.end(); j++, it++) {
      hold.insert(*it);
    }
    chunks.push_back(hold);
  }
  return chunks;
}
