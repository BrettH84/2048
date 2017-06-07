#pragma once

#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>

using namespace std;


enum MOVE{ LEFT, RIGHT, UP, DOWN };





class Tile
{
    friend class Board;

public:
    Tile();
    Tile(unsigned long int, bool merged = false);
    Tile(const Tile &tile);
    ~Tile();

    void Draw();
    Tile operator+(Tile rhs)const;
    unsigned long int GetValue() const;
    void SetValue(unsigned long int newValue);
    bool operator==(Tile rhs) const;

private:
    unsigned long int itsValue;
    unsigned long int GenerateRandomValue();
    bool wasMerged;
};




class Board
{
public:
    Board();
    Board(unsigned int height, unsigned int width);
    Board(vector<vector<Tile>> tiles);
    Board(const Board &board);
    ~Board();

    void Draw();
    int GetHeight() const;
    vector<vector<Tile>> GetTiles()const;
    int GetWidth() const;
    void Move(MOVE move);
    bool MoveIsValid(MOVE move)const;
    void PrintToStream(ostream& output) const;
    int SetHeight();
    int SetWidth();


protected:

private:
    bool GenerateTile();
    long int MoveRight();
    long int MoveLeft();
    long int MoveUp();
    long int MoveDown();
    void SetAllMergedFlags(bool flag = false);





private:
    vector<vector<Tile>> itsTiles;  //itsTiles[column][row]
    unsigned int itsWidth;
    unsigned int itsHeight;
    static const unsigned int DEFAULT_DIMENSION = 4;
    static const unsigned int DEFAULT_NUM_STARTING_TILES = 2;
};


#endif __BOARD_H__