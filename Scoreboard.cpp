#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

string tossResult(int toss) {
    return (toss == 0) ? "Heads" : "Tails";
}

string chooseBatOrBowl() {
    string choice;
    while (true) {
        cout << "Choose to Bat or Bowl first (bat/bowl): ";
        cin >> choice;
        for (int i = 0; i < (int)choice.size(); i++)
            choice[i] = tolower(choice[i]);
        if (choice == "bat" || choice == "bowl") break;
        cout << "Invalid input, please enter 'bat' or 'bowl'.\n";
    }
    return choice;
}

string randomBatOrBowl() {
    return (rand() % 2 == 0) ? "bat" : "bowl";
}

void showScoreboard(const string &teamName, int runs, int wickets, int balls, int extras, int target = -1) {
    int total = runs + extras;
    cout << "\n--- Scoreboard ---\n";
    cout << teamName << ": " << total << "/" << wickets 
         << " in " << balls / 6 << "." << balls % 6 << " overs\n";
    cout << "(Runs: " << runs << " + Extras: " << extras << ")\n";
    if (target != -1)
        cout << "Target: " << target + 1 << "\n";
    cout << "------------------\n";
}

struct InningsResult {
    int totalRuns;
    int wicketsLost;
};

InningsResult playInnings(const string &battingTeam, const string &bowlingTeam, int overs, int target = -1) {
    int runs = 0, wickets = 0, balls = 0, extras = 0;

    cout << "\n" << battingTeam << " batting now.\n";

    while (balls < overs * 6 && wickets < 10) {
        cout << "\nChoose an option:\n";
        cout << "1. Add Runs (3-6 runs)\n";
        cout << "2. Single (1 run)\n";
        cout << "3. Double (2 runs)\n";
        cout << "4. Extra\n";
        cout << "5. No Ball\n";
        cout << "6. Wicket\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                int run;
                cout << "Enter runs (3-6): ";
                cin >> run;
                if (run >= 3 && run <= 6) {
                    runs += run;
                    balls++;
                } else {
                    cout << "Invalid input.\n";
                }
                break;
            }
            case 2:
                runs += 1;
                balls++;
                break;
            case 3:
                runs += 2;
                balls++;
                break;
            case 4: {
                int ex;
                cout << "Enter extra runs: ";
                cin >> ex;
                if (ex > 0) extras += ex;
                else cout << "Invalid extra runs.\n";
                break;
            }
            case 5: {
                int nb;
                cout << "No Ball! Enter runs scored (excluding the free run): ";
                cin >> nb;
                if (nb >= 0 && nb <= 6) {
                    runs += 1 + nb;
                } else {
                    cout << "Invalid input.\n";
                }
                break;
            }
            case 6:
                wickets++;
                balls++;
                cout << "Wicket fallen!\n";
                break;
            default:
                cout << "Invalid choice.\n";
                continue;
        }

#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        showScoreboard(battingTeam, runs, wickets, balls, extras, target);

        if (target != -1 && (runs + extras) > target) {
            cout << battingTeam << " has surpassed the target!\n";
            break;
        }
    }

    cout << "\nInnings finished for " << battingTeam << ".\n";
    showScoreboard(battingTeam, runs, wickets, balls, extras, target);

    return InningsResult{runs + extras, wickets};
}

int main() {
    srand(time(0));

    string userTeam, opponentTeam;
    cout << "Enter your team name: ";
    getline(cin, userTeam);
    cout << "Enter opponent team name: ";
    getline(cin, opponentTeam);

    int overs;
    cout << "Enter number of overs: ";
    cin >> overs;
    cin.ignore();

    cout << "\n--- Toss Time! ---\n";
    string userCall;
    while (true) {
        cout << "Call the toss (heads/tails): ";
        cin >> userCall;
        for (int i = 0; i < (int)userCall.size(); i++)
            userCall[i] = tolower(userCall[i]);
        if (userCall == "heads" || userCall == "tails") break;
        cout << "Invalid input.\n";
    }

    int toss = rand() % 2;
    cout << "Toss result: " << tossResult(toss) << "\n";
    bool userWins = (userCall == tossResult(toss));

    string userDecision, opponentDecision;
    if (userWins) {
        cout << "You won the toss!\n";
        userDecision = chooseBatOrBowl();
        opponentDecision = (userDecision == "bat") ? "bowl" : "bat";
    } else {
        cout << "You lost the toss.\n";
        opponentDecision = randomBatOrBowl();
        userDecision = (opponentDecision == "bat") ? "bowl" : "bat";
        cout << opponentTeam << " chose to " << opponentDecision << " first.\n";
    }

    InningsResult userInnings, opponentInnings;

    if (userDecision == "bat") {
        cout << "\n-- First Innings --\n";
        userInnings = playInnings(userTeam, opponentTeam, overs);

        cout << "\n-- Second Innings --\n";
        opponentInnings = playInnings(opponentTeam, userTeam, overs, userInnings.totalRuns);
    } else {
        cout << "\n-- First Innings --\n";
        opponentInnings = playInnings(opponentTeam, userTeam, overs);

        cout << "\n-- Second Innings --\n";
        userInnings = playInnings(userTeam, opponentTeam, overs, opponentInnings.totalRuns);
    }

    cout << "\n--- Match Result ---\n";

    if (userInnings.totalRuns > opponentInnings.totalRuns) {
        cout << "Congratulations! " << userTeam << " won the match by "
             << (userInnings.totalRuns - opponentInnings.totalRuns) << " runs!\n";
    } else if (userInnings.totalRuns < opponentInnings.totalRuns) {
        cout << opponentTeam << " won the match by "
             << (10 - opponentInnings.wicketsLost) << " wickets. Better luck next time!\n";
    } else {
        cout << "The match ended in a tie!\n";
    }

    return 0;
}

