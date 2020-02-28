#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <cmath>
#include "Cache.h"

using namespace std;

#define OUTPUT_FILE "output.txt"
#define DEBUG_FILE "debug.txt"

bool DEBUG = true;

ifstream iFILE;
ofstream dFILE, oFILE;
map < unsigned int, vector < unsigned int > > TRACE_MAP;

string dec2hex(unsigned int dec);
unsigned int hex2dec(string hex);
void Read(Cache Caches[], int level, int total_level, unsigned int address, Block block, unsigned int rank_val);
void Write(Cache caches[], int level, int total_level, unsigned int address, unsigned int rank_val, bool dirty_bit_val);
int Search(Cache caches[], int level, int total_level, unsigned int tag, unsigned int index);
int Evict(Cache caches[], int level, int total_level, unsigned int index, unsigned int cur_trc_cnt);
void Replace(Cache cachesp[], int level, int total_level, unsigned int index, int way, Block block);
void UpdateRankArray(Cache caches[], int level, int total_level, unsigned int index, int way, int result, unsigned int rank_val);
unsigned int GetAddress(Cache caches[], int level, int total_level, unsigned int tag, unsigned int index);
int GetWayToPlaceOrReplace(Cache caches[], int level, int total_level, unsigned int index, unsigned int cur_trc_cnt);
void Invalidate(Cache caches[], int level, int total_level, unsigned int address);
string get_formatted_cache_contents(Cache caches[], int total_level);
string get_formatted_raw_results(Cache caches[], int total_level);

void show_help(string filename)
{
    cout << "Usage: " << filename << " <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <REPLACEMENT_POLICY> <INCLUSION_PROPERTY> <trace_file>" << endl;
}
unsigned int string2int(string s)
{
    unsigned int len = s.size();
    unsigned int res = 0;
    bool is_valid = true;
    for (int i=0; i<len; i++)
    {
        if (isdigit(s[i]))
            res = (res * 10 + s[i] - '0');
        else
            is_valid = false;
    }
    if (is_valid)
        return res;
    return -1;

}
bool validate_parameters(string BLOCKSIZE, string L1_SIZE, string L1_ASSOC, string L2_SIZE, string L2_ASSOC, string REPLACEMENT_POLICY, string INCLUSION_PROPERTY, char *TRACE_FILE)
{
    bool is_valid = true;

    unsigned int iBLOCKSIZE = string2int(BLOCKSIZE);
    unsigned int iL1_SIZE = string2int(L1_SIZE);
    unsigned int iL1_ASSOC = string2int(L1_ASSOC);
    unsigned int iL2_SIZE = string2int(L2_SIZE);
    unsigned int iL2_ASSOC = string2int(L2_ASSOC);
    unsigned int iREPLACEMENT_POLICY = string2int(REPLACEMENT_POLICY);
    unsigned int iINCLUSION_PROPERTY = string2int(INCLUSION_PROPERTY);

    if (iBLOCKSIZE <= 0)
    {
        cout << "ERROR: Invalid Blocksize (Should be positive integer value)" << endl;
        is_valid = false;
    }
    else if (ceil(log2(iBLOCKSIZE)) != floor(log2(iBLOCKSIZE)))
    {
        cout << "ERROR: Invalid Blocksize (Should be power of two)" << endl;
        is_valid = false;
    }
    if (iL1_SIZE <= 0)
    {
        cout << "ERROR: Invalid L1 size (Should be positive integer value)" << endl;
        is_valid = false;
    }
    if (iL1_ASSOC <= 0)
    {
        cout << "ERROR: Invalid L1 associativity (Should be positive integer value)" << endl;
        is_valid = false;
    }
    if (iL2_SIZE < 0)
    {
        cout << "ERROR: Invalid L2 size (Should be zero or positive integer value)" << endl;
        is_valid = false;
    }
    if (iL2_ASSOC < 0)
    {
        cout << "ERROR: Invalid L2 associativity (Should be zero or positive integer value)" << endl;
        is_valid = false;
    }
    if (iREPLACEMENT_POLICY < 0 || iREPLACEMENT_POLICY > 3)
    {
        cout << "ERROR: Invalid replacement policy (Should be a value between 0 to 2)" << endl;
        is_valid = false;
    }
    if (iINCLUSION_PROPERTY < 0 || iINCLUSION_PROPERTY > 1)
    {
        cout << "ERROR: Invalid inclusion property (Should be 0 or 1)" << endl;
        is_valid = false;
    }
    ifstream iFILE;
    iFILE.open(TRACE_FILE);

    if (!iFILE)
    {
        cout << "ERROR: " << TRACE_FILE << " not exists" << endl;
        is_valid = false;
    }
    iFILE.close();

    return is_valid;
}

int main(int argc, char **argv)
{
    if (argc != 9)
    {
        show_help(argv[0]);
        return 0;
    }
    if (validate_parameters(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]) == false)
    {
        show_help(argv[0]);
        return 0;
    }

    unsigned int iBLOCKSIZE = string2int(argv[1]);
    unsigned int iL1_SIZE = string2int(argv[2]);
    unsigned int iL1_ASSOC = string2int(argv[3]);
    unsigned int iL2_SIZE = string2int(argv[4]);
    unsigned int iL2_ASSOC = string2int(argv[5]);
    unsigned int iREPLACEMENT_POLICY = string2int(argv[6]);
    string sREPLACEMENT_POLICY = (iREPLACEMENT_POLICY == 0 ? "LRU" : (iREPLACEMENT_POLICY == 1 ? "FIFO" : "optimal"));
    unsigned int iINCLUSION_PROPERTY = string2int(argv[7]);
    string sINCLUSION_PROPERTY = (iINCLUSION_PROPERTY == 0 ? "non-inclusive" : "inclusive");
    char *TRACE_FILE = argv[8];
    int number_of_cache_levels = (iL2_SIZE == 0 ? 1 : 2);

    Cache *Caches;
    Caches = (Cache *)malloc(sizeof(Cache) * 2);
    Caches[0] = Cache(1, iL1_SIZE, iL1_ASSOC, iBLOCKSIZE, iREPLACEMENT_POLICY, iINCLUSION_PROPERTY);
    Caches[1] = Cache(2, iL2_SIZE, iL2_ASSOC, iBLOCKSIZE, iREPLACEMENT_POLICY, iINCLUSION_PROPERTY);

    //Cache L1Cache(1, iL1_SIZE, iL1_ASSOC, iBLOCKSIZE, iREPLACEMENT_POLICY, iINCLUSION_PROPERTY);
    //Cache L2Cache(2, iL2_SIZE, iL2_ASSOC, iBLOCKSIZE, iREPLACEMENT_POLICY, iINCLUSION_PROPERTY);

    string instruction, hex_address;
    unsigned int iAddress;

    if (iREPLACEMENT_POLICY == Optimal) {
        //Initialize rank by reading trace file
        iFILE.open(TRACE_FILE);
        if (iFILE.is_open())
        {
            unsigned int trc_count = 1;
            while (!iFILE.eof())
            {
                instruction = "";
                hex_address = "";

                iFILE >> instruction >> hex_address;
                iAddress = hex2dec(hex_address);
                int offset_width = (iBLOCKSIZE == 0 ? 0 : log2(iBLOCKSIZE));
                unsigned int iAddress_tag_ind_only = (iAddress >> offset_width) << offset_width;
                if (TRACE_MAP.count(iAddress_tag_ind_only) == 0) {
                    TRACE_MAP[iAddress_tag_ind_only] = vector <unsigned int> ();
                }
                TRACE_MAP[iAddress_tag_ind_only].push_back(trc_count);

                trc_count++;
                //if (trc_count == 10) break;
            }
        }
        iFILE.close();
    }

    iFILE.open(TRACE_FILE);
    oFILE.open(OUTPUT_FILE);
    dFILE.open(DEBUG_FILE);

    oFILE << "===== Simulator configuration =====" << endl;
    oFILE << "BLOCKSIZE:             " << iBLOCKSIZE << endl;
    oFILE << "L1_SIZE:               " << iL1_SIZE << endl;
    oFILE << "L1_ASSOC:              " << iL1_ASSOC << endl;
    oFILE << "L2_SIZE:               " << iL2_SIZE << endl;
    oFILE << "L2_ASSOC:              " << iL2_ASSOC << endl;
    oFILE << "REPLACEMENT POLICY:    " << sREPLACEMENT_POLICY << endl;
    oFILE << "INCLUSION PROPERTY:    " << sINCLUSION_PROPERTY<< endl;
    oFILE << "trace_file:            " << TRACE_FILE << endl;

    cout << "===== Simulator configuration =====" << endl;
    cout << "BLOCKSIZE:             " << iBLOCKSIZE << endl;
    cout << "L1_SIZE:               " << iL1_SIZE << endl;
    cout << "L1_ASSOC:              " << iL1_ASSOC << endl;
    cout << "L2_SIZE:               " << iL2_SIZE << endl;
    cout << "L2_ASSOC:              " << iL2_ASSOC << endl;
    cout << "REPLACEMENT POLICY:    " << sREPLACEMENT_POLICY << endl;
    cout << "INCLUSION PROPERTY:    " << sINCLUSION_PROPERTY<< endl;
    cout << "trace_file:            " << TRACE_FILE << endl;

    if (DEBUG)
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
        unsigned int trc_count = 1;
        unsigned int rank_val;
        while (!iFILE.eof())
        {
            instruction = "";
            hex_address = "";
            rank_val = trc_count;

            iFILE >> instruction >> hex_address;
            iAddress = hex2dec(hex_address);

            if (instruction.size() == 0)
                break;
            if (DEBUG) {
                dFILE << "----------------------------------------" << endl;
                dFILE << "# " << trc_count << " : ";
            }
            if (instruction == "r")
            {
                dFILE << "read " << dec2hex(iAddress) << endl;
                Read(Caches, 1, number_of_cache_levels, iAddress, Block(), rank_val);
            }
            else if (instruction == "w")
            {
                dFILE << "write " << dec2hex(iAddress) << endl;
                Write(Caches, 1, number_of_cache_levels, iAddress, rank_val, DIRTY);
            }
            else
            {
                cout << "other instruction " << endl;
            }
            trc_count++;
        }
    }
    
    string contents = get_formatted_cache_contents(Caches, number_of_cache_levels);
    string results = get_formatted_raw_results(Caches, number_of_cache_levels);

    cout << contents;
    cout << results;
    dFILE << contents;
    dFILE << results;
    oFILE << contents;
    oFILE << results;
    iFILE.close();
    oFILE.close();
    dFILE.close();

    return 0;
}

string dec2hex(unsigned int dec) {
    stringstream ss;
    ss << hex << dec;
    return ss.str();
}

unsigned int hex2dec(string hex) {
    unsigned int dec;
    hex = "0x" + hex;
    stringstream ss;
    ss << std::hex << hex;
    ss >> dec;
    return dec;
}

void Write(Cache caches[], int level, int total_level, unsigned int address, unsigned int rank_val, bool dirty_bit_val) {
    unsigned int cur_trc_cnt = rank_val;
    if (level > total_level) {
        caches[total_level - 1].BLOCK_TRANSFERS++;
        return;
    }
    caches[level - 1].ACCESSES++;
    caches[level - 1].WRITES++;

    rank_val = caches[level - 1].ACCESSES;

    unsigned int tag = caches[level - 1].GetTagBits(address);
    unsigned int index = caches[level - 1].GetIndexBits(address);

    dFILE << "L" << level << " write : " << dec2hex((address >> caches[level-1].BLOCK_OFFSET_BIT_WIDTH) << caches[level-1].BLOCK_OFFSET_BIT_WIDTH) << " (tag " << dec2hex(tag) << ", index " << index << ")" << endl;

    int way = Search(caches, level, total_level, tag, index);
    int search_result = -1;

    if (way > -1) {
        //hit
        dFILE << "L" << level << " hit" << endl;

        search_result = HIT;
        caches[level-1].SETS[index].BLOCKS[way].DIRTY_BIT = dirty_bit_val;

        UpdateRankArray(caches, level, total_level, index, way, search_result, rank_val);
        dFILE << "L" << level << " update " << (caches[level-1].REPLACEMENT_POLICY == 0 ? "LRU" : caches[level-1].REPLACEMENT_POLICY == 1 ? "FIFO" : caches[level-1].REPLACEMENT_POLICY == 2 ? "optimal" : "") << endl;
        dFILE << "L" << level << " set " << (dirty_bit_val == DIRTY ? "dirty" : "clean") << endl;
    }
    else {
        //miss
        dFILE << "L" << level << " miss" << endl;

        search_result = MISS;
        caches[level - 1].WRITE_MISSES++;
        way = Evict(caches, level, total_level, index, cur_trc_cnt);

        Block block = Block();
        Read(caches, level+1, total_level, address, block, rank_val);

        block.TAG = tag;
        Replace(caches, level, total_level, index, way, block);

        caches[level-1].SETS[index].BLOCKS[way].DIRTY_BIT = dirty_bit_val;
        UpdateRankArray(caches, level, total_level, index, way, search_result, rank_val);

        dFILE << "L" << level << " update " << (caches[level-1].REPLACEMENT_POLICY == 0 ? "LRU" : caches[level-1].REPLACEMENT_POLICY == 1 ? "FIFO" : caches[level-1].REPLACEMENT_POLICY == 2 ? "optimal" : "") << endl;
        dFILE << "L" << level << " set " << (dirty_bit_val == DIRTY ? "dirty" : "clean") << endl;
    }
}

void Read(Cache caches[], int level, int total_level, unsigned int address, Block block, unsigned int rank_val) {
    unsigned int cur_trc_cnt = rank_val;
    if (level > total_level) {
        caches[total_level - 1].BLOCK_TRANSFERS++;
        block.DIRTY_BIT = CLEAN;
        block.VALID_BIT = VALID;
        return;
    }

    caches[level - 1].ACCESSES++;
    caches[level - 1].READS++;

    rank_val = caches[level - 1].ACCESSES;

    unsigned int tag = caches[level - 1].GetTagBits(address);
    unsigned int index = caches[level - 1].GetIndexBits(address);

    dFILE << "L" << level << " read : " << dec2hex((address >> caches[level-1].BLOCK_OFFSET_BIT_WIDTH) << caches[level-1].BLOCK_OFFSET_BIT_WIDTH) << " (tag " << dec2hex(tag) << ", index " << index << ")" << endl;

    int way = Search(caches, level, total_level, tag, index);
    int search_result = -1;
    if (way > -1) {
        //hit
        dFILE << "L" << level << " hit" << endl;

        search_result = HIT;
        block = caches[level-1].SETS[index].BLOCKS[way];

        if (level > 1 && caches[level - 1 - 1].INCLUSION_PROPERTY == exclusive)
        {
            caches[level - 1].SETS[index].BLOCKS[way].VALID_BIT = INVALID;
        }
        block.DIRTY_BIT = CLEAN;

        UpdateRankArray(caches, level, total_level, index, way, search_result, rank_val);
        dFILE << "L" << level << " update " << (caches[level-1].REPLACEMENT_POLICY == 0 ? "LRU" : caches[level-1].REPLACEMENT_POLICY == 1 ? "FIFO" : caches[level-1].REPLACEMENT_POLICY == 2 ? "optimal" : "") << endl;
    }
    else {
        //miss
        dFILE << "L" << level << " miss" << endl;

        search_result = MISS;
        caches[level - 1].READ_MISSES++;
        if (level == 1 || caches[level - 1 - 1].INCLUSION_PROPERTY != exclusive) {
            way = Evict(caches, level, total_level, index, cur_trc_cnt);
        }

        Read(caches, level+1, total_level, address, block, rank_val);

        block.TAG = tag;
        if (level == 1 || caches[level - 1 - 1].INCLUSION_PROPERTY != exclusive) {
            Replace(caches, level, total_level, index, way, block);
            UpdateRankArray(caches, level, total_level, index, way, search_result, rank_val);
            dFILE << "L"<< level << " update " << (caches[level-1].REPLACEMENT_POLICY == 0 ? "LRU" : caches[level-1].REPLACEMENT_POLICY == 1 ? "FIFO" : caches[level-1].REPLACEMENT_POLICY == 2 ? "optimal" : "") << endl;
        }

    }
}

int Search(Cache caches[], int level, int total_level, unsigned int tag, unsigned int index) {
    for (int i=0; i<caches[level-1].ASSOC; i++) {
        if (caches[level-1].SETS[index].BLOCKS[i].VALID_BIT == VALID && caches[level-1].SETS[index].BLOCKS[i].TAG == tag)
            return i;
    }
    return -1;
}

int Evict(Cache caches[], int level, int total_level, unsigned int index, unsigned int cur_trc_cnt) {
    int way = GetWayToPlaceOrReplace(caches, level, total_level, index, cur_trc_cnt);

    if (caches[level-1].SETS[index].BLOCKS[way].VALID_BIT == VALID) {
        unsigned int address = GetAddress(caches, level, total_level, caches[level-1].SETS[index].BLOCKS[way].TAG, index);
        dFILE << "L" << level << " victim: " << dec2hex(address) << " (tag " << dec2hex(caches[level-1].SETS[index].BLOCKS[way].TAG) << ", index " << index << ", " << (caches[level-1].SETS[index].BLOCKS[way].DIRTY_BIT == DIRTY ? "dirty" : "clean") << ")" <<endl;
        if (caches[level-1].SETS[index].BLOCKS[way].DIRTY_BIT == DIRTY) {
            caches[level-1].WRITE_BACKS++;
            Write(caches, level+1, total_level, address, caches[level-1].SETS[index].RANK_ARRAY[way], DIRTY);
        }
        else {
            if (caches[level-1].INCLUSION_PROPERTY == exclusive)
			{
				Write(caches, level + 1, total_level, address, caches[level-1].SETS[index].RANK_ARRAY[way], CLEAN);
			}
        }
        if (level > 1) {
            Invalidate(caches, level - 1, total_level, address);
        }
    }
    else {
        dFILE << "L" << level << " victim: none" <<endl;
    }
    return way;
}
int GetWayToPlaceOrReplace(Cache caches[], int level, int total_level, unsigned int index, unsigned int cur_trc_cnt) {
    for (int i = 0; i < caches[level-1].ASSOC; i++) {
        if (caches[level-1].SETS[index].BLOCKS[i].VALID_BIT == INVALID)
            return i;
    }
    int r = 0;
    if (caches[level-1].REPLACEMENT_POLICY == Optimal) {
        int furthest = -1, furthest_i = -1;
        vector<int> furthest_addresses = vector<int>();

        for (int i = 0; i < caches[level-1].ASSOC; i++) {
            unsigned int temp_address = (GetAddress(caches, level, total_level, caches[level-1].SETS[index].BLOCKS[i].TAG, index));
            int k = -1;
            for (int j=0; j<TRACE_MAP[temp_address].size(); j++) {
                if (TRACE_MAP[temp_address][j] > cur_trc_cnt) {
                    k = TRACE_MAP[temp_address][j];
                    break;
                }
            }
            if (k == -1) {
                furthest_addresses.push_back(i);
                furthest = -1;
            }
            else {
                if (furthest_addresses.size() == 0) {
                    if (k > furthest) {
                        furthest = k;
                        furthest_i = i;
                    }
                }
            }
        }
        if (furthest_addresses.size() == 0 && furthest != -1) {
            r = furthest_i;
        }
        else if (furthest_addresses.size() == 1) {
            r = furthest_addresses[0];
        }
        else {
            r = furthest_addresses[0];
//            unsigned int min_rank = caches[level-1].SETS[index].RANK_ARRAY[furthest_addresses[0]];
//            int min_rank_i = furthest_addresses[0];
//            for (int i=1; i<furthest_addresses.size(); i++) {
//                if (min_rank < caches[level-1].SETS[index].RANK_ARRAY[furthest_addresses[i]]) {
//                    min_rank = caches[level-1].SETS[index].RANK_ARRAY[furthest_addresses[i]];
//                    min_rank_i = furthest_addresses[i];
//                }
//            }
//            r = min_rank_i;
        }
    }
    else {   //For LRU and FIFO
        for (int i = 0; i < caches[level-1].ASSOC; i++)
            if (caches[level-1].SETS[index].RANK_ARRAY[i] < caches[level-1].SETS[index].RANK_ARRAY[r])
                r = i;
    }
    return r;
}
unsigned int GetAddress(Cache caches[], int level, int total_level, unsigned int tag, unsigned int index) {
    unsigned int address = 0;
    address |= (tag << (caches[level-1].INDEX_BIT_WIDTH + caches[level-1].BLOCK_OFFSET_BIT_WIDTH));
    address |= (index << caches[level-1].BLOCK_OFFSET_BIT_WIDTH);
    return address;
}
void Invalidate(Cache caches[], int level, int total_level, unsigned int address) {
    int level_from = level + 1;
    while (level >= 1) {
        if (caches[level-1].INCLUSION_PROPERTY == inclusive) {
            unsigned int tag = caches[level - 1].GetTagBits(address);
            unsigned int index = caches[level - 1].GetIndexBits(address);
            int way = Search(caches, level, total_level, tag, index);
            int search_result = -1;
            if (way > -1) {
                search_result = HIT;
                caches[level-1].SETS[index].BLOCKS[way].VALID_BIT = INVALID;
                dFILE << "L" << level << " invalidated: " << dec2hex(address) << " (tag " << dec2hex(tag) << ", index " << index << ", " << (caches[level-1].SETS[index].BLOCKS[way].DIRTY_BIT == DIRTY ? "dirty" : "clean") << ")" <<endl;
                if (caches[level-1].SETS[index].BLOCKS[way].DIRTY_BIT == DIRTY) {
                    if (level_from == total_level) {
                        dFILE << "L" << level << " writeback to main memory directly" << endl;
                    }
                    Write(caches, level_from + 1, total_level, address, caches[level-1].SETS[index].RANK_ARRAY[way], DIRTY);
                }
            }
            else {
                search_result = MISS;
            }
            if (level == 1)
				return;
			else
				level--;
        }
        else {
            return;
        }
    }
}
void Replace(Cache caches[], int level, int total_level, unsigned int index, int way, Block block) {
    caches[level - 1].SETS[index].BLOCKS[way].VALID_BIT = VALID;
    caches[level - 1].SETS[index].BLOCKS[way].TAG = block.TAG;
    caches[level - 1].SETS[index].BLOCKS[way].DIRTY_BIT = block.DIRTY_BIT;
}
void UpdateRankArray(Cache caches[], int level, int total_level, unsigned int index, int way, int result, unsigned int rank_val) {
    if (caches[level - 1].REPLACEMENT_POLICY == FIFO) {
        if (result == MISS) {
            caches[level-1].SETS[index].RANK_ARRAY[way] = rank_val;
        }
    }
    else {
        caches[level-1].SETS[index].RANK_ARRAY[way] = rank_val;
    }
}
string get_formatted_cache_contents(Cache caches[], int total_level) {
    stringstream ss;
    string tag_string;
    int t;
    for (int i=0; i<total_level; i++) {
        ss << "===== L" << (i+1) << " contents =====\n";
        for (int j=0; j<caches[i].NUMBER_OF_SETS; j++) {
            int digits = j > 0 ? (int) log10 ((double) j) + 1 : 1;
            ss << "Set";
            t = 5;
            while (t--) ss << " ";
            ss << j << ":";
            t = 8 - digits - 1;
            while (t--) ss << " ";
            for (int k=0; k<caches[i].ASSOC; k++) {
                tag_string = dec2hex(caches[i].SETS[j].BLOCKS[k].TAG);
                ss << tag_string;
                if (caches[i].SETS[j].BLOCKS[k].DIRTY_BIT == DIRTY) {
                    ss << " D";
                }
                else {
                    ss << "  ";
                }
                t = 8 - tag_string.size();
                while (t--) ss << " ";
            }
            ss << "\n";
        }
    }
    return ss.str();
}
string get_formatted_raw_results(Cache caches[], int total_level) {
    stringstream ss;
    ss << "===== Simulation results (raw) =====\n";
    char list_char = 'a';
    for (int i=0; i<2; i++) {
        ss << (list_char++) << ". number of L" << (i+1) << " reads:        " << caches[i].READS << endl;
        ss << (list_char++) << ". number of L" << (i+1) << " read misses:  " << caches[i].READ_MISSES << endl;
        ss << (list_char++) << ". number of L" << (i+1) << " writes:       " << caches[i].WRITES << endl;
        ss << (list_char++) << ". number of L" << (i+1) << " write misses: " << caches[i].WRITE_MISSES << endl;
        if (total_level > i) {
            double miss_rate;
            if (i == 0) {
                miss_rate = (caches[i].READ_MISSES + caches[i].WRITE_MISSES)/(double)(caches[i].READS + caches[i].WRITES); //for L1
            }
            else {
                miss_rate = caches[i].READ_MISSES/(double)caches[i].READS;
            }
            ss << fixed;
            ss.precision(6);
            ss << (list_char++) << ". L" << (i+1) << " miss rate:              " << miss_rate << endl;
        }
        else {
            ss << (list_char++) << ". L" << (i+1) << " miss rate:              " << 0 << endl;
        }
        ss << (list_char++) << ". number of L" << (i+1) << " writebacks:   " << caches[i].WRITE_BACKS << endl;
    }
    ss << list_char++ << ". total memory traffic:      " << caches[total_level-1].BLOCK_TRANSFERS << endl;
    return ss.str();
}
