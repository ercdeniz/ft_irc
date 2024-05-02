#pragma once

// include
#include <iostream>
#include <sstream>
#include <sys/socket.h>

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

template <typename T>
void println(const T &variable , int color = WHITE)
{
    cout << COLORS[color] << variable << RESET << endl;
}

template <typename T>
void print(const T &variable , int color = WHITE)
{
    cout << COLORS[color] << variable << RESET;
}

template <typename T>
void printlnErr(const T &variable , int color = WHITE)
{
    cerr << COLORS[color] << variable << RESET << endl;
}

template <typename T>
void printFd(int fd, const T &variable)
{
    send(fd, (convertString(variable) + '\n').c_str(), convertString(variable).length() + 1, 0);
}