#include <string>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <chrono>
using namespace std;


bool isWord(string line, char letters[], int numOfLetters, int numDifferencesAllowed = 0){
    transform(line.begin(), line.end(), line.begin(), ::tolower); // Convert to lowercase
    string fullWord = line;

    if(line.length()>numOfLetters+numDifferencesAllowed || line.length()<numOfLetters){
        return false;
    } else if(line.length()>numOfLetters){
        numDifferencesAllowed = numDifferencesAllowed - (line.length()-numOfLetters);
    }
    bool isWordCheck = true;
    int numDifferences = 0;
    int* positionsChecked = new int[max(numOfLetters+numDifferencesAllowed,int(line.length()))];
    for (int i = 0; i < max(numOfLetters+numDifferencesAllowed,int(line.length())); i++)
    {
        positionsChecked[i] = -1;
    } // Initialising array. PositionsChecked represents each letter in line, is -1 if not been checked as being in letters[] yet, is the position in letters if it is.
    int index;
    for (int i = 0; i < numOfLetters; i++)
    {
        line = fullWord;
        index = line.find(letters[i]);
        if(index==line.npos){ // Character not in word
            numDifferences++;
            if(numDifferences>numDifferencesAllowed){
                isWordCheck = false;
                delete[] positionsChecked;
                return isWordCheck;
            }
        }
        else{ // character in word
            int numRemovals = 0;
            while (positionsChecked[index]!=-1){ // If there are multiple of the same letter in the word and already checked one of them
                line.erase(index,1);
                numRemovals++;
                index = line.find(letters[i]);
                if (line.find(letters[i])==line.npos){ // If no more of that letter in word and still letter remaining in letters[]
                    isWordCheck = false;
                    delete[] positionsChecked;
                    return isWordCheck;
                }
                index = index+numRemovals; // If is in word, make index aim to correct position and compensate for removed letters
            }
            positionsChecked[index] = i;
        }
    };

    delete[] positionsChecked;
    return isWordCheck;
}

int main(){
    const int MAXNUMOFLETTERS = 100;
    ifstream wordsFile;
    wordsFile.open("words.txt");
    string line;
    cout << "Enter the letters: ";
    char* letters = new char[MAXNUMOFLETTERS]; // Array to store the letters
    cin.getline(letters,MAXNUMOFLETTERS);
    int numOfLetters = 0;
    while (letters[numOfLetters] != '\0') {
        letters[numOfLetters] = char(tolower(letters[numOfLetters]));
        ++numOfLetters;
    }

    cout << "Include 1 letter difference? (y/n): ";
    char includeOneDifference;
    cin >> includeOneDifference;

    cout << "Include 1 letter difference in length? (y/n): ";
    char includeOneDifferenceInLength;
    cin >> includeOneDifferenceInLength;

    // Measure time taken
    auto start = chrono::high_resolution_clock::now();

    if(wordsFile.is_open()){
        while (getline(wordsFile, line)) // while there are lines in the file
        {
            if(line.length() == numOfLetters){ //If could be whole word.
                if(isWord(line, letters, numOfLetters)){
                    cout << "Perfect word "<<line << "\n";
                } else if(includeOneDifference == 'y' && isWord(line, letters, numOfLetters,1)){
                    cout << "Only 1 letter different: " << line << "\n";
                }
            }
            if(includeOneDifferenceInLength == 'y' && line.length() == numOfLetters+1){
                if(isWord(line, letters, numOfLetters,1)){
                    cout << "Only 1 letter different in length: " << line << "\n";
                }
            }
        }
        cout << "Done!" << "\n";
        wordsFile.close();
    }
    delete[] letters;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " milliseconds" << "\n";
    return 0;
}
