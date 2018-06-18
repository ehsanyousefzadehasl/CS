#include <iostream>
#include <fstream>
using namespace std;


void Input_Interface();
bool Look_Up(unsigned long long a);
bool find_address(unsigned long long b);
void insert_element(unsigned long long c, unsigned long long d);


#define Cache_Associativity 16
#define Offset_bits 6
#define index_bits  11


unsigned int hit = 0;
unsigned int miss = 0;
unsigned int Addresses_in_benchmark = 0;


struct Cache_Element{
bool valid_bit;
unsigned long long tag;
unsigned long age;
};


Cache_Element cache[1 << index_bits][Cache_Associativity] = {0};


int main()
{
    Input_Interface();
    cout << "Cache Size: " << (1 << index_bits)*(1 << Offset_bits)*(Cache_Associativity) << endl;
    cout << "Cache Block Size: " << (1 << Offset_bits) << endl;
    cout << "Total Benchmark addresses: " << Addresses_in_benchmark << endl;
    cout << "Hit: " << hit << endl;
    cout << "Miss: " << miss << endl;
    cout << "Hit Ratio: " << (float) hit/Addresses_in_benchmark << endl;
    return 0;
}


void Input_Interface()
{
  unsigned long long PC, adres;
  ifstream myfile("test.txt");
  if (myfile.is_open())
  {
    while ( myfile >> PC >> adres )
    {
        Addresses_in_benchmark++;
        if(Look_Up(adres))
        {
            hit++;
        }
        else
        {
            miss++;
        }
    }
    myfile.close();
  }
  else cout << "Unable to open file";
}



bool Look_Up(unsigned long long adr)
{
    if(find_address(adr))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool find_address(unsigned long long address)
{
    unsigned long long Index,Tag;
    Index = (address >> Offset_bits) & ((1<<index_bits) - 1);
    Tag = (address >> (Offset_bits + index_bits));
   /* int setcount = 1 << index_bits;
    int blocksize = 1 << Offset_bits;
    unsigned long long blockadd = address / blocksize;
    Index = blockadd % setcount;
    Tag = blockadd / setcount;*/

    for(int way = 0 ; way < Cache_Associativity ; way++)
    {
        if((cache[Index][way].valid_bit == true)&&(cache[Index][way].tag == Tag))
        {
            for(int column = 0 ; column < Cache_Associativity ; column++)
            {
                if(cache[Index][column].valid_bit == true && cache[Index][column].age<cache[Index][way].age)
                    cache[Index][column].age++;
            }
            cache[Index][way].age = 0;
            return true;
        }
    }

    insert_element(Index,Tag);
    return false;

}


void insert_element(unsigned long long index, unsigned long long Tag)
{
    for(int way = 0 ; way < Cache_Associativity ; way++)
    {
        if(cache[index][way].valid_bit == false)
        {
            cache[index][way].tag = Tag;
            cache[index][way].valid_bit = true;

            for(int column = 0 ; column < way ; column++)
            {
                cache[index][column].age++;
            }
            cache[index][way].age = 0;
            return;
        }
    }
            int way_to_insert=0;
            int maximum = 0;
            for(int column = 0 ; column < Cache_Associativity ; column++)
            {
                if(maximum < cache[index][column].age)
                {
                    maximum = cache[index][column].age;
                    way_to_insert = column;
                }
            }
            cache[index][way_to_insert].tag = Tag;

            for(int column = 0 ; column < Cache_Associativity ; column++)
            {
                cache[index][column].age++;
            }
            cache[index][way_to_insert].age = 0;
    }
