// C++ program to demonstrate  
// accessing of data members  
 
#include <bits/stdc++.h>  
using namespace std; 
class DateTime 
{ 
    // Access specifier 
    public: 
  
    // Data Members 
    unsigned int years; 
    unsigned int months; 
    unsigned int days; 
    unsigned int hours; 
    unsigned int minutes; 
    unsigned int seconds; 
  
}; 
  
int main() { 
  
    // Declare an object of class geeks 
    DateTime obj1; 
  
    // accessing data member 
    obj1.years = 15; 

    cout << "Year is: " << obj1.years;
    cout << "\nMonth is: " << obj1.months;  
    return getchar(); 
} 