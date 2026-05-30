#include "bitstruct.h"
#include <stdlib.h>

static const int PERIODS[] = {
    0,
    1, 1,
    2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

static const int GROUPS[119] = {
    0,
    1, 8,
    1, 2, 3, 4, 5, 6, 7, 8,
    1, 2, 3, 4, 5, 6, 7, 8,
    1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 1, 2, 3, 4, 5, 6, 7, 8,
    1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 1, 2, 3, 4, 5, 6, 7, 8,
    1, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 5, 6, 7, 8, 8, 8, 1, 2, 3, 4, 5, 6, 7, 8,
    1, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 5, 6, 7, 8, 8, 8, 1, 2, 3, 4, 5, 6, 7, 8
};

static int getGroupFor(int atomicNumber) {
    if (atomicNumber < 1 || atomicNumber > 118) return -1;
    return GROUPS[atomicNumber];
}

static int getPeriodFor(int atomicNumber) {
    if (atomicNumber < 1 || atomicNumber > 118) return -1;
    return PERIODS[atomicNumber];
}

static int isMetal(int atomicNumber) {
    if (atomicNumber < 1 || atomicNumber > 118) return -1;
    static const int NON_METALS[] = {
        1, 2,
        5, 6, 7, 8, 9, 10,
        14, 15, 16, 17, 18,
        32, 33, 34, 35, 36,
        51, 52, 53, 54,
        84, 85, 86,
        117, 118
    };
    int count = sizeof(NON_METALS) / sizeof(NON_METALS[0]);
    for (int i = 0; i < count; i++) {
        if (atomicNumber == NON_METALS[i]) return 0;
    }
    return 1;
}

atom *init(int numberOfTable) {
    atom *result = (atom *)calloc(1, sizeof(atom));
    writeNumberOfTable(numberOfTable, result);
    writeGroup(getGroupFor(numberOfTable), result);
    writePeriod(getPeriodFor(numberOfTable), result);
    writeIsMetal(isMetal(numberOfTable), result);
    return result;
}

atom getAtomFromPointer(atom *atom) {
    return *atom;
}

void deinit(atom **me) {
    if (me && *me) {
        free(*me);
        *me = NULL;
    }
}

void writeNumberOfTable(int it, atom *forMe) {
    forMe->value = (forMe->value & ~NumberOfTableMask) | (it << NumberOfTableOffset);
}

void writeGroup(int it, atom *forMe) {
    forMe->value = (forMe->value & ~GroupMask) | (it << GroupOffset);
}

void writePeriod(int it, atom *forMe) {
    forMe->value = (forMe->value & ~PeriodMask) | (it << PeriodOffset);
}

void writeIsMetal(int it, atom *forMe) {
    forMe->value = (forMe->value & ~MetalMask) | (it << isMetalOffset);
}

uint8_t getNumberOfTable(const atom *from) {
    return (from->value & NumberOfTableMask) >> NumberOfTableOffset;
}

uint8_t getGroup(const atom *from) {
    return (from->value & GroupMask) >> GroupOffset;
}

uint8_t getPeriod(const atom *from) {
    return (from->value & PeriodMask) >> PeriodOffset;
}

uint8_t getIsMetal(const atom *from) {
    return (from->value & MetalMask) >> isMetalOffset;
}

void atomReconfigure(atom *me, int newNumberOfTable) {
    if (!me) return;
    writeNumberOfTable(newNumberOfTable, me);
    writeGroup(getGroupFor(newNumberOfTable), me);
    writePeriod(getPeriodFor(newNumberOfTable), me);
    writeIsMetal(isMetal(newNumberOfTable), me);
}

void printAtomInfo(const char *key, atom *value) {
    if (!value) return;
    printf("Key: %s -> Atom{ Z=%d, group=%d, period=%d, metal=%s }\n",
           key ? key : "NULL",
           getNumberOfTable(value),
           getGroup(value),
           getPeriod(value),
           getIsMetal(value) ? "yes" : "no");
}

void transmuteToGold(const char *key, atom *value) {
    (void)key;
    if (value) atomReconfigure(value, 79);
}

void transmuteToSilver(const char *key, atom *value) {
    (void)key;
    if (value) atomReconfigure(value, 47);
}
