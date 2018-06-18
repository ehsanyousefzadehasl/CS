// --------------------------
// Direct-Mapped Cache code
// By Ehsan Yousefzadeh
// HPCAN lab
// Sharif University
// --------------------------
#include <iostream>
#include <fstream>
using namespace std;

// Define your cache configuration here
#define CACHE_ENTRIES 		1024
#define CACHE_BLOCK_SIZE 	128
#define OFFSET_BITS 		6
#define INDEX_BITS 			10
#define TAG_BITS 			16

long long cache[CACHE_ENTRIES] 		= {0};
bool cacheValidBit[CACHE_ENTRIES] 	= {0};

long long hit  = 0;
long long miss = 0;

void interface();
bool lookUp(long long address);

int main(int argc, char** argv) {
    std::cout << "direct-mapped cache" << std::endl;
    std::cout << "addresses are produced randomly" << std::endl;
    interface();
    cout << "Hits: " << hit << endl;
    cout << "Misses: " << miss << endl;
    cout << fixed;
    std::cout << "cache hit rate: " << (float) hit / (miss + hit) << std::endl;
    return 0;
}

void interface()
{
    long long address;
    ifstream infile("input.txt", ifstream::in);
    while (infile >> address)
    {
        if(lookUp(address))
        {
            hit++;
        }
        else
        {
            miss++;
        }
    }
}

bool lookUp(long long address)
{
    unsigned long long offset;
    unsigned long long index;
    unsigned long long tag;

    offset = address & ((1 << OFFSET_BITS) - 1);
    index  = (address >> OFFSET_BITS) & ((1 << INDEX_BITS) - 1);
    tag    = address >> (OFFSET_BITS + INDEX_BITS);

    if(cache[index] == tag && cacheValidBit[index] == true)
    {
        return true;
    }
    else
    {
        cache[index] = tag;
        cacheValidBit[index] = true;
        return false;
    }
}
