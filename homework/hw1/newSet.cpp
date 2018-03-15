

#include "newSet.h"

/* Constructor that takes user defined max capacity for the set */
Set::Set(int maxCapacity): m_capacity(maxCapacity), m_size(0)
{
    if(maxCapacity > 0)
        set=new ItemType[m_capacity];
    else
    {
        std::cout << "Please pass a non-negative capacity" << std::endl;
        exit(1); 
    }
}

/* Copy constructor */ 
Set::Set(const Set& alternate)
{
    m_size = alternate.m_size;
    m_capacity=alternate.m_capacity;
    
    set = new ItemType[m_capacity];
    
    for(int i = 0; i < m_size; i++ )
        set[i]=alternate.set[i];
}

/* Destructor */
Set:: ~Set()
{
    delete [] set;
}

/* Assignment Operator */
Set& Set::operator=(const Set& rhs)
{
    if(this != &rhs)
    {
        delete [] set;
        m_size = rhs.m_size;
        m_capacity = rhs.m_capacity;
        set = new ItemType[m_capacity];
        for(int i = 0; i < m_capacity; i++)
            set[i] = rhs.set[i];
    }
    return *this;
}

bool Set::empty() const
{
    if(m_size==0)
        return true;

    return false;
}

int Set::size() const
{
    return m_size;
}

bool Set::insert(const ItemType& value)
{
    /* if value is already present of set has reached fixed max capacity,
     do not insert */
    if((contains(value)) || m_size == m_capacity || m_capacity == 0)
        return false;

    /* first insertion */
    if(m_size==0)
        set[m_size]=value;

    Set temp=*this;

    /* inserting in beginning of set */
    if(value < set[0])
    {
        for(int i = 0; i < m_size; i++)
            set[i+1]=temp.set[i];

        set[0]=value;
        m_size++;
        return true;
    }

    bool result=false;
    /* insert in ascending order */
    for(int i = 0; i < m_size; i++)
    {
        /* no duplicates, so no need to check for elements of equal value */
        if(value > set[i] && value < set[i+1])
        {
            /* condition is met such that we are inserting between 2 values in set */
            result=true;
            int toInsert=i+1;

            /* shift everything to the right */
            for(int j = i+1; j < m_size; j++)
                set[j+1]=temp.set[j];

            /* can now insert in corresponding index */
            set[toInsert]=value;
            break;
        }
    }
    /* inserting at end of set. at this point, we know that the value is greater
     than all other values in list, since it didnt satisfy condition in loop */
    if(!result)
        set[m_size]=value;
    /* just inserted, so increment the size */
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    if(!contains(value))
        return false;

    for(int i = 0; i < m_size; i++)
    {
        /* look for index of value */
        if(set[i]==value)
        {
            /* shift everything one to the left */
            for(int j = i; j < m_size - 1; j++)
                set[j] = set[j+1];
            break;
        }
    }
    /* erased an element, decrement the size */
    m_size--;
    return true;
}


bool Set::contains(const ItemType& value) const
{
    for(int i = 0; i < m_size; i++)
    {
        /* found */
        if(set[i]==value)
            return true;
    }
    /* not found */
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if(i >= m_size || i < 0)
        return false;

    value=set[i];
    return true;
}

void Set::swap(Set& other)
{
    Set temp=other;
    other=*this;
    *this=temp;
}



