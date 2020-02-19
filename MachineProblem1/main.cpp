#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <cmath>
#include "Cache.h"

using namespace std;

//#define OUTPUT_FILE "output.txt"
#define DEBUG_FILE "debug.txt"

bool DEBUG = true;

string dec2hex(int dec);
int hex2dec(string hex);
/*enum ReplacementPolicies {
    LRU,
    FIFO,
    Optimal
};
enum InclusionProperty{
    non_inclusive,
    inclusive
};*/
void show_help(string filename)
{
    cout << "Usage: " << filename << " <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <REPLACEMENT_POLICY> <INCLUSION_PROPERTY> <trace_file>" << endl;
}
int string2int(string s)
{
    int len = s.size();
    int res = 0;
    bool is_valid = true;
    for(int i=0;i<len;i++)
    {
        if(isdigit(s[i]))
            res = (res * 10 + s[i] - '0');
        else
            is_valid = false;
    }
    if(is_valid)
        return res;
    return -1;

}
bool validate_parameters(string BLOCKSIZE, string L1_SIZE, string L1_ASSOC, string L2_SIZE, string L2_ASSOC, string REPLACEMENT_POLICY, string INCLUSION_PROPERTY, char *TRACE_FILE)
{
    bool is_valid = true;

    int iBLOCKSIZE = string2int(BLOCKSIZE);
    int iL1_SIZE = string2int(L1_SIZE);
    int iL1_ASSOC = string2int(L1_ASSOC);
    int iL2_SIZE = string2int(L2_SIZE);
    int iL2_ASSOC = string2int(L2_ASSOC);
    int iREPLACEMENT_POLICY = string2int(REPLACEMENT_POLICY);
    int iINCLUSION_PROPERTY = string2int(INCLUSION_PROPERTY);

    if(iBLOCKSIZE <= 0)
    {
        cout << "ERROR: Invalid Blocksize (Should be positive integer value)" << endl;
        is_valid = false;
    }
    else if(ceil(log2(iBLOCKSIZE)) != floor(log2(iBLOCKSIZE)))
    {
        cout << "ERROR: Invalid Blocksize (Should be power of two)" << endl;
        is_valid = false;
    }
    if(iL1_SIZE <= 0)
    {
        cout << "ERROR: Invalid L1 size (Should be positive integer value)" << endl;
        is_valid = false;
    }
    if(iL1_ASSOC <= 0)
    {
        cout << "ERROR: Invalid L1 associativity (Should be positive integer value)" << endl;
        is_valid = false;
    }
    if(iL2_SIZE < 0)
    {
        cout << "ERROR: Invalid L2 size (Should be zero or positive integer value)" << endl;
        is_valid = false;
    }
    if(iL2_ASSOC < 0)
    {
        cout << "ERROR: Invalid L2 associativity (Should be zero or positive integer value)" << endl;
        is_valid = false;
    }
    if(iREPLACEMENT_POLICY < 0 || iREPLACEMENT_POLICY > 3)
    {
        cout << "ERROR: Invalid replacement policy (Should be a value between 0 to 2)" << endl;
        is_valid = false;
    }
    if(iINCLUSION_PROPERTY < 0 || iINCLUSION_PROPERTY > 1)
    {
        cout << "ERROR: Invalid inclusion property (Should be 0 or 1)" << endl;
        is_valid = false;
    }
    ifstream iFILE;
    iFILE.open(TRACE_FILE);

    if(!iFILE)
    {
        cout << "ERROR: " << TRACE_FILE << " not exists" << endl;
        is_valid = false;
    }
    iFILE.close();

    return is_valid;
}

int main(int argc, char **argv)
{
    if(argc != 9)
    {
        show_help(argv[0]);
        return 0;
    }
    if(validate_parameters(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]) == false)
    {
        show_help(argv[0]);
        return 0;
    }

    int iBLOCKSIZE = string2int(argv[1]);
    int iL1_SIZE = string2int(argv[2]);
    int iL1_ASSOC = string2int(argv[3]);
    int iL2_SIZE = string2int(argv[4]);
    int iL2_ASSOC = string2int(argv[5]);
    int iREPLACEMENT_POLICY = string2int(argv[6]);
    string sREPLACEMENT_POLICY = (iREPLACEMENT_POLICY == 0 ? "LRU" : (iREPLACEMENT_POLICY == 1 ? "FIFO" : "Optimal"));
    int iINCLUSION_PROPERTY = string2int(argv[7]);
    string sINCLUSION_PROPERTY = (iINCLUSION_PROPERTY == 0 ? "non-inclusive" : "inclusive");
    char *TRACE_FILE = argv[8];

    Cache L1Cache(1, iL1_SIZE, iL1_ASSOC, iBLOCKSIZE, (ReplacementPolicies)iREPLACEMENT_POLICY, (InclusionProperty)iINCLUSION_PROPERTY);
    Cache L2Cache(2, iL2_SIZE, iL2_ASSOC, iBLOCKSIZE, (ReplacementPolicies)iREPLACEMENT_POLICY, (InclusionProperty)iINCLUSION_PROPERTY);

    ifstream iFILE;
    ofstream dFILE;//, oFILE;
    string instruction, hex_address;

    iFILE.open(TRACE_FILE);
    //oFILE.open(OUTPUT_FILE);
    dFILE.open(DEBUG_FILE);

    /*oFILE << "===== Simulator configuration =====" << endl;
    oFILE << "BLOCKSIZE:             " << iBLOCKSIZE << endl;
    oFILE << "L1_SIZE:               " << iL1_SIZE << endl;
    oFILE << "L1_ASSOC:              " << iL1_ASSOC << endl;
    oFILE << "L2_SIZE:               " << iL2_SIZE << endl;
    oFILE << "L2_ASSOC:              " << iL2_ASSOC << endl;
    oFILE << "REPLACEMENT POLICY:    " << sREPLACEMENT_POLICY << endl;
    oFILE << "INCLUSION PROPERTY:    " << sINCLUSION_PROPERTY<< endl;
    oFILE << "trace_file:            " << TRACE_FILE << endl;*/

    cout << "===== Simulator configuration =====" << endl;
    cout << "BLOCKSIZE:             " << iBLOCKSIZE << endl;
    cout << "L1_SIZE:               " << iL1_SIZE << endl;
    cout << "L1_ASSOC:              " << iL1_ASSOC << endl;
    cout << "L2_SIZE:               " << iL2_SIZE << endl;
    cout << "L2_ASSOC:              " << iL2_ASSOC << endl;
    cout << "REPLACEMENT POLICY:    " << sREPLACEMENT_POLICY << endl;
    cout << "INCLUSION PROPERTY:    " << sINCLUSION_PROPERTY<< endl;
    cout << "trace_file:            " << TRACE_FILE << endl;

    if(DEBUG)
    {
        dFILE << "===== Simulator configuration =====" << endl;
        dFILE << "BLOCKSIZE:             " << iBLOCKSIZE << endl;
        dFILE << "L1_SIZE:               " << iL1_SIZE << endl;
        dFILE << "L1_ASSOC:              " << iL1_ASSOC << endl;
        dFILE << "L2_SIZE:               " << iL2_SIZE << endl;
        dFILE << "L2_ASSOC:              " << iL2_ASSOC << endl;
        dFILE << "REPLACEMENT POLICY:    " << sREPLACEMENT_POLICY << endl;
        dFILE << "INCLUSION PROPERTY:    " << sINCLUSION_PROPERTY << endl;
        dFILE << "trace_file:            " << TRACE_FILE << endl;
    }

    if (iFILE.is_open())
    {
        //string input_line;
        int count = 1;
        while(!iFILE.eof())
        //while(true)
        {
            instruction = "";
            hex_address = "";
            iFILE >> instruction >> hex_address;
            if(instruction.size() == 0)
                break;
            if(DEBUG){
                dFILE << "----------------------------------------" << endl;
                dFILE << "# " << count << " : ";
            }
            if(instruction == "r")
            {
                if(DEBUG){
                    dFILE << "read " << hex_address << endl;
                }
                //Read()
                cout << "read instruction " << endl;
            }
            else if(instruction == "w")
            {
                if(DEBUG){
                    dFILE << "write " << hex_address << endl;
                }
                cout << "write instruction " << endl;
            }
            else
            {
                cout << "other instruction " << endl;
            }
            //cout << instruction << " " << hex_address << endl;
            count++;
            if(count == 10)
                break;
        }
        //cout << count << endl;
    }
    iFILE.close();
    //oFILE.close();
    dFILE.close();
/*
    string input_line, instruction, hex_address;
    int hex_address_int;

    while(getline(cin, input_line))
    {
        istringstream iss(input_line);
        iss >> instruction;
        iss >> hex_address;
        //hex_address.insert(hex_address.begin(), 8 - hex_address.length(), '0');
        cout << instruction << " and " << hex_address << endl;
        //L1Cache.
        cout << hex2dec(hex_address) << endl;
        cout << dec2hex(hex2dec(hex_address)) << endl;
    }
*/
    return 0;
}

string dec2hex(int dec){
    stringstream ss;
    ss << hex << dec;
    return ss.str();
}

int hex2dec(string hex){
    int dec;
    hex = "0x" + hex;
    stringstream ss;
    ss << std::hex << hex;
    ss >> dec;
    return dec;
}
