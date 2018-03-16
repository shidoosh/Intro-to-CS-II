


#ifndef Set_h
#define Set_h

#include <string>
#include <iostream>
typedef std::string ItemType;

using namespace std;
class Set
{
public:
    Set();         // Create an empty set.
    
    ~Set();        // Deconstructor.
    
    Set(const Set& alternate); // Copy constructor.
    
    Set& operator=(const Set& rhs);
    
    bool empty() const;  // Return true if the set is empty, otherwise false.
    
    int size() const;    // Return the number of items in the set.
    
    bool insert(const ItemType& value);
    // Insert value into the set if it is not already present.  Return
    // true if the value was actually inserted.  Leave the set unchanged
    // and return false if the value was not inserted (perhaps because it
    // is already in the set or because the set has a fixed capacity and
    // is full).
    
    bool erase(const ItemType& value);
    // Remove the value from the set if present.  Return true if the
    // value was removed; otherwise, leave the set unchanged and
    // return false.
    
    bool contains(const ItemType& value) const;
    // Return true if the value is in the set, otherwise false.
    
    bool get(int i, ItemType& value);
    // If 0 <= i < size(), copy into value the item in the set that is
    // greater than exactly i items in the set and return true.  Otherwise,
    // leave value unchanged and return false.
    
    void swap(Set& other);
    // Exchange the contents of this set with the other one.
    

        //FOR TESTING PURPOSES!
        void dump()
        {
            for(Node*v = head; v!=nullptr; v = v->next)
            {
                std::cerr << v->m_data << std::endl;
            }
            if(empty()==true)
            {
                std::cout << "it's empty!" << std::endl;
            }
            std::cout << std::endl;
        }

private:
    int m_size;
    struct Node
    {
        ItemType m_data;
        Node*next;
        Node*prev;
    };
    Node*head;
    Node*tail;
    
};

//////////////////////////////////////////////////////////////////
/**********************NON-MEMBER FUNCTIONS**********************/
//////////////////////////////////////////////////////////////////
void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);

#endif /* Set_h */


