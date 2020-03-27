// lockDownNesEmu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "types.h"

using namespace std;

int main()
{ 
  cout << "Lockdown Nes Emulator v0.01" << endl;
  cout << "   William Kosasih, 27 March 2020" << endl;
  cout << endl;
  cout << "Type help for help" << endl;
  cout << "> ready!" << endl;

  string command;
  vector<string> command_split(10);


  while (true)
  {
    cout << "< ";
    cin >> command;
    stringstream sstream(command);
    index32_t i = 0;
    while (sstream.good() && i < 10) sstream >> command_split[i];
    
    if (command_split[0] == "help")
    {
    
    }
    else if (command_split[0] == "load")
    {
      
    }
  }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
