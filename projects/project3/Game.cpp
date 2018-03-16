#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    
    
private:
    int nRows;
    int nCols;

    struct Ship
    {
        string nName;
        char nSymbol;
        int nLength;
    };
    
    vector<Ship> gameShips;
    
    void makeMove(Player* &p, Board& b, bool& shouldPause);
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    this->nRows = nRows;
    this->nCols = nCols;
}

int GameImpl::rows() const
{
    return nRows;
}

int GameImpl::cols() const
{
    return nCols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}


// Each player has a number of different ships. In Standard Battleship, each player has five ships:
//a. An aircraft carrier which is 5 segments long: AAAAA
//b. A battleship which is 4 segments long: BBBB
//c. A destroyer which is 3 segments long: DDD
//d. A submarine which is 3 segments long: SSS
//e. A patrol boat which is 2 segments long: PP
bool GameImpl::addShip(int length, char symbol, string name)
{
    if (length <= 0)    //must be positive
        return false;
    
    //can't fit on board ??
    
    if(symbol == 'X' || symbol == 'o' || symbol == '.') //in-game symbols, not allowed
        return false;
    

    Ship temp;              //push this ship, since it can be added, into vector
    temp.nName = name;
    temp.nSymbol = symbol;
    temp.nLength = length;
    gameShips.push_back(temp);
    
    return true;
}

int GameImpl::nShips() const
{
    return (int)gameShips.size();
}

int GameImpl::shipLength(int shipId) const
{
    return gameShips[shipId].nLength;
}

char GameImpl::shipSymbol(int shipId) const
{
    return gameShips[shipId].nSymbol;
}

string GameImpl::shipName(int shipId) const
{
    return gameShips[shipId].nName;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    b1.clear(); //clear the boards
    b2.clear();
    
    if(p1->placeShips(b1) == false || (p2-> placeShips(b2) == false))
        return nullptr; //can't place ships, return nullptr

    while(!b1.allShipsDestroyed() && !b2.allShipsDestroyed())   //while we're still in play
    {
        if(shouldPause)
            waitForEnter();
        cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
        makeMove(p1, b2, shouldPause);//attacks and displays board according to player type for player 2
        if (shouldPause)
            waitForEnter();
        cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
        makeMove(p2, b1, shouldPause); //attacks and displays board according to player type for player 2
    }
    if(b2.allShipsDestroyed())  //all player 2's ships destroyed, player 1 wins
    {
        cout << p1->name() << " wins!" <<endl;
        return p1;
    }
    if(b1.allShipsDestroyed()) //all player 1's ships destroyed, player 2 wins
    {
        cout << p2->name() << " wins!" <<endl;
        return p2;
    }
    
    return nullptr;
}

void GameImpl::makeMove(Player* &p, Board& b, bool &shouldPause) //helper function for play()
{
    bool shotHit;
    bool destroyed;
    int shipId = -1;
    bool shotsOnly = false;
    bool validShot = false;
    
    if(p->isHuman())    //if player is human, display board with only shots, so as not to cheat
    {
        shotsOnly = true;
        b.display(shotsOnly);
    }
    if (!p->isHuman())  //if not, then show boards with everything for the obeserver
        b.display(shotsOnly);
    
    Point attacked = p->recommendAttack();
    b.attack(attacked, shotHit, destroyed, shipId);
    
    validShot = shotHit;    //use as passed param
    
    p->recordAttackResult(attacked, validShot, shotHit, destroyed, shipId);
    
    if(p->isHuman() && validShot == false)
        cout << p->name()<< " wasted a shot at (" << attacked.r << "," << attacked.c << ")." << endl;
    
    else    //cosmetic stuff
    {
        cout << p->name() << " attacked (" << attacked.r << "," << attacked.c << ") and ";
        if (shotHit == false)
            cout << "missed, resulting in:"<< endl;
        else if (shotHit == true && destroyed != true)
            cout << "hit something, resulting in:" <<endl;
        else if (shotHit == true && destroyed == true)
            cout << "destroyed the " << shipName(shipId)<< ", resulting in:" <<endl;
    }
    b.display(shotsOnly);
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
        << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
        << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
        << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
            << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}


