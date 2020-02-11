#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include "Cache.h"

using namespace std;

int main()
{
    string input_line, instruction, hex_address;

    while(getline(cin, input_line))
    {
        istringstream iss(input_line);
        iss >> instruction;
        iss >> hex_address;
        hex_address.insert(hex_address.begin(), 8 - hex_address.length(), '0');
        //input_params((istream_iterator<string>(iss)), istream_iterator<string>());
        cout << instruction << " and " << hex_address << endl;
        cout << hex_address << endl;
    }
    return 0;
}
