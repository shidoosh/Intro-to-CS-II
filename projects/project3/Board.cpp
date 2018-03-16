#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    ~BoardImpl();
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;
    
private:
    // TODO:  Decide what private members you need.  Here's one that's likely
    //        to be useful:
    const Game& m_game;
    
    int findShipID(Point p); //helper for attack function.
    
    char** gameBoard;
    
    struct Ship
    {
        int ID;
        Point startCoor;
        Direction direction;
    };
    Ship* addedShips;
    int totalShips;
    int destroyedShips;
};

BoardImpl::BoardImpl(const Game& g) : m_game(g), totalShips(0), destroyedShips(0)
{
    gameBoard = new char*[g.rows()];
    for (int i=0; i<g.rows(); i++)
        gameBoard[i] = new char[g.cols()];
    addedShips = new Ship[m_game.nShips()];
}

BoardImpl::~BoardImpl()
{
    for (int i=0; i<m_game.rows(); i++)
        delete [] gameBoard[i];
    delete [] gameBoard;
    delete [] addedShips;
}


void BoardImpl::clear()
{
    //clears the board so it is empty, ready to be populated with ships
    for (int i = 0; i <m_game.rows(); i++)
    {
        for (int j = 0; j < m_game.cols(); j++)
            gameBoard[i][j] = '.';
    }
}

int BoardImpl::findShipID(Point p)  //need to know what ship we're destroying
{
    for(int i = 0; i < totalShips; i++)
    {
        if(addedShips[i].direction == VERTICAL)
        {
            if((p.r >= addedShips[i].startCoor.r) && (p.r <= addedShips[i].startCoor.r + m_game.shipLength(addedShips[i].ID)) && (p.c == addedShips[i].startCoor.c))
                    return addedShips[i].ID;
        }
        if(addedShips[i].direction == HORIZONTAL)
        {
            if((p.c >= addedShips[i].startCoor.c) && (p.c <= addedShips[i].startCoor.c + m_game.shipLength(addedShips[i].ID)) && (p.r == addedShips[i].startCoor.r))
                return addedShips[i].ID;
        }
    }
    return -1;
}

void BoardImpl::block()
{
    // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (randInt(2) == 0)
            {
                // TODO:  Replace this with code to block cell (r,c)
                gameBoard[r][c] = '#';
            }
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            // TODO:  Replace this with code to unblock cell (r,c) if blocked
            if (gameBoard[r][c] == '#')
                gameBoard[r][c] = '.';
        }
}


bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (totalShips == m_game.nShips())  //filled up capacity for ships
        return false;
    
    if (shipId < 0 || shipId >= m_game.nShips())    //invalid entries
        return false;
    
    for (int i = 0; i < totalShips; i++)
    {
        if (addedShips[i].ID == shipId) //already placed
            return false;
    }
    
    if (topOrLeft.c < 0 || topOrLeft.c > m_game.cols() || topOrLeft.r < 0 || topOrLeft.r > m_game.rows())
        return false;   //can't fit on board

    int shipLength = m_game.shipLength(shipId);

    if (dir == HORIZONTAL)  //placing horizontally
    {
        if (shipLength > m_game.cols()-topOrLeft.c)
            return false;
        
        for (int i = 0; i < shipLength; i++)
        {
            if (gameBoard[topOrLeft.r][topOrLeft.c + i] != '.')
                return false;   //can't place anywhere else but water, '.'
        }
        
        for (int i=0; i<shipLength; i++)
            gameBoard[topOrLeft.r][topOrLeft.c+i] = m_game.shipSymbol(shipId);
        
        //store info in struct
        addedShips[totalShips].ID = shipId;
        addedShips[totalShips].startCoor.r = topOrLeft.r;
        addedShips[totalShips].startCoor.c = topOrLeft.c;
        addedShips[totalShips].direction = HORIZONTAL;
        
        totalShips++;
        return true;
    }
    
    if (dir == VERTICAL)
    {
        if(shipLength > m_game.rows()-topOrLeft.r)
            return false;
        
        for (int i=0; i<shipLength; i++)
        {
            if(gameBoard[topOrLeft.r+i][topOrLeft.c] != '.')
                return false;   //need to place in water
        }
        
        for (int i=0; i<shipLength; i++)
            gameBoard[topOrLeft.r+i][topOrLeft.c] = m_game.shipSymbol(shipId);

        //store info in struct
        addedShips[totalShips].ID = shipId;
        addedShips[totalShips].startCoor.r = topOrLeft.r;
        addedShips[totalShips].startCoor.c = topOrLeft.c;
        addedShips[totalShips].direction = VERTICAL;
        
        totalShips++;
        return true;
    }
    return false;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    {
        if(shipId >= m_game.nShips() || shipId < 0)
            return false;
        if(topOrLeft.r < 0 || topOrLeft.r > m_game.rows() || topOrLeft.c < 0 || topOrLeft.c > m_game.cols())
            return false;
        for (int r = 0; r < m_game.rows(); r++)
            for (int c = 0; c < m_game.cols(); c++)
            {
                if(gameBoard[r][c]==m_game.shipSymbol(shipId))
                    gameBoard[r][c]='.';    //where the ship was, replace w water
            }
        totalShips--;
        return true;
    }

}

void BoardImpl::display(bool shotsOnly) const
{
    cout<<"  ";
    for (int i=0; i<m_game.cols(); i++)
    {
        cout << i;
    }
    cout << endl;
    
    for(int i=0; i<m_game.rows(); i++)
    {
        cout<< i << " ";
        for(int j=0; j<m_game.cols(); j++)
        {
            if(gameBoard[i][j] != 'X' && gameBoard[i][j] != 'o')
            {
                if(shotsOnly)
                    cout << '.';
                else
                    cout<< gameBoard[i][j];
            }
            else
                cout<< gameBoard[i][j];
        }
        cout<<endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if(!m_game.isValid(p))
        return false;
    
    if(gameBoard[p.r][p.c] == 'X' || gameBoard[p.r][p.c] == 'o')
        return false;   //can't attack where u have before

    if(gameBoard[p.r][p.c] == '.')
    {
        shotHit = false;    //attacked point in water
        gameBoard[p.r][p.c] = 'o';
    }
    else
    {
        shotHit = true; //hit ship!
        gameBoard[p.r][p.c] = 'X';
        int shipAttackId = findShipID(p);
        shipDestroyed = true;
        
        for(int i = 0; i < totalShips; i++)
        {
            if(addedShips[i].ID == shipAttackId)
            {
                if(addedShips[i].direction == HORIZONTAL)
                {
                    for(int j = 0; j < m_game.shipLength(shipAttackId); j++)
                    {
                        if(gameBoard[addedShips[i].startCoor.r][addedShips[i].startCoor.c+j] != 'X')
                        {
                            shipDestroyed= false;
                            break;
                        }

                    }
                }
                if(addedShips[i].direction == VERTICAL)
                {
                    for(int j = 0; j < m_game.shipLength(shipAttackId); j++)
                    {
                        if(gameBoard[addedShips[i].startCoor.r+j][addedShips[i].startCoor.c] != 'X')
                        {
                            shipDestroyed= false;
                            break;
                        }
                    }
                }
            }
        }
        if(shipDestroyed == true)
        {
            shipId = shipAttackId;
            destroyedShips += 1; //doing this to check if game is over
        }
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    return destroyedShips == totalShips;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
