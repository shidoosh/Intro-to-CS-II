

#include <iostream>
#include <cassert>
#include "newSet.h" /* or #include "Set.h" */
int main() {

/*
    Set s;
        assert(s.empty());
        ItemType x = "arepa";
        assert( !s.get(42, x)  &&  x == "arepa"); // x unchanged by get failure
        s.insert("chapati");
        assert(s.size() == 1);
        assert(s.get(0, x)  &&  x == "chapati");
        cout << "Passed all tests" << endl;
    
    
    Set s1;
        s1.insert("lavash");
        s1.insert("roti");
        s1.insert("chapati");
        s1.insert("injera");
        s1.insert("roti");
        s1.insert("matzo");
        s1.insert("injera");
        assert(s1.size() == 5);  // duplicate "roti" and "injera" were not added

    
        string y;
        s1.get(0, y);
        assert(y == "chapati");  // "chapati" is greater than exactly 0 items in ss
        s1.get(4, y);
        assert(y == "roti");  // "roti" is greater than exactly 4 items in ss
        s1.get(2, y);
        assert(y == "lavash");  // "lavash" is greater than exactly 2 items in ss


    
    Set ss;
        ss.insert("dosa");
        assert(!ss.contains(""));
        ss.insert("tortilla");
        ss.insert("");
        ss.insert("focaccia");
        assert(ss.size()==4);
        assert(ss.contains(""));
        ss.erase("dosa");
        assert(ss.size() == 3  &&  ss.contains("focaccia")  &&  ss.contains("tortilla")  &&
               ss.contains(""));

    Set ss1;
        ss1.insert("bing");
        ss1.insert("white");
        assert(ss1.size()==2);

    Set ss2;
        ss2.insert("matzo");
        ss2.insert("pita");
        assert(ss2.size()==2);
        ss1.swap(ss2);
        assert(ss1.size() == 2);
        assert(ss2.size() == 2);

        assert(ss1.contains("matzo"));
        assert(ss1.contains("pita"));

        assert(ss2.contains("bing"));
        assert(ss2.contains("white"));
        assert(!ss1.contains("bing"));
        assert(!ss2.contains("matzo"));
        assert(!ss2.contains("pita"));



    Set ss3;
        ss3.insert("bing");
    Set ss4;
        ss4.insert("matzo");
        ss4.insert("pita");
        ss4.swap(ss3);
        assert(ss3.size() == 2  &&  ss3.contains("matzo")  &&  ss3.contains("pita")  &&
               ss4.size() == 1  &&  ss4.contains("bing"));
    
*/
    return 0;
}
