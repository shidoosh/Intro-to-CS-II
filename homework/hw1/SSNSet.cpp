

#include "SSNSet.h"
#include <iostream> 

SSNSet::SSNSet()          // Create an empty SSN set.
{
    
}
bool SSNSet::add(ItemType ssn)
{
    bool result = false;
    if(SSN.insert(ssn))
    {
        result = true;
    }
    return result;
}
// Add an SSN to the SSNSet.  Return true if and only if the SSN
// was actually added.

int SSNSet::size() const  // Return the number of SSNs in the SSNSet.
{
    return SSN.size();
}
void SSNSet::print() const
{
    for(int i = 0; i < SSN.size(); i++)
    {
        ItemType socialSecNum;
        SSN.get(i, socialSecNum);
        std::cout << socialSecNum << std::endl;
    }
}
// Write every SSN in the SSNSet to cout exactly once, one per
// line.  Write no other text.
