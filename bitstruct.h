//
//  bitstruct.h
//  rgr
//
//  Created by Bakhtovar Akhmedov on 01.03.2026.
//

#ifndef bitstruct_h
#define bitstruct_h
#include <stdint.h>
#include <stdalign.h>

#include <stdio.h>
#define NumberOfTableOffset 9
#define GroupOffset 6
#define PeriodOffset 3
#define isMetalOffset 2
#define NumberOfTableMask (0b1111111 << NumberOfTableOffset) //1111.1110.0000.0000
#define GroupMask (0b111 << GroupOffset)                     //0000.0001.1100.0000
#define PeriodMask (0b111 << PeriodOffset)                   //0000.0000.0011.1000
#define MetalMask (0b1 << isMetalOffset)                     //0000.0000.0000.0100
typedef struct {
    uint16_t value;
} atom;

atom* init(int numberOfTable);
atom getAtomFromPointer(atom* atom);
void deinit(atom** me);
void writeNumberOfTable(int it, atom* forMe);
void writeGroup(int it, atom* forMe);
void writePeriod(int it, atom* forMe);
void writeIsMetal(int it, atom* forMe);

uint8_t getNumberOfTable(const atom* from);
uint8_t getGroup(const atom* from);
uint8_t getPeriod(const atom* from);
uint8_t getIsMetal(const atom* from);

void atomReconfigure(atom* me, int newNumberOfTable);

void printAtomInfo(const char* key, atom* value);
void transmuteToGold(const char* key, atom* value);
void transmuteToSilver(const char* key, atom* value);
#endif /* bitstruct_h */
