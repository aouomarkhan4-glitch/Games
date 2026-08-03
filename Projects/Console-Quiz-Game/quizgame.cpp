#include<iostream>    // cin, cout,
#include<fstream>     // ifstream,ofstream,
#include<string>      // getline
#include<cstdlib>     // rand()%
#include<ctime>       // srand(time(0)), time_t
#include<iomanip>     // setw()
#include<conio.h>     // Required for _kbhit() and _getch()

using namespace std;

// ==========================================
//           HELPER FUNCTIONS
// ==========================================

void valid(ifstream& file)
{
    if (!file.is_open()) cout << "error : menu file does not open" << endl;
}

void space()
{
    cout << "==================================" << endl;
}

// ==========================================
//           TIMER - INPUT FUNCTION
// ==========================================
string getTimedInput(int timeLimit, bool allowLifeline)
{
    time_t startTime = time(0);

    // Clear keyboard buffer
    while (_kbhit()) _getch();

    while (true)
    {
        time_t currentTime = time(0);
        int timeElapsed = difftime(currentTime, startTime);
        int timeRemaining = timeLimit - timeElapsed;

        // Check if Time is Up
        if (timeRemaining <= 0)
        {
            cout << endl << "Time's up!" << endl;
            return "TIMEOUT";
        }

        // Display Timer
        cout << "\rEnter (a/b/c/d)";
        if (allowLifeline) cout << " or 'L' for Lifeline";
        cout << " [ Time: " << setw(3) << timeRemaining << " s ] ----- ";

        // Check if a key is pressed
        if (_kbhit())
        {
            char ch = _getch();

            // Check for Lifeline Trigger
            if (allowLifeline && (ch == 'L' || ch == 'l')) {
                return "LIFELINE";
            }

            cout << ch << endl;
            string s(1, ch);
            return s;
        }
       
    }
}

// ==========================================
//           CORE GAME ENGINE
// ==========================================
void playGame(string currentFileName, string playerName, string levelName)
{
    ofstream fout("assets/output.txt", ios::app);
    if (!fout.is_open()) cout << "error : output file does not open " << endl;

    ofstream logFile("assets/quiz_log.txt", ios::app);
    if (!logFile.is_open()) cout << "error : quiz_log file does not open " << endl;

    logFile << "===========================================" << endl;
    logFile << "PLAYER: " << playerName << " | LEVEL: " << levelName << endl;
    logFile << "===========================================" << endl;

    string str8;
    string ans8;
    int score = 0;

    int consecutiveCorrect = 0;
    int penalty = 0;
    bool lifelineUsed = false;

    if (levelName.find("Easy") != string::npos) penalty = 2;
    else if (levelName.find("Medium") != string::npos) penalty = 3;
    else if (levelName.find("Hard") != string::npos) penalty = 5;

    // === LOOP FOR 10 QUESTIONS ===
    for (int q = 1; q <= 10; ++q)
    {
        ifstream fin(currentFileName);
        if (!fin.is_open()) { cout << " error : file error " << endl; return; }

        int n = rand() % 50;
        int arr[50] = { 1,7,13,19,25,31,37, 43, 49, 55 ,61, 67, 73, 79, 85, 91, 97, 103, 109, 115, 121, 127, 133, 139, 145, 151, 157, 163, 169, 175, 181, 187, 193, 199, 205, 211, 217, 223, 229, 235, 241, 247, 253, 259, 265, 271, 277, 283, 289, 295 };
        int h = arr[n];

        logFile << "\nQuestion " << q << ":" << endl;

        // Control flags for breaking out of nested loops 
        bool shouldSkip = false;
        bool shouldReplace = false;

        // Loop to find random line
        for (int x = 1; x <= h; ++x)
        {
            if (x < h)
            {
                getline(fin, str8);
            }
            else
            {
                space();
                cout << "Question " << q << "/10" << endl;

                // Read and Print Options
                for (int d = -6; d < -1; )
                {
                    if (d < -1)
                    {
                        getline(fin, str8);
                        cout << str8 << endl;
                        logFile << str8 << endl;
                    }
                    ++d;

                    if (d == -1)
                    {
                        getline(fin, str8); // Correct answer
                        string correctAns = str8;

                        space();

                        // ==== INPUT LOGIC WITH DO-WHILE (REPLACES GOTO) ====
                        int currentTimeLimit = 15;
                        bool reaskInput = true; // Controls the loop

                        do {
                            ans8 = getTimedInput(currentTimeLimit, !lifelineUsed);

                            if (ans8 == "LIFELINE") {
                                cout << "\n\n=== LIFELINE MENU ===" << endl;
                                cout << "1. 50/50" << endl;
                                cout << "2. Skip Question" << endl;
                                cout << "3. Replace Question" << endl;
                                cout << "4. Add Time (+10s)" << endl;
                                cout << "Enter Choice (1-4): ";
                                char lifeChoice;
                                cin >> lifeChoice;
                                lifelineUsed = true;

                                if (lifeChoice == '1') { // 50/50
                                    cout << "\n[LIFELINE USED] 50/50 applied..." << endl;
                                    if (correctAns == "a") cout << ">> Hint: B and C are WRONG." << endl;
                                    else if (correctAns == "b") cout << ">> Hint: A and D are WRONG." << endl;
                                    else if (correctAns == "c") cout << ">> Hint: A and B are WRONG." << endl;
                                    else if (correctAns == "d") cout << ">> Hint: B and C are WRONG." << endl;
                                    logFile << "Lifeline Used: 50/50" << endl;
                                    reaskInput = true; // Loop again
                                }
                                else if (lifeChoice == '2') { // Skip
                                    ans8 = "SKIP";
                                    reaskInput = false; // Stop looping, process skip
                                }
                                else if (lifeChoice == '3') { // Replace
                                    ans8 = "REPLACE";
                                    reaskInput = false; // Stop looping, process replace
                                }
                                else if (lifeChoice == '4') { // +10s
                                    cout << "\n[LIFELINE USED] Adding 10 seconds..." << endl;
                                    logFile << "Lifeline Used: +10s" << endl;
                                    currentTimeLimit += 10;
                                    reaskInput = true; // Loop again with new time
                                }
                                else {
                                    cout << "Invalid. Wasted." << endl;
                                    reaskInput = true;
                                }
                            }
                            else {
                                reaskInput = false; // Valid answer received
                            }

                        } while (reaskInput);

                        // ==== PROCESS RESULT ====
                        space();

                        if (ans8 == "SKIP") {
                            cout << "\n[SKIPPING]..." << endl;
                            logFile << "Lifeline Used: Skip" << endl;
                            consecutiveCorrect = 0;
                            shouldSkip = true; // Set flag to break outer loops
                        }
                        else if (ans8 == "REPLACE") {
                            cout << "\n[REPLACING]..." << endl;
                            logFile << "Lifeline Used: Replace" << endl;
                            shouldReplace = true; // Set flag to break outer loops
                        }
                        else if (ans8 == "TIMEOUT") {
                            cout << "Time Out! Correct: " << correctAns << endl;
                            score -= penalty;
                            consecutiveCorrect = 0;
                            cout << "Penalty: -" << penalty << " | Score: " << score << endl;
                            logFile << "Result: TIMEOUT | Penalty: -" << penalty << endl;
                        }
                        else if (ans8 == correctAns) {
                            int pointsAdded = 10;
                            consecutiveCorrect++;
                            if (consecutiveCorrect == 3) { pointsAdded += 5; cout << " [BONUS! 3 Streak] "; }
                            else if (consecutiveCorrect == 5) { pointsAdded += 15; cout << " [BONUS! 5 Streak] "; }
                            score += pointsAdded;
                            cout << " Correct : " << correctAns << " | +" << pointsAdded << " | Score : " << score << endl;
                            logFile << "Result: CORRECT | Added: " << pointsAdded << endl;
                        }
                        else {
                            score -= penalty;
                            consecutiveCorrect = 0;
                            cout << " Wrong : " << ans8 << " | Correct : " << correctAns << endl;
                            cout << "Penalty: -" << penalty << " | Score : " << score << endl;
                            logFile << "Result: WRONG | Penalty: -" << penalty << endl;
                        }
                        space();
                        system("pause");
                        system("cls");
                    }
                }
            }
            // If Skip or Replace was triggered inside, break this loop
            if (shouldSkip || shouldReplace) break;
        }
        fin.close();

        // Handle Loop Control for Replace logic
        if (shouldReplace) {
            q--; // Decrement Q so loop repeats same number
        }
        // If shouldSkip, we just do nothing and the loop continues to q++
    }

    //=====  FINAL RESULT DISPLAY ============ 
    cout << endl; space(); cout << endl;
    if (score >= 100) cout << " welldone    :    " << score << endl;
    else if (score > 50) cout << " nice    :    " << score << endl;
    else cout << " try hard    :    " << score << endl;
    cout << endl; space();

    fout << setw(8) << "name :  " << setw(14) << playerName << " | " << setw(8) << "score : " << setw(5) << score << " | " << setw(10) << levelName << endl;
    fout.close();

    logFile << "-------------------------------------------" << endl;
    logFile << "FINAL SCORE: " << score << endl;
    logFile << "===========================================\n" << endl;
    logFile.close();

    system("pause");
    system("cls");
}

// ==========================================
//           MAIN FUNCTION
// ==========================================
int main()
{
    while (true)
    {
        srand(time(0));
        ifstream menu("assets/menu.txt");
        valid(menu);
        string str;
        int i;
        bool validIn = false;
        char option;
        string name;

        // Display Main Menu
        for (i = 0; i < 7; ++i)
        {
            getline(menu, str);
            cout << str << endl;
            if (i == 6)
            {
                while (!validIn)
                {
                    cout << " ENTER-----(S/L/E)---- ";
                    cin >> option;
                    if (option == 'S' || option == 's' || option == 'e' || option == 'E' || option == 'L' || option == 'l') { validIn = true; system("cls"); }
                    else { cout << "error : wrong input" << endl; }
                }
            }
        }
        menu.close();

        int option2 = 0;
        bool valid2 = false;

        switch (option)
        {
        case 's': case 'S': // START GAME
        {
            cin.ignore(); cout << "Enter your name : "; getline(cin, name); system("cls");
            ifstream menuSub("assets/menu.txt");
            for (int j = 0; j < 16; ++j) { getline(menuSub, str); if (j >= 7) cout << str << endl; }
            menuSub.close();
            while (!valid2) {
                cout << "Enter-------(1/2/3/4/5)----"; cin >> option2;
                if (option2 >= 1 && option2 <= 5) { valid2 = true; system("cls"); }
                else { cout << "error : wrong input" << endl; } cin.clear(); cin.ignore();
            }
            break;
        }
        case 'l': case 'L': // VIEW LOGS
        {
            ifstream fin("assets/output.txt");
            if (!fin.is_open()) cout << "error : output file error" << endl;
            string led; while (getline(fin, led)) cout << led << endl;
            fin.close(); system("pause"); system("cls"); break;
        }
        case 'e': case 'E': { space(); cout << endl; cout << "----------- THANK YOU ------------" << endl; cout << endl; space(); system("pause"); return 0; }
        }

        // LEVEL SELECTION
        if (option == 'S' || option == 's')
        {
            char option3 = 0;
            bool valid3 = false;
            ifstream menu2("assets/menu.txt");
            for (int k = 0; k < 23; ++k) { if (k < 16) getline(menu2, str); else if (k < 23) { getline(menu2, str); cout << str << endl; } }
            menu2.close();
            while (!valid3) {
                cout << "Enter----(E/M/H)----"; cin >> option3;
                if (option3 == 'E' || option3 == 'e' || option3 == 'M' || option3 == 'm' || option3 == 'H' || option3 == 'h') { valid3 = true; system("cls"); }
                else { cout << "error : wrong input" << endl; }
            }
            cin.ignore();

            // NESTED SWITCH FOR GAME LAUNCH
            switch (option2) {
            case 1: // Science
                switch (tolower(option3)) {
                case 'e': playGame("assets/science(e).txt", name, "Science Easy"); break;
                case 'm': playGame("assets/science(m).txt", name, "Science Medium"); break;
                case 'h': playGame("assets/science(h).txt", name, "Science Hard"); break;
                } break;
            case 2: // Computer
                switch (tolower(option3)) {
                case 'e': playGame("assets/computer(e).txt", name, "Computer Easy"); break;
                case 'm': playGame("assets/computer(m).txt", name, "Computer Medium"); break;
                case 'h': playGame("assets/computer(h).txt", name, "Computer Hard"); break;
                } break;
            case 3: // Sport
                switch (tolower(option3)) {
                case 'e': playGame("assets/sport(e).txt", name, "Sport Easy"); break;
                case 'm': playGame("assets/sport(m).txt", name, "Sport Medium"); break;
                case 'h': playGame("assets/sport(h).txt", name, "Sport Hard"); break;
                } break;
            case 4: // History
                switch (tolower(option3)) {
                case 'e': playGame("assets/history(e).txt", name, "History Easy"); break;
                case 'm': playGame("assets/history(m).txt", name, "History Medium"); break;
                case 'h': playGame("assets/history(h).txt", name, "History Hard"); break;
                } break;
            case 5: // IQ
                switch (tolower(option3)) {
                case 'e': playGame("assets/iq(e).txt", name, "IQ Easy"); break;
                case 'm': playGame("assets/iq(m).txt", name, "IQ Medium"); break;
                case 'h': playGame("assets/iq(h).txt", name, "IQ Hard"); break;
                } break;
            }
        }
    }
    return 0;
}