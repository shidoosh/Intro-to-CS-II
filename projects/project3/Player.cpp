#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
    // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

// TODO:  You need to replace this with a real class declaration and
//        implementation.
//typedef AwfulPlayer HumanPlayer;
class HumanPlayer: public Player
{
public:
    HumanPlayer(string nm, const Game& g) : Player(nm, g){}
    virtual ~HumanPlayer() {}
    virtual bool isHuman() const { return true; }
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p) {}
    bool placeShipHelper(Board&b, Point& toplace, Direction& playerChoice);
private:
    vector<Point> hitPoints;
};


bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " must place " << game().nShips() << " ships." << endl;
    b.display(false);
    
    char direction;
    int row;
    int col;
    Direction playerChoice;
    
    
    for (int i=0; i<game().nShips(); i++)
    {
        do {
            cout<< "Enter h or v for direction of "<< game().shipName(i) <<" (length " <<game().shipLength(i) << "): ";
            cin>> direction;
            
            if (direction != 'h' && direction != 'v')
            {
                cout << "Direction must be h or v." <<endl;
            }
        } while (direction != 'h' && direction != 'v');
        
        if (direction == 'h')
            playerChoice = HORIZONTAL;
        
        else
            playerChoice = VERTICAL;
        
        do {
            cout << "Enter row and column of topmost cell (e.g. 3 5): ";
            if (getLineWithTwoIntegers(row, col))
            {
                Point pointPlaceShip(row, col);
                
                if(!b.placeShip(pointPlaceShip, i, playerChoice))
                    cout << "The ship can not be placed there." <<endl;
                else
                    break;
            }
        }while (true);
        
        b.display(false);   //after placed, display board
    }
    return true;
}

Point HumanPlayer::recommendAttack()    //human, so it's not "recommmended"
{                                       //do what the human says, using getline
    int row;
    int col;
    Point attackPoint;
    cout << "Enter the row and column to attack (e.g, 3 5): ";
    if (getLineWithTwoIntegers(row, col))
    {
        attackPoint.r = row;
        attackPoint.c = col;
    }
    hitPoints.push_back(attackPoint);   //store points that have been attacked
    return attackPoint;
}


void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId)
{
    validShot = true;
    if(!game().isValid(p))
    {
        validShot = false;
        return;
    }
    
    for(vector<Point>::iterator it = hitPoints.begin(); it != hitPoints.end(); it++)
    {
        if(it->r == p.r && it->c == p.c)
        {
            validShot = false;  //if shot found in vector that holds previous shots, return false.
            break;              //you've hit it before! wasted a shot.
        }
    }
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************


// TODO:  You need to replace this with a real class declaration and
//        implementation.
class MediocrePlayer: public Player
{
public:
    MediocrePlayer(string nm, const Game& g): Player(nm, g) { state1 = true; }
    virtual ~MediocrePlayer() {}
    virtual bool isHuman() const { return false;}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p){}
private:
    bool state1;
    vector<Point> shots;
    vector<Point> hasBeenShot;
    bool recursive(Board&b, int shipId);
    Point unusedPoint();
};

bool MediocrePlayer::placeShips(Board& b)
{
    for (int i=0; i<50; i++)
    {
        b.block();
        if(recursive(b, 0)) //recursive determines whether it can be placed. if true, then return true.
        {
            b.unblock();
            return true;
        }
        else
        {
            b.unblock();
            continue;   //continue for the 50 iterations. if reached, return false.
        }
    }
    return false;
}

bool MediocrePlayer::recursive(Board& b, int shipId)
{
    if (shipId >= game().nShips())
        return true;
    
    for (int i=0; i<game().rows(); i++)
    {
        for (int j=0; j<game().cols(); j++)
        {
            Point temp(i, j);
            if(b.placeShip(temp, shipId, HORIZONTAL)) //for placing horizontally..
            {
                if (recursive(b, shipId+1)) //if can be placed
                    return true;
                else
                    b.unplaceShip(temp, shipId, HORIZONTAL); //otherwise, unplace that ship
            }
            
            if (b.placeShip(temp, shipId, VERTICAL)) //for placing vertically..
            {
                if(recursive(b, shipId+1))  //if can be placed
                    return true;
                else
                    b.unplaceShip(temp, shipId, VERTICAL);  //otherwise, unplace that ship
            }
        }
    }
    return false;
}

Point MediocrePlayer::unusedPoint()
{
    bool newPoint = false;
    Point temp(0,0);
    do
    {
        temp = game().randomPoint();    //need random coordinate at first
        
        vector<Point>::iterator p = hasBeenShot.begin();
        for (p = hasBeenShot.begin(); p != hasBeenShot.end(); p++)
        {
            if ((*p).r == temp.r && (*p).c == temp.c)   //search if the coordinate has been visited
                break;
        }
        if( p == hasBeenShot.end())
        {
            newPoint = true;
            hasBeenShot.push_back(temp);    //otherwise, push this coordinate.
        }
    } while (!newPoint);
    
    return temp;    //return the coordinate
}

Point MediocrePlayer::recommendAttack()
{
    if (state1==true)
        return unusedPoint(); //if in state 1, return an unused point

    else
    {
        int index = randInt((int)shots.size());
        Point randomPoint(shots[index].r, shots[index].c);
        
        for (vector<Point>::iterator p = shots.begin(); p != shots.end(); p++)
        {
            if(p->r == randomPoint.r && p-> c == randomPoint.c)
            {
                p = shots.erase(p);
                break;
            }
        }
        hasBeenShot.push_back(randomPoint);
        return randomPoint;
    }
}


void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId)
{
    if (shots.empty())  //no shots taken yet
        state1 = true;
    
    if (state1==false)  //in state 2
    {
        if (shotHit == false)
            state1 = false;

        if((shotHit == true) && (shipDestroyed == false))
            state1 = false;
        
        if ((shotHit == true)&&(shipDestroyed == true))
        {
            state1 = true;
            shots.clear();
        }
    }
    
    if(state1==true)
    {
        if (shotHit == false)
            state1 = true;
        
        else if (shipDestroyed == true)
            state1 = true;

        else if (shipDestroyed == false && shotHit == true)
        {
            state1 = false;
            for (int i = 1; i <= 4; i++)    //to make the cross such that hit point is the middle
            {
                Point toAdd(p.r-i, p.c);    //below middle
                if(game().isValid(toAdd))
                {
                    vector<Point>::iterator it = hasBeenShot.begin();
                    for (it = hasBeenShot.begin(); it != hasBeenShot.end(); it++)
                    {
                        if((*it).r == toAdd.r && (*it).c == toAdd.c)
                            break;
                    }
            
                    if (it == hasBeenShot.end())    //wasn't found so push
                        shots.push_back(toAdd);
                }
            }
            for (int i = 1; i <= 4; i++)
            {
                Point toAdd(p.r+i, p.c);    //above middle
                if(game().isValid(toAdd))
                {
                    vector<Point>::iterator it = hasBeenShot.begin();
                    for (it = hasBeenShot.begin(); it != hasBeenShot.end(); it++)
                    {
                        if( (*it).r == toAdd.r && (*it).c == toAdd.c)
                            break;
                    }
                    
                    if (it == hasBeenShot.end())    //wasn't found so push
                        shots.push_back(toAdd);
                }
            }
            for (int i = 1; i <= 4; i++)
            {
                Point toAdd(p.r, p.c-i);    //left of middle
                if(game().isValid(toAdd))
                {
                    vector<Point>::iterator it = hasBeenShot.begin();
                    for (it = hasBeenShot.begin(); it != hasBeenShot.end(); it++)
                    {
                        if((*it).r == toAdd.r && (*it).c == toAdd.c)
                            break;
                    }
                    if (it == hasBeenShot.end())    //wasn't found so push
                        shots.push_back(toAdd);
                }
            }
            for (int i = 1; i <= 4; i++)
            {
                Point toAdd(p.r, p.c+i);    //right of middle
                if(game().isValid(toAdd))
                {
                    vector<Point>::iterator it = hasBeenShot.begin();
                    for (it = hasBeenShot.begin(); it != hasBeenShot.end(); it++)
                    {
                        if((*it).r == toAdd.r && (*it).c == toAdd.c)
                            break;
                    }
                    if (it == hasBeenShot.end())    //wasn't found
                        shots.push_back(toAdd); //so push
                }
            }
        }
    }
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer: public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer() {}
    virtual bool isHuman() const {return false;}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                    bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p) {}
private:
    bool state1;
    vector<Point> shots;
    vector<Point> hasBeenShot;
    
    vector<Point> targets;
    vector<Point> randomShots;
    vector<int> oddNumbers;
    void addPointsAroundIt(Point p);
    
    Point generateRandomPointForState1();
};

GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g)
{
    state1 = true;
    for(int i = 0; i < game().rows(); i++)
    {
        if(i % 2 == 1)
            oddNumbers.push_back(i);
    }
    for(int i = 0; i < game().rows(); i++)
    {
        //random points for state 1
        Point randTempPlus(i, i + oddNumbers[i]);
        
        if(game().isValid(randTempPlus))
            randomShots.push_back(randTempPlus);
        
        Point randTempMinus(i, i - oddNumbers[i]);
        if(game().isValid(randTempMinus))
            randomShots.push_back(randTempMinus);
    }
    for (int i = 0; i < game().rows(); i++)
    {
        //hard code every other point, attacking every other is good strategy for battlefield.
        targets.push_back(Point(i, i));
        
        Point temp(i, i+2);
        if(game().isValid(temp))
        {
            targets.push_back(temp);
        }
        Point temp1(i, i-2);
        if(game().isValid(temp1))
        {
            targets.push_back(temp1);
        }
        Point temp2(i, i+4);
        if(game().isValid(temp2))
        {
            targets.push_back(temp2);
        }
        Point temp3(i, i-4);
        if(game().isValid(temp3))
        {
            targets.push_back(temp3);
        }
        Point temp4(i, i+6);
        if(game().isValid(temp4))
        {
            targets.push_back(temp4);
        }
        Point temp5(i, i-6);
        if(game().isValid(temp5))
        {
            targets.push_back(temp5);
        }
        Point temp6(i, i+8);
        if(game().isValid(temp6))
        {
            targets.push_back(temp6);
        }
        Point temp7(i, i-8);
        if(game().isValid(temp7))
        {
            targets.push_back(temp7);
        }
    }
}

bool GoodPlayer::placeShips(Board& b)
{
    for(int i = 0; i < game().nShips(); i++)
    {
        Point random = game().randomPoint();
        int direction = randInt(2); //randomly place ships
        
        if(direction == 0)
        {
            int count = 0;
            while(count < 10)
            {
                count++;
                random = game().randomPoint();
                if(b.placeShip(random, i, HORIZONTAL))  //could be placed horizontally
                    break;
                else
                    continue;   //if not, try another
            }
        }
        if(direction == 1)
        {
            int count = 0;
            while(count<10)
            {
                count++;
                random = game().randomPoint();
                if(b.placeShip(random, i, VERTICAL)) //could be placed vertically
                    break;
                else
                    continue;   //if not, try another
            }
        }
    }
    return true;
}

void GoodPlayer::addPointsAroundIt(Point p)
{   //add points around passed points
    Point temp(p.r-1, p.c); //below
    if(game().isValid(temp))
    {
        vector<Point>::iterator it = hasBeenShot.begin();
        for(; it != hasBeenShot.end(); it++)
        {
            if((it->r == temp.r) && (it->c == temp.c))
                break;
        }
        if(it == hasBeenShot.end()) //not been shot yet, so push
            shots.push_back(temp);
    }
    Point temp1(p.r+1, p.c);    //abovee
    if(game().isValid(temp1))
    {
        vector<Point>::iterator it =hasBeenShot.begin();
        for(; it != hasBeenShot.end(); it++)
        {
            if((it->r == temp1.r) && (it->c == temp1.c))
                break;
        }
        if( it == hasBeenShot.end())    //not been shot yet, so push
            shots.push_back(temp1);
    }
    Point temp2(p.r, p.c-1);    //to the left
    if(game().isValid(temp2))
    {
        vector<Point>::iterator it = hasBeenShot.begin();
        for(; it != hasBeenShot.end(); it++)
        {
            if((it->r == temp2.r) && (it->c == temp2.c))
                break;
        }
        if(it == hasBeenShot.end()) //not been shot yet, so push
            shots.push_back(temp2);
    }
    
    Point temp3(p.r, p.c+1);    //to the right
    if(game().isValid(temp3))
    {
        vector<Point>::iterator it =hasBeenShot.begin();
        for(; it != hasBeenShot.end(); it++)
        {
            if((it->r == temp3.r) && (it->c == temp3.c))
                break;
        }

        if( it == hasBeenShot.end())    //not been shot yet, so push
            shots.push_back(temp3);
    }
    
}


Point GoodPlayer::generateRandomPointForState1()
{
    int index;
    Point randomPoint;
    
    while(true)
    {
        if(targets.size() !=0)
        {
            index = randInt((int)targets.size());
            
            randomPoint.r = targets[index].r;
            randomPoint.c = targets[index].c;
            
            for(vector<Point>::iterator it = targets.begin();
                it != targets.end(); it++)
            {
                if (it->r == randomPoint.r && it->c == randomPoint.c)
                {
                    it = targets.erase(it);
                    break;
                }
            }

            vector<Point>::iterator it = hasBeenShot.begin();
            for (; it != hasBeenShot.end(); it++)
            {
                if(randomPoint.r == it->r && randomPoint.c == it->c)
                    break;
            }

            if(it!=hasBeenShot.end())
                continue;

            if(it == hasBeenShot.end())
            {
                hasBeenShot.push_back(randomPoint);
                break;
            }

        }

        else
        {
            while(true)
            {
                if(!randomShots.empty())
                {
                    int index= randInt((int)randomShots.size());
                    randomPoint = randomShots[index];
                    vector<Point>::iterator it = hasBeenShot.begin();
                    for (; it != hasBeenShot.end(); it++)
                    {
                        if(randomPoint.r == it->r && randomPoint.c == it->c)
                            break;
                    }
                    
                    if(it == hasBeenShot.end()) //if not found, we can push the random point
                    {
                        hasBeenShot.push_back(randomPoint);
                        return randomPoint;
                    }
                    
                    
                    
                    for(vector<Point>::iterator it = randomShots.begin();
                        it != randomShots.end(); it++)
                    {
                        if(it->r == randomPoint.r && it->c == randomPoint.c)
                        {
                            it = randomShots.erase(it); //if found, erase
                            break;
                        }
                    }
                }
            }
        }
    }
    return randomPoint;
}



Point GoodPlayer::recommendAttack()
{
    Point attackPoint;
    int index;
    bool genPoint = true;   //if we need to generate a new point, which is true intitially
    
    if(state1==false)
    {
        if(shots.empty())
            state1 = true;
        else
        {
            while(genPoint)
            {
                index = randInt((int)shots.size());
                attackPoint.r = shots[index].r;
                attackPoint.c = shots[index].c;

                vector<Point>::iterator it = hasBeenShot.begin();
                for(; it != hasBeenShot.end(); it++)
                {
                    if((it->r == attackPoint.r) && (it->c == attackPoint.c))
                        genPoint = true;
                }
                if(it == hasBeenShot.end()) //need point
                {
                    genPoint = false;
                    hasBeenShot.push_back(attackPoint);
                }
                for(vector<Point>::iterator iter = shots.begin();
                    iter != shots.end(); iter++)
                {
                    if((iter->r == attackPoint.r) && (iter->c == attackPoint.c))
                    {
                        iter = shots.erase(iter);   //erase if found
                        break;
                    }
                }
            }
        }
    }

    if (state1==true)
        attackPoint = generateRandomPointForState1();   //attack randomly
    return attackPoint;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId)
{
    Point newPoint;
    if(state1==false)
    {
        if((shotHit == true) && (shipDestroyed ==false))
            addPointsAroundIt(p);
    }
    if (state1==true)
    {
        if(shotHit == true && shipDestroyed == false)
        {
            state1 = false;
            newPoint = p;
            addPointsAroundIt(p);   //hit a ship: find point around it, shoot, if hit go in that direction
        }
    }
}
//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
         type != types[pos]; pos++)
        ;
    switch (pos)
    {
        case 0:  return new HumanPlayer(nm, g);
        case 1:  return new AwfulPlayer(nm, g);
        case 2:  return new MediocrePlayer(nm, g);
        case 3:  return new GoodPlayer(nm, g);
        default: return nullptr;
    }
}





