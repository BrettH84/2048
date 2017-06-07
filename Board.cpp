#include "stdafx.h"
#include"Board.h"
#include <stdlib.h>
#include <time.h>

/*
*
*   TILE CLASS IMPLEMENTATION
*
*/
Tile::Tile()
{
    itsValue = GenerateRandomValue();
    wasMerged = false;
}
Tile::Tile(unsigned long int value, bool merged)
{
    itsValue = value;
    wasMerged = merged;
}
Tile::Tile(const Tile &tile)
{
    itsValue = tile.GetValue();
    wasMerged = tile.wasMerged;
}
Tile::~Tile()
{

}
void Tile::Draw()
{

}
Tile Tile::operator+(Tile rhs)const
{
    Tile temp(itsValue + rhs.GetValue(), true);
    return temp;
}
unsigned long int Tile::GetValue()const 
{
    return itsValue;
}
void Tile::SetValue(unsigned long int newValue)
{
    itsValue = newValue;
}
bool Tile::operator==(Tile rhs)const
{
    if (itsValue == rhs.GetValue())
        return true;
    return false;    
}
unsigned long int Tile::GenerateRandomValue()
{
    srand(static_cast<unsigned int>(time(NULL)));
    return (rand() % 2 + 1) * 2;  //returns either 2 or 4, as per the game design.
}





/*
*
*   TILE CLASS IMPLEMENTATION
*
*/
Board::Board() :itsWidth(DEFAULT_DIMENSION), itsHeight(DEFAULT_DIMENSION), itsTiles(DEFAULT_DIMENSION, vector<Tile>(DEFAULT_DIMENSION, Tile(0)))
{
    itsTiles;
    for (int i = 0; i < DEFAULT_NUM_STARTING_TILES; i++)
    {
        GenerateTile();
    }    
    Draw();
}
// Add safeguards against the height/width being invalid values.
Board::Board(unsigned int height, unsigned int width) :itsWidth(width), itsHeight(height), itsTiles(width, vector<Tile>(height, Tile(0)))
{
    for (int i = 0; i < DEFAULT_NUM_STARTING_TILES; i++)
    {
        GenerateTile();
    }
    Draw();
}
// Add safeguards against tiles or its elements being an empty vector.
Board::Board(vector<vector<Tile>> tiles)
{
    itsWidth = tiles.size();
    itsHeight = tiles[0].size();
    itsTiles = tiles;
    Draw();
}
Board::Board(const Board &board)
{
    itsHeight = board.GetHeight();
    itsWidth = board.GetWidth();
    itsTiles = board.GetTiles();
    Draw();
}
Board::~Board()
{

}
void Board::Draw()
{

}
int Board::GetHeight() const
{
    return itsHeight;
}
int Board::GetWidth() const
{
    return itsWidth;
}
// Throw an exception in the default case?
vector<vector<Tile>> Board::GetTiles()const
{
    return itsTiles;
}
void Board::Move(MOVE move)
{   
    if (!MoveIsValid(move))
        return;
    
    switch (move)
    {
    case UP:
        MoveUp();
        break;
    case DOWN:
        MoveDown();
        break;
    case LEFT:
        MoveLeft();
        break;
    case RIGHT:
        MoveRight();
        break;
    default:
        cout << "Add error handling here" << endl;
    }

    GenerateTile();
    Draw();
        
}
// Throw an exception in the default case?
bool Board::MoveIsValid(MOVE move)const
{

    // Determine where each tile should look in itsTiles to compare for an eligible move.
    int columnOffset = 0;
    int rowOffset = 0;
    switch (move)
    {
        case LEFT:
            columnOffset--;
            break;
        case RIGHT:
            columnOffset++;
            break;
        case UP:
            rowOffset++;
            break;
        case DOWN:
            rowOffset--;
            break;
        default:
            cerr << "Add some error handling" << endl;
    }

    for (unsigned int i_row = 0; i_row < itsHeight; i_row++)
    {
        if (i_row + rowOffset < 0 || i_row + rowOffset >= itsHeight)
            continue;
        for (unsigned int j_column = 0; j_column <= itsWidth; j_column++)
        {
            if (j_column + columnOffset < 0 || j_column + columnOffset >= itsWidth)
                continue;
            if (itsTiles[j_column + columnOffset][i_row + rowOffset].GetValue() == 0 ||             // If the adjacent tile is empty
                itsTiles[j_column][i_row] == itsTiles[j_column + columnOffset][i_row + rowOffset])  // or if it matches the current tile return true.
                return true;
        }
    }
    return false;
}
void Board::PrintToStream(ostream& output)const
{
    //system("CLS");
    for (int j_row = (int)itsHeight - 1; j_row >= 0; j_row--)
    {
        for (int i_column = 0; i_column < (int)itsWidth; i_column++)
        {
            output << itsTiles[i_column][j_row].GetValue();
        }
        output << endl;
    }
    output << endl;
}
bool Board::GenerateTile()
{
    vector<pair<unsigned int, unsigned int>> openSlots;
    for (unsigned int i_column = 0; i_column < itsWidth; i_column++)
    {
        for (unsigned int j_row = 0; j_row < itsHeight; j_row++)
        {
            if (itsTiles[i_column][j_row].GetValue() == 0)
                openSlots.push_back(pair<unsigned int, unsigned int>(i_column, j_row));
        }
    }

    if (openSlots.empty())
    {
        return false;
    }

    unsigned int openSlotsIndex = rand() % openSlots.size();
    itsTiles[openSlots[openSlotsIndex].first][openSlots[openSlotsIndex].second] = Tile();
    return true;
}
long int Board::MoveRight()
{
    long int score = 0;

    for (int i_column = (int)itsWidth - 2; i_column >= 0; i_column--)
    {
        for (int j_row = 0; j_row < (int)itsHeight; j_row++)
        {
            if (itsTiles[i_column][j_row].GetValue() > 0)
            {
                // Test to see whether a tile can be merged or if it can just be moved.
                int lastOpenColumn = i_column;
                bool canBeMerged = false;
                for (int k_column = i_column + 1; k_column < (int)itsWidth; k_column++)
                {
                    if (itsTiles[k_column][j_row].GetValue() == 0)
                    {
                        lastOpenColumn = k_column;
                    }
                    else
                    {
                        if ((itsTiles[i_column][j_row] == itsTiles[k_column][j_row]) && (itsTiles[k_column][j_row].wasMerged == false))
                            canBeMerged = true;
                        break;
                    }
                }

                if (canBeMerged)
                {
                    itsTiles[lastOpenColumn + 1][j_row] = itsTiles[i_column][j_row] + itsTiles[lastOpenColumn + 1][j_row];
                    itsTiles[lastOpenColumn + 1][j_row].wasMerged = true;
                    itsTiles[i_column][j_row] = Tile(0);
                    score += itsTiles[lastOpenColumn + 1][j_row].GetValue();
                }
                else if (lastOpenColumn != i_column)
                {
                    itsTiles[lastOpenColumn][j_row] = itsTiles[i_column][j_row];
                    itsTiles[i_column][j_row] = Tile(0);
                }
            }
        }
    }

    SetAllMergedFlags();

    return score;
}
long int Board::MoveLeft()
{
    long int score = 0;

    for (int i_column = 1; i_column < (int)itsWidth; i_column++)
    {
        for (int j_row = 0; j_row < (int)itsHeight; j_row++)
        {
            if (itsTiles[i_column][j_row].GetValue() > 0)
            {
                // Test to see whether a tile can be merged or if it can just be moved.
                int lastOpenColumn = i_column;
                bool canBeMerged = false;
                for (int k_column = i_column - 1; k_column >= 0; k_column--)
                {
                    if (itsTiles[k_column][j_row].GetValue() == 0)
                    {
                        lastOpenColumn = k_column;
                    }
                    else
                    {
                        if ((itsTiles[i_column][j_row] == itsTiles[k_column][j_row]) && (itsTiles[k_column][j_row].wasMerged == false))
                            canBeMerged = true;
                        break;
                    }
                }

                if (canBeMerged)
                {
                    itsTiles[lastOpenColumn - 1][j_row] = itsTiles[i_column][j_row] + itsTiles[lastOpenColumn - 1][j_row];
                    itsTiles[lastOpenColumn - 1][j_row].wasMerged = true;
                    itsTiles[i_column][j_row] = Tile(0);
                    score += itsTiles[lastOpenColumn - 1][j_row].GetValue();
                }
                else if (lastOpenColumn != i_column)
                {
                    itsTiles[lastOpenColumn][j_row] = itsTiles[i_column][j_row];
                    itsTiles[i_column][j_row] = Tile(0);
                }
            }
        }
    }

    SetAllMergedFlags();

    return score;
}
long int Board::MoveUp()
{
    long int score = 0;

    for (int j_row = itsHeight - 2; j_row >= 0; j_row--)
    {
        for (int i_column = 0; i_column < (int)itsWidth; i_column++)
        {
            if (itsTiles[i_column][j_row].GetValue() > 0)
            {
                // Test to see whether a tile can be merged or if it can just be moved.
                int lastOpenRow = j_row;
                bool canBeMerged = false;
                for (int k_row = j_row + 1; k_row < (int)itsHeight; k_row++)
                {
                    if (itsTiles[i_column][k_row].GetValue() == 0)
                    {
                        lastOpenRow = k_row;
                    }
                    else
                    {
                        if ((itsTiles[i_column][j_row] == itsTiles[i_column][k_row]) && (itsTiles[i_column][k_row].wasMerged == false))
                            canBeMerged = true;
                        break;
                    }
                }

                if (canBeMerged)
                {
                    itsTiles[i_column][lastOpenRow + 1] = itsTiles[i_column][j_row] + itsTiles[i_column][lastOpenRow + 1];
                    itsTiles[i_column][lastOpenRow + 1].wasMerged = true;
                    itsTiles[i_column][j_row] = Tile(0);
                    score += itsTiles[i_column][lastOpenRow + 1].GetValue();
                }
                else if (lastOpenRow != j_row)
                {
                    itsTiles[i_column][lastOpenRow] = itsTiles[i_column][j_row];
                    itsTiles[i_column][j_row] = Tile(0);
                }
            }
        }
    }

    SetAllMergedFlags();

    return score;
}
long int Board::MoveDown()
{
    long int score = 0;

    for (int j_row = 1; j_row < (int)itsHeight; j_row++)
    {
        for (int i_column = 0; i_column < (int)itsWidth; i_column++)
        {
            if (itsTiles[i_column][j_row].GetValue() > 0)
            {
                // Test to see whether a tile can be merged or if it can just be moved.
                int lastOpenRow = j_row;
                bool canBeMerged = false;
                for (int k_row = j_row - 1; j_row >= 0; k_row--)
                {
                    if (itsTiles[i_column][k_row].GetValue() == 0)
                    {
                        lastOpenRow = k_row;
                    }
                    else
                    {
                        if ((itsTiles[i_column][j_row] == itsTiles[i_column][k_row]) && (itsTiles[i_column][k_row].wasMerged == false))
                            canBeMerged = true;
                        break;
                    }
                }

                if (canBeMerged)
                {
                    itsTiles[i_column][lastOpenRow - 1] = itsTiles[i_column][j_row] + itsTiles[i_column][lastOpenRow - 1];
                    itsTiles[i_column][lastOpenRow - 1].wasMerged = true;
                    itsTiles[i_column][j_row] = Tile(0);
                    score += itsTiles[i_column][lastOpenRow - 1].GetValue();
                }
                else if (lastOpenRow != j_row)
                {
                    itsTiles[i_column][lastOpenRow] = itsTiles[i_column][j_row];
                    itsTiles[i_column][j_row] = Tile(0);
                }
            }
        }
    }

    SetAllMergedFlags();

    return score;
}
void Board::SetAllMergedFlags(bool flag)
{
    for (unsigned int i_column = 0; i_column < itsWidth; i_column++)
    {
        for (unsigned int j_row = 0; j_row < itsHeight; j_row++)
        {
            itsTiles[i_column][j_row].wasMerged = flag;
        }
    }
}