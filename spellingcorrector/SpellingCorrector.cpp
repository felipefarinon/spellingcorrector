#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "SpellingCorrector.h"

using namespace std;

bool sortBySecond(const pair<std::string, int>& left, const pair<std::string, int>& right)
{
  return left.second < right.second;
}

char filterNonAlphabetic(char& letter)
{
  if (letter < 0)
    return '-';
  if (isalpha(letter))
    return tolower(letter);
  return '-';
}

void SpellingCorrector::load(const std::string& filename)
{
  ifstream file(filename.c_str(), ios_base::binary | ios_base::in);

  file.seekg(0, ios_base::end);
  std::streampos length = file.tellg();
  file.seekg(0, ios_base::beg);

  string data(static_cast<std::size_t>(length), '\0');

  file.read(&data[0], length);

  transform(data.begin(), data.end(), data.begin(), filterNonAlphabetic);

  for (string::size_type i = 0; i != string::npos;)
  {
    const string::size_type firstNonFiltered = data.find_first_not_of('-', i + 1);
    if (firstNonFiltered == string::npos)
      break;

    const string::size_type end = data.find('-', firstNonFiltered);
    dictionary[data.substr(firstNonFiltered, end - firstNonFiltered)]++;

    i = end;
  }
}

string SpellingCorrector::correct(const std::string& word)
{
  Vector result;
  Dictionary candidates;

  if (dictionary.find(word) != dictionary.end()) { return word; }

  edits(word, result);
  known(result, candidates);

  if (candidates.size() > 0) { return max_element(candidates.begin(), candidates.end(), sortBySecond)->first; }

  for (unsigned int i = 0;i < result.size();i++)
  {
    Vector subResult;

    edits(result[i], subResult);
    known(subResult, candidates);
  }

  if (candidates.size() > 0) { return max_element(candidates.begin(), candidates.end(), sortBySecond)->first; }

  return "";
}

void SpellingCorrector::known(Vector& results, Dictionary& candidates)
{
  Dictionary::iterator end = dictionary.end();

  for (unsigned int i = 0;i < results.size();i++)
  {
    Dictionary::iterator value = dictionary.find(results[i]);

    if (value != end) candidates[value->first] = value->second;
  }
}

void SpellingCorrector::edits(const std::string& word, Vector& result)
{
  for (string::size_type i = 0;i < word.size(); i++)    result.push_back(word.substr(0, i) + word.substr(i + 1)); //deletions
  for (string::size_type i = 0;i < word.size() - 1;i++) result.push_back(word.substr(0, i) + word[i + 1] + word[i] + word.substr(i + 2)); //transposition

  for (char j = 'a';j <= 'z';++j)
  {
    for (string::size_type i = 0;i < word.size(); i++)    result.push_back(word.substr(0, i) + j + word.substr(i + 1)); //alterations
    for (string::size_type i = 0;i < word.size() + 1;i++) result.push_back(word.substr(0, i) + j + word.substr(i)); //insertion
  }
}