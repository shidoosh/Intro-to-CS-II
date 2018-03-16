
#include "SymbolTable.h"
#include <string>
#include <vector>
#include <functional>
#include <stack>
#include <list>
#include <iostream>
using namespace std;

struct Node
{
    Node(string ID, int lineNumber, int scope) {
        mID = ID;
        lineNum = lineNumber;
        mScope = scope;
    }
    string mID;
    int lineNum;
    int mScope;
};

class SymbolTableImpl
{
public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    int hashFunction(const string &id) const;
    list<Node*> hashTable[19997];
    stack<Node*> allNodes;
    int scopeCount;
};


int SymbolTableImpl::hashFunction(const string &id) const
{
    int MAXBUCKETS = 19997;  //biggest prime number within max buckets
    std::hash<std::string> str_hash;
    unsigned int hashValue = (unsigned int)str_hash(id); //id as string -> id as unsigned int
    unsigned int bucketNum = hashValue % MAXBUCKETS;     //use int value of string to hash into bucket
    return bucketNum;
}

int SymbolTableImpl::find(const string& id) const
{
    /* entry marker */ 
    if (id == "")
        return -1;

    /* find bucket */
    int bucket = hashFunction(id);

    /* search for most recent occurence of id //when we declare we push_front, so start for begin */
    list <Node*>::const_iterator it;
    for (it = hashTable[bucket].begin(); it != hashTable[bucket].end(); it++)
    {
        /* if found, return the line number it occured in */
        if ((*it)->mID == id)
            return (*it)->lineNum;
    }
    return -1;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
    /* entry marker */
    if (id == "")
        return false;
    
    /* find bucket */
    int bucket = hashFunction(id);

    /* loop through nodes in this bucket */
    list <Node*>::iterator it;
    for (it = hashTable[bucket].begin(); it != hashTable[bucket].end(); it++)
    {
        if((*it)->mScope < scopeCount)
            break;
        /* already in hash table */
        else if ((*it)->mID == id && (*it)->mScope == scopeCount)
            return false;
    }
    /* at this point post-loop, not in hash table. so add! */
    Node* addNode = new Node(id, lineNum, scopeCount);
    hashTable[bucket].push_front(addNode);
    allNodes.push(addNode);
    return true;
}


void SymbolTableImpl::enterScope()
{
    Node *start = new Node("",-1, scopeCount);
    allNodes.push(start);
    scopeCount++;
}


bool SymbolTableImpl::exitScope()
{
    /* can't exit if there's no scope to begin with */
    if (allNodes.empty())
        return false;

    while (!allNodes.empty() &&  allNodes.top()->mID != "")
    {
        Node *top = allNodes.top();
        
        /* find bucket */
        int bucket = hashFunction(top->mID);

        list <Node*>::const_iterator it;
        /* loop through the nodes in returned bucket */
        for (it = hashTable[bucket].begin(); it != hashTable[bucket].end(); it++)
        {
            if((*it)->mScope < scopeCount)
                break;
            else if ((*it)->mID == top->mID)
            {
                allNodes.pop();
                /* delete this scope */
                Node* toDelete = *it;
                it = hashTable[bucket].erase(it);
                delete toDelete;
                break;
            }
            else
                it++;
        }
    }
    /*pop the bucket markers*/
    if(scopeCount==0)
        return false;
    else
    {
        allNodes.pop();
        scopeCount--;
    }

    return true;
}


//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
    m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
    delete m_impl;
}

void SymbolTable::enterScope()
{
    m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
    return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
    return m_impl->find(id);
}

