#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <string>
#include <unordered_map>
#include <sstream>
#include <deque>

std::string input;
// Define the Turing machine class
class TuringMachine {
public:
    TuringMachine(const std::string& input);
    void run();
    void loadZeroOneTransitions();
    void loadReverseTransitions();

private:
    void printTape();
    void extendTape();
    std::string findTransitionRule();
    int updateTape(std::string &currentTransitionRule);


private:
    std::deque<char> tape;
    size_t headPosition;
    std::string currentState;
    std::vector<std::unordered_map<char, std::string > > transitionTable;

};

// Constructor for the Turing machine
TuringMachine::TuringMachine(const std::string& input) : tape(input.begin(), input.end()), headPosition(0), currentState("Q0") {}


void TuringMachine::extendTape() {

}

//load 0n1n transition rules
void TuringMachine::loadZeroOneTransitions() {

    //state 0 transitions
    std::unordered_map<char, std::string> stateZero;
    stateZero['0'] = "Q1,X,R";
    stateZero['Y'] = "Q3,Y,R";
    transitionTable.push_back(stateZero);

    //state 1 transitions
    std::unordered_map<char, std::string> stateOne;
    stateOne['0'] = "Q1,0,R";
    stateOne['1'] = "Q2,Y,L";
    stateOne['Y'] = "Q1,Y,R";
    transitionTable.push_back(stateOne);

    //state 2 transitions
    std::unordered_map<char, std::string> stateTwo;
    stateTwo['0'] = "Q2,0,L";
    stateTwo['X'] = "Q0,X,R";
    stateTwo['Y'] = "Q2,Y,L";
    transitionTable.push_back(stateTwo);

    //state 3 transitions
    std::unordered_map<char, std::string> stateThree;
    stateThree['Y'] = "Q3,Y,R";
    stateThree['-'] = "Q4,-,A";
    transitionTable.push_back(stateThree);

} 

void TuringMachine::loadReverseTransitions() {
    //state 0 transitions
    std::unordered_map<char, std::string> stateZero;
    stateZero['A'] = "Q0,A,R";
    stateZero['B'] = "Q0,B,R";
    stateZero['-'] = "Q1,-,L";
    transitionTable.push_back(stateZero);

    //state 1 transitions
    std::unordered_map<char, std::string> stateOne;
    stateOne['B'] = "Q3,X,R";
    stateOne['A'] = "Q2,X,R";
    stateOne['X'] = "Q1,X,L";
    stateOne['-'] = "Q5,-,R";
    transitionTable.push_back(stateOne);

    //state 2 transitions
    std::unordered_map<char, std::string> stateTwo;
    stateTwo['X'] = "Q2,X,R";
    stateTwo['B'] = "Q2,B,R";
    stateTwo['A'] = "Q2,A,R";
    stateTwo['-'] = "Q4,A,L";
    transitionTable.push_back(stateTwo);

    //state 3 transitions
    std::unordered_map<char, std::string> stateThree;
    stateThree['X'] = "Q3,X,R";
    stateThree['A'] = "Q3,A,R";
    stateThree['B'] = "Q3,B,R";
    stateThree['-'] = "Q4,B,L";
    transitionTable.push_back(stateThree);

    //state 4 transitions
    std::unordered_map<char, std::string> stateFour;
    stateFour['A'] = "Q4,A,L";
    stateFour['B'] = "Q4,B,L";
    stateFour['X'] = "Q1,X,L";
    transitionTable.push_back(stateFour);

    //state 5 transitions
    std::unordered_map<char, std::string> stateFive;
    stateFive['A'] = "Q6,A,A";
    stateFive['B'] = "Q6,B,A";
    stateFive['X'] = "Q5,-,R";
    transitionTable.push_back(stateFive);
}


// Function to print the tape and the tape head
void TuringMachine::printTape() {
    system("clear"); 
    std::cout << "Turing Machine Computing... \n" << std::endl;
    std::cout << "Original input: " << input << "\n"<< std::endl;
    for (int i = 0; i < tape.size(); ++i) {
        char symbol = tape[i];
        std::cout << "| ";
        if(symbol == 'A') {
            std::cout << "\033[1;31m";
        } else if (symbol == 'B') {
            std::cout << "\033[1;34m";
        } else if (symbol == '1') {
            std::cout << "\033[1;35m";
        } else if (symbol == '-') {
            std::cout << "\033[1;30m";
        } else if (symbol == 'Y') {
            std::cout << "\033[1;33m";
        } else if (symbol == 'X') {
            std::cout << "\033[1;32m";
        }
        std::cout << symbol << "\033[0;37m" << " ";
    }
    std::cout << "|\n";

    for (size_t i = 0; i < headPosition * 4 + 2; ++i) {
        std::cout << " ";
    }
    std::cout << "^\n";
    std::cout << "Current State: " << currentState << std::endl;
}

std::string TuringMachine::findTransitionRule() {
    int stateIndex = currentState[1] - '0';
    if(stateIndex > transitionTable.size()) {
        return "NONE";
    }
    std::unordered_map<char, std::string> transitionRow = transitionTable[stateIndex];
    char currentSymbol = tape[headPosition];
    if(transitionRow.find(currentSymbol) == transitionRow.end()) {
        return "NONE";
    }
    return transitionRow[currentSymbol];
}

int TuringMachine::updateTape(std::string& currentTransitionRule) {
    if(!currentTransitionRule.compare("NONE")) {
        return -1; //rejecting status
    }
    //parse transition rules
    std::vector<std::string> transitionKey;
    std::stringstream ss(currentTransitionRule);
    while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        transitionKey.push_back(substr);
    }

    currentState = transitionKey[0];
    tape[headPosition] = transitionKey[1][0];
    if(transitionKey[2][0] == 'A') {
        return 0; //accepting status
    } else if(transitionKey[2][0] == 'R') {
        if(headPosition == tape.size() - 1) {
            tape.push_back('-');
        }
        headPosition++;
    } else if (transitionKey[2][0] == 'L') {
        if(headPosition == 0) {
            tape.push_front('-');
        } else {
            headPosition--;
        }
    }


    return 1;
}


// Function to simulate the Turing machine
void TuringMachine::run() {

    // For demonstration purposes, let's move the tape head to the right
    while (true) {
        std::string currentTransitionRule = findTransitionRule();
        int status = updateTape(currentTransitionRule);
        printTape();
        if(status == -1) {
            std::cout << "The turing machine rejected the input." << std::endl;
            break;
        } else if (status == 0) {
            std::cout << "The turing machine accepted the input." << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Pause for 500 milliseconds
    }
}

int main() {
    // Example problem input

    std::cout << " " <<std::endl;
    std::cout << "This is a Turing Machine simulator." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Please choose a machine to simulate:\n" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "1. TM that accepts string of 0n 1n" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "2. TM that reverses an input string over alphabet {A,B}" << std::endl;
    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    int selection;
    while(true) {
        std::cout << "Please enter 1 or 2:" << std::endl;
        std::cin >> selection;
        if(selection != 1 && selection != 2) {
            std::cout << "invalid choice, try again to enter 1 or 2:" << std::endl;
        } else {
            break;
        }
    }

    std::cout << "Please enter a string to be processed: " << std::endl;
    std::cin >> input;

    // Create a Turing machine with the SAT problem input
    TuringMachine tm(input);

    //load transition rules into the TM
    if(selection == 1) {
        tm.loadZeroOneTransitions();
    } else if (selection == 2) {
        tm.loadReverseTransitions();
    }


    // Run the Turing machine and animate the process
    tm.run();

    return 0;
}
