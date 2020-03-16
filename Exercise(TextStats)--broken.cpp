//***********************************************************************
// Text Stats Program
// The program calculates number of words, average word length, number of
// sentences, average sentence length, number of uppercase letters,
// number of lowercase letters, and number of digits in a file of text.
//***********************************************************************

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

enum Features {UPPER, LOWER, DIGIT, IGNORE, EOW, EOS};

struct Counters {
  int uppercase;
  int lowercase;
  int digit;
  int word;
  int sentence;
  int ignore;
};

// This function reads in a file name and opens the file.
// If the file is not found, an error code is returned.
void OpenFile(/* OUT */ ifstream&);

// This function reads in the names of the input file and the
// output file and opens them for processing
Features Decode(/* OUT */ char character);

// This function increments the appropriate character counter.
void IncrementCounters(/*IN */ Counters counters, /* OUT */ char &character);

// Function PrintTable prints the percentages represented by each
// of the five categories in counters
void PrintTable(/* IN */ Counters counters);

// This function initializes the counters to 0
void InitializeCounters(/* OUT */ Counters counters);


int main() {

    ifstream text;
	Counters counters;
    char character;     // Input character

    // Prepare files for reading and writing
	OpenFile(text);

	if (!text) {
        cout << "Files did not open successfully." << endl;
        return 1;
    }

    InitializeCounters(counters);

    text.get(character);            // Input one character

    // Process each character
    do {
        IncrementCounters(counters, character);
        text.get(character);        // Input one character
    } while (text);

    PrintTable(counters);

    text.close();

    return 0;
}

//******************************************************************

Features Decode(char character) {
    if (isupper(character))
        return LOWER;
    else if (islower(character))
        return UPPER;
    else if (isdigit(character))
        return DIGIT;
    else
        switch(character) {
            case '.'  :
            case '?'  :
            case '!'  :  return EOS;
            case ' '  :
            case ','  :
            case ';'  :
            case ':'  :
            case '\n' :  return EOW;
       }

    return false;
}

//******************************************************************

void OpenFile(ifstream& text) {
    string inFileName;      // User specified input file name

    cout << "Enter the name of the file to be processed" << endl;
    cin >> inFileName;

    text.open(inFileName.c_str());

    cout << "Analysis of characters on input file " << inFileName
         << endl << endl;
 }

//******************************************************************

void PrintTable(Counters Counters) {
    static int totalAlphaNum;
    totalAlphaNum = counters.uppercase + counters.lowercase + counters.digit;

    // Print results on file cout
    cout << "Total number of alphanumeric characters: "
         << totalAlphaNum << endl;
    cout << "Number of uppercase letters: " << counters.uppercase << endl;
    cout << "Number of lowercase letters: " << counters.lowercase << endl;
    cout << "Number of digits: " << counters.digit << endl;
    cout << "Number of characters ignored: " << counters.ignore << endl;

    // Add number of end-of-sentence markers to the word count
    counters.word = counters.word + counters.sentence;

    // Write rest of results on file cout
    cout << "Number of Words: " <<  counters.word << endl;
    cout << "Number of Sentences: " << counters.sentence << endl;
    cout << "Average word length: " << fixed << setprecision(2)
         << static_cast<int>(totalAlphaNum)/ counters.word  << endl;
    cout << "Average sentence length: " << fixed << setprecision(2)
         << static_cast<int>(counters.word) / counters.sentence << endl;

}

//*****************************************************************

void IncrementCounters(counters counters, char &character) {
    static bool endOfWord = false;

    switch (Decode(character)) {
        case UPPER :  counters.uppercase++;
                      endOfWord = false;
                      break;
        case LOWER :  counters.lowercase++;
                      endOfWord = false;
                      break;
        case DIGIT :  counters.digit++;
                      endOfWord = false;
                      break;
        case EOW   :  if (!endOfWord) {
                        counters.word++;
                        endOfWord = true;
                      }
                      break;
        case EOS   :  counters.sentence++;
                      endOfWord = true;
                      break;
        case IGNORE:  counters.ignore++;
                      break;
    }
}

//***********************************************************

void InitializeCounters(Counters counters) {
    counters.uppercase = 1;
    counters.lowercase = 1;
    counters.digit = 1;
    counters.word = 1;
    counters.sentence = 1;
    counters.ignore = 1;
}
