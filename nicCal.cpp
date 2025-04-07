#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <utility>   // for std::pair
#include <cstdlib>   // for std::atoi
#include <cctype>    // for std::isdigit

using namespace std;

// Enable ANSI colors on Windows
#ifdef _WIN32
#include <windows.h>
void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004
    SetConsoleMode(hOut, dwMode);
}
#else
void enableVirtualTerminalProcessing() {}
#endif

#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define RESET   "\033[0m"
#define BOLD    "\033[1m"

int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

pair<int, int> getMonthAndDay(int dayOfYear, int year) {
    if (isLeapYear(year)) daysInMonth[1] = 29;
    else daysInMonth[1] = 28;

    int month = 0;
    while (dayOfYear > daysInMonth[month]) {
        dayOfYear -= daysInMonth[month];
        month++;
    }
    return make_pair(month + 1, dayOfYear);
}

int getCurrentYear() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return 1900 + ltm->tm_year;
}

void drawLine() {
    cout << CYAN << "+------------------------------------------+" << RESET << endl;
}

void parseNIC(const string& nic) {
    int year, dayOfYear;
    string gender;

    if (nic.length() == 12) {
        year = atoi(nic.substr(0, 4).c_str());
        dayOfYear = atoi(nic.substr(4, 3).c_str());
    } else if (nic.length() == 10) {
        year = 1900 + atoi(nic.substr(0, 2).c_str());
        dayOfYear = atoi(nic.substr(2, 3).c_str());
    } else {
        cout << YELLOW << " Invalid NIC format. Please enter a valid NIC.\n" << RESET;
        return;
    }

    if (dayOfYear >= 500) {
        gender = "Female";
        dayOfYear -= 500;
    } else {
        gender = "Male";
    }

    pair<int, int> birth = getMonthAndDay(dayOfYear, year);
    int birthMonth = birth.first;
    int birthDay = birth.second;
    int age = getCurrentYear() - year;

    drawLine();
    cout << CYAN << "|" << RESET << " " << BOLD << " Sri Lankan NIC Information Extractor" << RESET;
    cout << setw(13) << CYAN << "|" << RESET << endl;
    drawLine();
    cout << CYAN << "|" << RESET << " " << MAGENTA << "Gender      : " << RESET << gender << setw(22 - gender.length()) << CYAN << "|" << RESET << endl;
    cout << CYAN << "|" << RESET << " " << MAGENTA << "Birth Year  : " << RESET << year << setw(19) << CYAN << "|" << RESET << endl;
    cout << CYAN << "|" << RESET << " " << MAGENTA << "Birth Month : " << RESET << birthMonth << setw(19) << CYAN << "|" << RESET << endl;
    cout << CYAN << "|" << RESET << " " << MAGENTA << "Birth Day   : " << RESET << birthDay << setw(20) << CYAN << "|" << RESET << endl;
    cout << CYAN << "|" << RESET << " " << MAGENTA << "Age         : " << RESET << age << setw(20) << CYAN << "|" << RESET << endl;
    drawLine();
}

bool isValidNICFormat(const string& nic) {
    if (nic.length() == 10) {
        for (int i = 0; i < 9; ++i) {
            if (!isdigit(nic[i])) return false;
        }
        char last = nic[9];
        return last == 'V' || last == 'v' || last == 'X' || last == 'x';
    } else if (nic.length() == 12) {
        for (int i = 0; i < 12; ++i) {
            if (!isdigit(nic[i])) return false;
        }
        return true;
    }
    return false;
}

int main() {
    enableVirtualTerminalProcessing(); // For color on Windows
    string nic;

    while (true) {
        cout << BOLD << GREEN << "Enter NIC number (new or old format): " << RESET;
        cin >> nic;

        if (isValidNICFormat(nic)) {
            parseNIC(nic);
            break;
        } else {
            cout << YELLOW << "Invalid NIC format. Please try again.\n" << RESET;
        }
    }

    return 0;
}

