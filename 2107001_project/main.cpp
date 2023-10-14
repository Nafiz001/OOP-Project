#include <bits/stdc++.h>
using namespace std;


const unsigned int length = 5;
const unsigned int chances = 5;

enum LetterIs {
    Match,
    Contained,
    Nonexistent
};

template<typename T>
class CustomVector {
private:
    T* data;
    size_t size;
    size_t capacity;

public:
    CustomVector() : data(nullptr), size(0), capacity(0) {}

    CustomVector(size_t initialCapacity) : size(0), capacity(initialCapacity) {
        data = new T[capacity];
    }

    ~CustomVector() {
        delete[] data;
    }

    void push_back(const T& element) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = element;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }


    friend bool areEqual(const CustomVector<T>& vec1, const CustomVector<T>& vec2) {
        if (vec1.size != vec2.size) {
            return false;
        }

        for (size_t i = 0; i < vec1.size; i++) {
            if (vec1.data[i] != vec2.data[i]) {
                return false;
            }
        }

        return true;
    }




private:
    void resize() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
};

class WordleGame {
public:
    virtual void PlayGame() = 0;
};

class FileWordProvider {
public:
    virtual string GetRandomWord() = 0;
};

class FileWordProviderImpl : public FileWordProvider {
private:
    CustomVector<string> words;

public:
    FileWordProviderImpl(const string& filePath) {
        ifstream file(filePath);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                words.push_back(line);
            }
            file.close();
        } else {
            cout << "Failed to open the file." << endl;
            cin.get();
            exit(1);
        }
    }

    string GetRandomWord()  {
        return words[rand() % words.getSize()];
    }
};

class WordleGameImpl : public WordleGame {
private:
    FileWordProvider& wordProvider;
    map<LetterIs, string> displayLetterIs;

public:
    WordleGameImpl(FileWordProvider& provider) : wordProvider(provider) {
        displayLetterIs[Match] = "Match";
        displayLetterIs[Contained] = "Contained";
        displayLetterIs[Nonexistent] = "Nonexistent";
    }

    void PlayGame()  {
        srand(static_cast<unsigned int>(time(nullptr)));

        while (true) {
            string wordle = wordProvider.GetRandomWord();
            for (int i = 0; i < chances; i++) {
                cout << "Guess [" << i + 1 << '/' << chances << "]: ";
                string guess;
                cin >> guess;
                if (guess.length() == length) {
                    CheckGuess(wordle, guess);
                } else {
                    cout << "That was invalid, please try again." << endl;
                    i--;
                }
            }
            cout << "The wordle is: " << wordle << endl;
        }
    }

private:
    void CheckGuess(const string& wordle, const string& guess) {

        CustomVector<LetterIs> lettersAre;
        bool won = true;

        for (int j = 0; j < length; j++) {
            if (guess[j] == wordle[j]) {
                lettersAre.push_back(Match);
            } else {
                bool nonexistent = true;
                for (int k = 0; k < length; k++) {
                    if (guess[j] == wordle[k]) {
                        lettersAre.push_back(Contained);
                        nonexistent = false;
                    }
                }
                if (nonexistent) {
                    lettersAre.push_back(Nonexistent);
                }
            }

            cout << '#' << j + 1 << "\n  " << guess[j] << ": " << displayLetterIs[lettersAre[j]] << '\n';

            if (lettersAre[j] != Match) {
                won = false;
            }
        }

        if (won) {
            cout << "You won!" << endl;

        }

    }

};

int main() {
    cout<<"                          **************************WORDLE****************************"<<endl<<endl;
    cout<<"1.Match means the letter is in that particular position."<<endl;

    cout<<"2.Contained means the letter is in the word but not in that position."<<endl;

    cout<<"3.Nonexistent means the letter is not in the word."<<endl<<endl;

    FileWordProviderImpl wordProvider("words.txt");

    WordleGameImpl game(wordProvider);

    game.PlayGame();

    return 0;
}
