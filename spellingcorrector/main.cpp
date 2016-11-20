#include <iostream>
#include <string>
#include "SpellingCorrector.h"

using namespace std;

SpellingCorrector corrector;

void Correct(const std::string& wrong, const std::string& expected)
{
  const bool isCorrect = corrector.correct(wrong) == expected;
  std::cout << "(" << wrong << ") == (" << expected << ") = (" << std::boolalpha << isCorrect << ")" << std::endl;
}

int main()
{
  corrector.load("big.txt");

  Correct("speling", "spelling"); // insert
  Correct("korrectud", "corrected"); // replace 2
  Correct("bycycle", "bicycle"); // replace
  Correct("inconvient", "inconvenient"); // insert 2
  Correct("arrainged", "arranged"); // delete
  Correct("peotry", "poetry"); // transpose
  Correct("peotryy", "poetry"); // transpose + delete
  Correct("word", "word"); // known
  Correct("quintessential", ""); // unknown

	string request;
	while (request != "quit")
	{
		cout << "Enter a word\n";
		cin >> request;

		string correct(corrector.correct(request));

		if (correct != "")
			cout << "You meant: " << correct << "?\n\n\n";
		else
			cout << "No correction suggestion\n\n\n";
	}

	cin.get();

	return 0;
}