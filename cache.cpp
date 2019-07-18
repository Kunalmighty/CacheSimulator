/*

Name: Kunal Baran Maiti, JHED ID: kmaiti1, CSF Spring 2019, Prog Assignment 6 (Cache Simulator)

cache.cpp - Cache Simulator for a file containing an abbreviated memory trace file.

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;


static unsigned long long int asstimestamp = 0;
static unsigned long long int settimestamp = 0;
static unsigned long long int blktimestamp = 0;
static unsigned long long int nwatimestamp = 0;
static unsigned long long int prftimestamp = 0;

static unsigned long long int dir[8192];
static unsigned long long int ass[8192][2];
static unsigned long long int set[2048][4][2];
static unsigned long long int blk[256][4][8];
static unsigned long long int blktimestamps[256][4];
static unsigned long long int nwa[256][4][8];
static unsigned long long int nwatimestamps[256][4];
static unsigned long long int prf[256][4][8];
static unsigned long long int prftimestamps[256][4];

static unsigned long long int output[6][2];


static void simDIR(unsigned long long int address) {    

    //Use the lower 13 bits of the memory access address as the index into the cache.
    unsigned short hash = 0;
    hash = address & 0x1FFF;

    //Update the cache on both loads and stores. 
    //Always replace the cache entry with the current address on a miss. 
    if (dir[hash] == address) {
        output[0][0]++; //increment hits
    }
    else {
        output[0][1]++; //increment misses
    }
    dir[hash] = address; 

}


static void simASS(unsigned long long int address) {

    //storing a time count of when the access last occurred
    //update on both hits and misses 
    asstimestamp++; 
    unsigned int smallest = ass[0][1]; 
    int counter = 0; 


    //For this model, update the cache on both loads and stores

    for (int i = 0; i < 8193; i++) { 

        //hit
        if (ass[i][0] == address) {
            output[1][0]++; //increment hits
            ass[i][0] = address;
            ass[i][1] = asstimestamp;
            return;
        }


    }

    //miss
    output[1][1]++; //increment misses 

    //Keep track of how recently each entry was used, 
    //and if you run out of space, 
    //replace the least recently used with the new entry. 
    for (int j = 0; j < 8192; j++) {

        if (ass[j][1] < smallest) {
            smallest = ass[j][1];
            counter = j;                    
        }

    }

    ass[counter][0] = address; 
    ass[counter][1] = asstimestamp;    

    
}



static void simSET(unsigned long long int address) {

    settimestamp++;

    //Use the lower 11 bits of the memory access address as the index to the set 
    unsigned short hash = address & 0x7FF;

    unsigned int smallest = set[0][0][1]; 
    smallest = set[hash][0][1];
    int counter = 0; 

    //hit
    for (int i = 0; i < 4; i++) {

        if (set[hash][i][0] == address) {
            output[2][0]++; //increment hits
            set[hash][i][1] = settimestamp; 
            return;
        }               

    }

    //miss
    output[2][1]++; //increment misses

    //checking if spot open
    for (int j = 0; j < 4; j++) {

        if (set[hash][j][0] == 0) {
            set[hash][j][0] = address; 
            set[hash][j][1] = settimestamp;   
            return;               
        }

    }

    //if spot not open, find spot with smallest timestamp
    for (int k = 0; k < 4; k++) {

        if (set[hash][k][1] < smallest) {
            smallest = set[hash][k][1];
            counter = k;                    
        }

    }

    set[hash][counter][0] = address; 
    set[hash][counter][1] = settimestamp;    

}



static void simBLK(unsigned long long int address) {

    blktimestamp++;

    //static unsigned long long int blk[256][4][8];
    //static unsigned long long int blktimestamps[256][4];

    //lower 3 bits = offset within the block 
    //next 8 bits = index to select the set
    //unsigned short offset = address & 0x7;
    unsigned short hash = (address >> 3) & 0xFF;

    unsigned int smallest = blktimestamps[hash][0];
    int counter = 0; 

    //hit
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            if (blk[hash][i][j] == address) {
                output[3][0]++; //increment hits
                blktimestamps[hash][i] = blktimestamp; 
                return;
            }   
        }    
    }

    //miss
    output[3][1]++; //increment misses

    //checking if spot open, use if it is
    for (int j = 0; j < 4; j++) {

        if (blk[hash][j][0] == 0) {

            blk[hash][j][0] = (address & 0xFFFFFFFFFFFFFFF8) | 0x0; 
            blk[hash][j][1] = (address & 0xFFFFFFFFFFFFFFF8) | 0x1; 
            blk[hash][j][2] = (address & 0xFFFFFFFFFFFFFFF8) | 0x2; 
            blk[hash][j][3] = (address & 0xFFFFFFFFFFFFFFF8) | 0x3;  
            blk[hash][j][4] = (address & 0xFFFFFFFFFFFFFFF8) | 0x4;  
            blk[hash][j][5] = (address & 0xFFFFFFFFFFFFFFF8) | 0x5; 
            blk[hash][j][6] = (address & 0xFFFFFFFFFFFFFFF8) | 0x6; 
            blk[hash][j][7] = (address & 0xFFFFFFFFFFFFFFF8) | 0x7; 
            blktimestamps[hash][j] = blktimestamp; 
            return;               
        }

    }

    //if spot not open, find spot with smallest timestamp and use it
    for (int k = 0; k < 4; k++) {

        if (blktimestamps[hash][k] < smallest) {
            smallest = blktimestamps[hash][k];
            counter = k;                    
        }

    }

    blk[hash][counter][0] = (address & 0xFFFFFFFFFFFFFFF8) | 0x0; 
    blk[hash][counter][1] = (address & 0xFFFFFFFFFFFFFFF8) | 0x1; 
    blk[hash][counter][2] = (address & 0xFFFFFFFFFFFFFFF8) | 0x2; 
    blk[hash][counter][3] = (address & 0xFFFFFFFFFFFFFFF8) | 0x3; 
    blk[hash][counter][4] = (address & 0xFFFFFFFFFFFFFFF8) | 0x4; 
    blk[hash][counter][5] = (address & 0xFFFFFFFFFFFFFFF8) | 0x5; 
    blk[hash][counter][6] = (address & 0xFFFFFFFFFFFFFFF8) | 0x6; 
    blk[hash][counter][7] = (address & 0xFFFFFFFFFFFFFFF8) | 0x7; 
    blktimestamps[hash][counter] = blktimestamp;   

}


static void simNWA(unsigned long long int address, char command) {

    nwatimestamp++;

    unsigned short hash = (address >> 3) & 0xFF;

    unsigned int smallest = nwatimestamps[hash][0];
    int counter = 0; 

    //hit
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            if (nwa[hash][i][j] == address) {
                output[4][0]++; //increment hits
                nwatimestamps[hash][i] = nwatimestamp; 
                return;
            }   
        }    
    }

    //miss
    output[4][1]++; //increment misses

    if (command == 'S') {
        return;
    }

    //checking if spot open, use if it is
    for (int j = 0; j < 4; j++) {

        if (nwa[hash][j][0] == 0) {

            nwa[hash][j][0] = (address & 0xFFFFFFFFFFFFFFF8) | 0x0; 
            nwa[hash][j][1] = (address & 0xFFFFFFFFFFFFFFF8) | 0x1; 
            nwa[hash][j][2] = (address & 0xFFFFFFFFFFFFFFF8) | 0x2; 
            nwa[hash][j][3] = (address & 0xFFFFFFFFFFFFFFF8) | 0x3;  
            nwa[hash][j][4] = (address & 0xFFFFFFFFFFFFFFF8) | 0x4;  
            nwa[hash][j][5] = (address & 0xFFFFFFFFFFFFFFF8) | 0x5; 
            nwa[hash][j][6] = (address & 0xFFFFFFFFFFFFFFF8) | 0x6; 
            nwa[hash][j][7] = (address & 0xFFFFFFFFFFFFFFF8) | 0x7; 
            nwatimestamps[hash][j] = nwatimestamp; 
            return;               
        }

    }

    //if spot not open, find spot with smallest timestamp and use it
    for (int k = 0; k < 4; k++) {

        if (nwatimestamps[hash][k] < smallest) {
            smallest = nwatimestamps[hash][k];
            counter = k;                    
        }

    }

    nwa[hash][counter][0] = (address & 0xFFFFFFFFFFFFFFF8) | 0x0; 
    nwa[hash][counter][1] = (address & 0xFFFFFFFFFFFFFFF8) | 0x1; 
    nwa[hash][counter][2] = (address & 0xFFFFFFFFFFFFFFF8) | 0x2; 
    nwa[hash][counter][3] = (address & 0xFFFFFFFFFFFFFFF8) | 0x3; 
    nwa[hash][counter][4] = (address & 0xFFFFFFFFFFFFFFF8) | 0x4; 
    nwa[hash][counter][5] = (address & 0xFFFFFFFFFFFFFFF8) | 0x5; 
    nwa[hash][counter][6] = (address & 0xFFFFFFFFFFFFFFF8) | 0x6; 
    nwa[hash][counter][7] = (address & 0xFFFFFFFFFFFFFFF8) | 0x7; 
    nwatimestamps[hash][counter] = nwatimestamp;   

}

static void prefetch(unsigned long long int address) {

    unsigned short hash2 = ((address) >> 3) & 0xFF;

    unsigned long long int smallest = prftimestamps[hash2][0];
    int counter = 0; 

    
    //check if prefetch block is already in set[hash2]
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            if (prf[hash2][i][j] == address) {                        
                prftimestamps[hash2][i] = prftimestamp; 
                return;
            }   
        }    
    }

    //if it is not already in the cache, find an open spot
    for (int k = 0; k < 4; k++) {

        if (prf[hash2][k][0] == 0) {

            prf[hash2][k][0] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x0; 
            prf[hash2][k][1] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x1; 
            prf[hash2][k][2] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x2; 
            prf[hash2][k][3] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x3;  
            prf[hash2][k][4] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x4;  
            prf[hash2][k][5] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x5; 
            prf[hash2][k][6] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x6; 
            prf[hash2][k][7] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x7; 
            prftimestamps[hash2][k] = prftimestamp; 
            return;         
        }
    } 

    //if spot not open, find spot with smallest timestamp and use it
    for (int k = 0; k < 4; k++) {

        if (prftimestamps[hash2][k] < smallest) {
            smallest = prftimestamps[hash2][k];
            counter = k;                    
        }

    }

    prf[hash2][counter][0] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x0; 
    prf[hash2][counter][1] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x1; 
    prf[hash2][counter][2] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x2;  
    prf[hash2][counter][3] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x3;   
    prf[hash2][counter][4] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x4; 
    prf[hash2][counter][5] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x5;
    prf[hash2][counter][6] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x6; 
    prf[hash2][counter][7] = ((address) & 0xFFFFFFFFFFFFFFF8) | 0x7;
    prftimestamps[hash2][counter] = prftimestamp; 


}



static void simPRF(unsigned long long int address, char command) {

    prftimestamp++;

    unsigned short hash = (address >> 3) & 0xFF;

    unsigned long long int smallest = prftimestamps[hash][0];
    int counter = 0; 

    //hit
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            if (prf[hash][i][j] == address) {
                output[5][0]++; //increment hits
                prftimestamps[hash][i] = prftimestamp; 
                return;
            }   
        }    
    }

    //miss
    output[5][1]++; //increment misses

    if (command == 'S') {
        return;
    }

    //checking if spot open, use if it is
    for (int j = 0; j < 4; j++) {

        if (prf[hash][j][0] == 0) {

            prf[hash][j][0] = (address & 0xFFFFFFFFFFFFFFF8) | 0x0; 
            prf[hash][j][1] = (address & 0xFFFFFFFFFFFFFFF8) | 0x1; 
            prf[hash][j][2] = (address & 0xFFFFFFFFFFFFFFF8) | 0x2; 
            prf[hash][j][3] = (address & 0xFFFFFFFFFFFFFFF8) | 0x3;  
            prf[hash][j][4] = (address & 0xFFFFFFFFFFFFFFF8) | 0x4;  
            prf[hash][j][5] = (address & 0xFFFFFFFFFFFFFFF8) | 0x5; 
            prf[hash][j][6] = (address & 0xFFFFFFFFFFFFFFF8) | 0x6; 
            prf[hash][j][7] = (address & 0xFFFFFFFFFFFFFFF8) | 0x7; 
            prftimestamps[hash][j] = prftimestamp; 
            prefetch(address+8); 
            return;               
        }

    }

    //if spot not open, find spot with smallest timestamp and use it
    for (int k = 0; k < 4; k++) {

        if (prftimestamps[hash][k] < smallest) {
            smallest = prftimestamps[hash][k];
            counter = k;                    
        }

    }

    prf[hash][counter][0] = (address & 0xFFFFFFFFFFFFFFF8) | 0x0; 
    prf[hash][counter][1] = (address & 0xFFFFFFFFFFFFFFF8) | 0x1; 
    prf[hash][counter][2] = (address & 0xFFFFFFFFFFFFFFF8) | 0x2; 
    prf[hash][counter][3] = (address & 0xFFFFFFFFFFFFFFF8) | 0x3; 
    prf[hash][counter][4] = (address & 0xFFFFFFFFFFFFFFF8) | 0x4; 
    prf[hash][counter][5] = (address & 0xFFFFFFFFFFFFFFF8) | 0x5; 
    prf[hash][counter][6] = (address & 0xFFFFFFFFFFFFFFF8) | 0x6; 
    prf[hash][counter][7] = (address & 0xFFFFFFFFFFFFFFF8) | 0x7; 
    prftimestamps[hash][counter] = prftimestamp; 
    prefetch(address+8); 

}


int main() {	             

    //initialize output array
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            output[i][j] = 0;
        }
    }

    //initialize ASS array
    for (int i = 0; i < 8192; i++) {
        for (int j = 0; j < 2; j++) {
            ass[i][j] = 0;
        }
    }

    //initialize SET array
    for (int i = 0; i < 2048; i++) {
        for (int j = 0; j < 4; j++) {
            set[i][j][0] = 0;
            set[i][j][1] = 0;
        }
    }

    //initialize BLK arrays
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 8; k++) {
                blk[i][j][k] = 0;                
            }
        }
    }
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 4; j++) {
            blktimestamps[i][j] = 0;
        }
    }

    //initialize NWA arrays
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 8; k++) {
                nwa[i][j][k] = 0;                
            }
        }
    }
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 4; j++) {
            nwatimestamps[i][j] = 0;
        }
    }

    //initialize PRF arrays
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 8; k++) {
                prf[i][j][k] = 0;                
            }
        }
    }
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 4; j++) {
            prftimestamps[i][j] = 0;
        }
    }

    unsigned long long int mem_address = 0;
    char flag;

    while (scanf("%llx %c", &mem_address, &flag) == 2) {

        simDIR(mem_address);
        simASS(mem_address);  
        simSET(mem_address);
        simBLK(mem_address);
        simNWA(mem_address, flag);
        simPRF(mem_address, flag);  
    }                

    printf("DIR: %20llu %20llu\n", output[0][0], output[0][1]); 
    printf("ASS: %20llu %20llu\n", output[1][0], output[1][1]); 
    printf("SET: %20llu %20llu\n", output[2][0], output[2][1]); 
    printf("BLK: %20llu %20llu\n", output[3][0], output[3][1]); 
    printf("NWA: %20llu %20llu\n", output[4][0], output[4][1]); 
    printf("PRF: %20llu %20llu\n", output[5][0], output[5][1]);
 
 	return 0;

}