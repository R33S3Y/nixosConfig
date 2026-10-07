#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <vector>
using namespace std;

namespace split {
/**
 * @brief Splits a str by the any instances of a char.
 * Removing instances of that char as it goes.
 */
vector<string> splitStrByChar(string inputStr, char inputChar);
/**
 * @brief Splits a str by the any instances of any chars.
 * Removing all instances of any filter chars as it goes.
 */
vector<string> splitStrByChars(string inputStr, vector<char> inputChars);
/**
 * @brief Works just like splitStrByChar, except it's looking for instances of
 * char in filterStr and applying the splits to inputStr.
 *
 * Removing all instances of where char is in filterStr in inputStr instead.
 *
 * @return if the two strs don't have matching size it will just return a empty
 * array.
 */
vector<string> splitStrByCharByFilterStr(string inputStr, string filterStr,
                                         char inputChar);
/**
 * @brief Works just like splitStrByChars, except it's looking for any instance
 * of anything in chars in filterStr and applying the splits to inputStr.
 *
 * Removing all/any instances of where chars were in filterStr in inputStr
 * instead.
 *
 * @return if the two strs don't have matching size it will just return a empty
 * array.
 */
vector<string> splitStrByCharsByFilterStr(string inputStr, string filterStr,
                                          vector<char> inputChars);

template <typename type>
vector<vector<type>> splitVector(vector<type> vec, int splits);

template <typename keyType, typename valueType>
vector<map<keyType, valueType>> splitMap(map<keyType, valueType> inputMap,
                                         int splits);
} // namespace split

#include "split.tpp"
