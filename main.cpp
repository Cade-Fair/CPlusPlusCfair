#include <iostream>
#include <string>

// A. Class specification
class Player {
private:
    std:: string name;
    int score;

public:
    Player(const std:: string&n, int startScore) : name(n), score(startScore) {}

    // getter returns const reference to avoid copying string and const prevents callers from modifying internal name.
    const std:: string& getName() const {
        return name;
    }

    // Getter returns by value because int is small to copy and callers don't need a reference to iternal storage.
    int getScore() const {
        return score;
    }

    bool hasWon(int target) const {
        return score >= target;
    }

    // amount is pass by value because we only need a copy of the integer.
    void addToScore(int amount) {
        score += amount;
    }
};

// B. Required free functions

// Pass by reference so changes here affect the actual player.
void addMove(Player& who, int step) {
    if (step != 1 && step != 2 && step != 3) {
        std::cout << "Invali move. Please choose 1, 2, or 3. \n";
        return;
    }
    who.addToScore(step);
    std::cout << who.getName() << " new score: " << who.getScore() << "\n";
}

// Pointer to primitive bonusPtr pBonus points to an int main.
// *pBonus means the integer value stored at the memory address pBonus.
void useBonus(int* pBonus, Player& who) {
    if (pBonus == nullptr) {
        return; //Nothing to do if no pointer.
    }
    if (*pBonus <=0) {
        std::cout << "Bonus already used or unavaliable.\n";
        return;
    }
    who.addToScore(*pBonus);
    std::cout << who.getName() << " used bonus +" << *pBonus
    << "! New score: " << who.getScore() << "\n";
    *pBonus = 0; // Mark bonus as used
}

// Deterministic computer helper
// Always returns 1
int compStep() {
    return 1;
}

// C. Game loop requirement

int main() {
    const int TARGET = 20;

    Player you("You", 0);
    Player computer("Computer", 0);

    int bonus = 2;

    bool quit = false;
    bool gameOver = false;

    std::cout << "Welcome to Race to 20!\n";

    while (!quit && !gameOver) {
        std::cout << "\n-- Your Turn ---\n";
        std::cout << "Your Score: " << you.getScore() << " Computer: " << computer.getScore() << "\n";
        std::cout << "Menu:\n";
        std::cout << " 1) Add 1\n";
        std::cout << " 2) Add 2\n";
        std::cout << " 3) Add 3\n";
        std::cout << " 4) Use one-time bonus (+2)\n";
        std::cout << " 5) Quit\n";
        std::cout << "Choose: ";

        int choice;
        if (!(std::cin >> choice)) {
            // Handle non-numeric input safely
            std::cin.clear(); // clear error flags
            std::cin.ignore(1000000, '\n'); // discard up to 1,000,000 chars
            std::cout << "Invalid input. Please enter a number 1-5. \n";
            continue;
        }

        if (choice == 5) {
            quit = true;
            break;
        }

        switch (choice) {
            case 1:
            case 2:
            case 3:
                addMove(you, choice);
                break;
            case 4:
                useBonus(&bonus, you);
                break;
            default:
                std::cout << "Invalid menu choice. Please enter 1-5. \n";
                continue;
        }

        // End condition: If person reaches 20, stop immediately
        if (you.hasWon(TARGET)) {
            std::cout << "\nYou reached " << you.getScore() << "! Game over.\n";
            gameOver = true;
            break;
        }

        //Computer's turn
        std::cout << "\n   Computer's Turn   \n";
        int step = compStep();
        addMove(computer, step);

        // Round end check
        bool youWon = you.hasWon(TARGET);
        bool compWon = computer.hasWon(TARGET);

        if (youWon && compWon) {
            std::cout << "\nBoth reached at least " << TARGET << " in the same round. Therefore it is a tie.\n";
            gameOver = true;
        } else if (compWon) {
            std::cout << "\nComputer reached " << computer.getScore() << ". Computer wins!\n";
            gameOver = true;
        }

    }

    // Final Summary
    std::cout << "\nFinal scores:\n";
    std::cout << "You: " << you.getScore() << " Computer: " << computer.getScore() << "\n";
    if (quit) {
        std::cout << "You chose to quit. Thanks for playing!\n";
    } else if (!you.hasWon(TARGET) && !computer.hasWon(TARGET)) {
        std::cout << "Game ended without a winner.\n";
    } else if (you.hasWon(TARGET) && computer.hasWon(TARGET)) {
        std::cout << "Result: tie.\n";
    } else if (you.hasWon(TARGET)) {
        std::cout << "Result: You Win!\n";
    } else if (computer.hasWon(TARGET)) {
        std::cout << "Result: Computer wins.\n";
    }

    return 0;
}