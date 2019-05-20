#ifndef CAGE_H
#define CAGE_H
#include <iostream>

class Cage
{
public:
    Cage();
    bool opened = false;
    std::string text = "0";
    int countOfMines=0;
    bool mine = false;
    bool mineFlag = false;
};

#endif // CAGE_H
