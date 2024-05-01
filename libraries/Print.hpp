#pragma once

// include
#include <iostream>
#include <sstream>
#include <vector>

// using namespace
using namespace std;

enum Color
{
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

vector<string> buffer;
const std::string RESET = "\033[0m";
const std::string COLORS[] = {
    "\033[31m", // RED
    "\033[32m", // GREEN
    "\033[33m", // YELLOW
    "\033[34m", // BLUE
    "\033[35m", // MAGENTA
    "\033[36m", // CYAN
    "\033[37m"  // WHITE
};

template <typename T>
string convertString(const T &variable)
{
    ostringstream oss;
    oss << variable;
    return oss.str();
}

template <class T>
void printArgs(const T &arg)
{
    buffer.push_back(convertString(arg));
    buffer.push_back("\n");
    cout << "2" << endl;
}

template <class T, class... Args>
void printArgs(const T &arg, const Args &...args)
{
    buffer.push_back(convertString(arg));
    buffer.push_back(" ");
    printArgs(args...);
    cout << "2" << endl;
}

void printlnColor(int color = WHITE)
{
    cout << COLORS[color];
    for (const string &str : buffer)
    {
        cout << str;
    }
    cout << RESET;
    buffer.clear();
}
