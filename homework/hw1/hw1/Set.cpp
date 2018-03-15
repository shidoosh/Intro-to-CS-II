

#include "Set.h"
/* Create an empty set */
Set::Set()         // Create an empty set.
{
    m_size=0;
}

/* Return true if the set is empty, otherwise false. */
bool Set::empty() const
{
    if(m_size==0)
        return true;

    return false;
}
/* Return the number of items in the set.*/
int Set::size() const
{
    return m_size;
}


/*  Insert value into the set if it is not already present.  Return
  true if the value was actually inserted.  Leave the set unchanged
  and return false if the value was not inserted (perhaps because it
  is already in the set or because the set has a fixed capacity and
  is full).*/
/* Personal note: My intuitive approach is to append value
 to the end, and then sort.
 Shifting values array is intensive but for
 the scope of this assignment, sorting algorithms are not permitted.  */
bool Set::insert(const ItemType& value)
{
    /* if value is already present of set has reached fixed max capacity,
     do not insert */
    if((contains(value)) || m_size == DEFAULT_MAX_ITEMS)
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

/*  Remove the value from the set if present.  Return true if the
  value was removed; otherwise, leave the set unchanged and
  return false. */
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

/* Return true if the value is in the set, otherwise false. */
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


/*  If 0 <= i < size(), copy into value the item in the set that is
  greater than exactly i items in the set and return true.  Otherwise,
  leave value unchanged and return false. */
bool Set::get(int i, ItemType& value) const
{
    if(i >= m_size || i < 0)
        return false;

    value=set[i];
    return true;
}

/* Exchange the contents of this set with the other one. */
void Set::swap(Set& other)
{
    Set temp=other;
    other=*this;
    *this=temp;
}

