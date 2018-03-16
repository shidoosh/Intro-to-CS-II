

#include "Set.h"
#include <iostream>

/* Constructor */
Set::Set()
{
    /* create an empty set */
    m_size = 0;
    head = nullptr;
    tail = nullptr;
}

/* Deconstructor */
Set::~Set()
{
    while(head!=nullptr)
    {
        Node * temp = head->next;
        delete head;
        head = temp;
    }
}

/* Copy constructor */
Set::Set(const Set& alternate)
{
    m_size = alternate.m_size;

    /* empty list. could also check if alternate.head is NULL */
    if(m_size==0)
    {
        head = nullptr;
        tail = nullptr;
    }
    else
    {
        Node * p;
        Node * temp = nullptr;
        for(p=alternate.head; p != nullptr; p=p->next)
        {
            /* first copying */
            if(p == alternate.head)
            {
                /* create first new Node in list, get corresponding data  */
                temp = new Node;
                temp->m_data = p->m_data;
                temp->prev=nullptr;
                /* beginning of list, head points to this Node */ 
                head = temp;
            }
            
            /* end of list */
            if(p->next == nullptr)
            {
                /* make next nullptr to indicate end of list
                end of list, make tail point to this Node */
                temp->next = nullptr;
                tail = temp;
            }
            
            /* more Nodes to copy */
            else
            {
                /* create new Node, get corresponding data */
                Node * nextTemp = new Node;
                nextTemp->m_data = p->next->m_data;
                /* link the previous Node to the new Node */
                temp->next = nextTemp;
                /* link to New node to the previous one */
                nextTemp->prev = temp;
                /* temp pointer now points to new Node so when at
                at next iteration, temp will point to most
                recently inserted node.*/
                temp = nextTemp;
            }
        }
    }
}

Set& Set::operator=(const Set& rhs)
{
    /* cannot assign to itself, don't do anything */
    if(&rhs == this)
        return *this;
    
    /* need a temp variable to match swap signature */
    Set temp = rhs;
    
    /* swap will take rhs of assignment and swap rhs with 'this' set */ 
    swap(temp);
    
    return *this;
}

/* Return true if the set is empty, otherwise false. */
bool Set::empty() const
{
    if(m_size==0)
        return true;
    else
        return false;
}

/* Return the number of items in the set. */
int Set::size() const
{
    return m_size;
}

/* Insert value into the set if it is not already present.  Return
 true if the value was actually inserted.  Leave the set unchanged
 and return false if the value was not inserted (perhaps because it
 is already in the set or because the set has a fixed capacity and
 is full). */
bool Set::insert(const ItemType& value)
{
    /* if value is already in the list, do not insert */
    if(contains(value))
        return false;
    
    /* create new Node. insert on following conditions */
    Node * temp = new Node;
    temp->m_data = value;
    
    /* empty list */
    if(head==nullptr)
    {
        temp->next=nullptr;
        temp->prev=nullptr;
        head=temp;
        tail=temp;
    }
    
    /* non-empty list */
    for(Node * p = head; p!=nullptr; p=p->next)
    {
        /* insert at beginning */
        if(p==head && value < p->m_data)
        {
            temp->next=p;
            p->prev=temp;
            temp->prev=nullptr;
            head=temp;
            break;
        }
        /* appending to end */
        if(p->next == nullptr)
        {
            if(value > p->m_data)
            {
                p->next=temp;
                temp->prev=p;
                temp->next=nullptr;
                tail=temp;
                break; 
            }
        }
        /* inserting in the middle */
        if(p->m_data < value && p->next->m_data >= value)
        {
            p->next->prev=temp;
            temp->prev=p;
            temp->next=p->next;
            p->next=temp;
            break; 
        }
    }
    /* inserted a new Node, update the size */
    m_size++;
    return true;
}


/* Remove the value from the set if present.  Return true if the
 value was removed; otherwise, leave the set unchanged and
 return false. */
bool Set::erase(const ItemType& value)
{
    if(!contains(value))
        return false;
    
    /* value is in the list, continue */
    Node * toDelete=nullptr;
    for(Node * p = head; p != nullptr; p = p->next)
    {
        /* erasing from beginning */
        if(p==head && p->m_data == value)
        {
            toDelete = p;
            /* if erasing one element list, p->next>prev is bad access! */ 
            if(p->next!=nullptr)
                p->next->prev = nullptr;
            head=p->next;
            break;
        }
        
        /* erasing from middle or the last node */
        if(p->next->m_data == value)
        {
            toDelete = p->next;
            p->next = toDelete->next;
            
            /* erasing last node
            need to check here to avoid accessing nullptr in next line
            also need to adjust tail pointer */
            if(toDelete->next==nullptr)
            {
                tail=p;
                break; 
            }
            /* erasing from middle */
            toDelete->next->prev=p;
            break;
        }
    }
    delete toDelete;
    m_size--;
    return true;
}


/* Return true if the value is in the set, otherwise false. */
bool Set::contains(const ItemType& value) const
{
    for(Node * p = head; p != nullptr; p = p->next)
    {
        if(p->m_data == value)
            return true;
    }
    return false;
}


/*  If 0 <= i < size(), copy into value the item in the set that is
 greater than exactly i items in the set and return true.  Otherwise,
 leave value unchanged and return false. */
bool Set::get(int i, ItemType& value)
{
    /* 0 <= i < size() is not true, return false */
    if(i >= size() || i < 0)
        return false;
    
    Node * p = head;
    /*  since in ascending order, we can iterate through list i times
    which will be exactly how many items that value is greater than
    and get value at that "index" */
    for(int j = 0; j < i; j++)
        p=p->next;
        
    /* copy into value */
    value=p->m_data;
    return true;
}


/* Exchange the contents of this set with the other one. */
void Set::swap(Set& other)
{
    /* switch head nodes */
    Node * tempHead = other.head;
    other.head = head;
    head = tempHead;
    
    /* switch tail nodes */
    Node * tempTail = other.tail;
    other.tail = tail;
    tail = tempTail;
    
    /* switch sizes */
    int otherSize = other.m_size;
    other.m_size = m_size;
    m_size = otherSize;

}


void unite(const Set& s1, const Set& s2, Set& result)
{
    Set temp = s1;
    Set temp2 = s2;
    /* put s1 in result */
    result.swap(temp);
    /* loop through s2 */
    for(int i = 0; i < temp2.size(); i++)
    {
        ItemType x;
        /* get each value of the set */
        temp2.get(i, x);
        /* if it's not already in the set, insert it! */
        if(!(result.contains(x)))
            result.insert(x);
    }
}
void subtract(const Set& s1, const Set& s2, Set& result)
{
    Set temp = s1;
    Set temp2 = s2;
    result.swap(temp);
    for(int i = 0 ; i < temp2.size(); i++)
    {
        ItemType x;
        /* get each value of the set */
        temp2.get(i, x);
        /* if it's already in the set, erase it! */
        if(result.contains(x))
            result.erase(x);
    }
}
