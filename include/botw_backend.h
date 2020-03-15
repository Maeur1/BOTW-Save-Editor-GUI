#ifndef _BOTW_BACKEND_H_
#define _BOTW_BACKEND_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    FLAGS_BOW = 0,
    FLAGSV_BOW = 1,
    RUPEES = 2,
    FLAGS_WEAPON = 3,
    ITEMS = 4,
    FLAGSV_SHIELD = 5,
    ITEMS_QUANTITY = 6,
    FLAGSV_WEAPON = 7,
    FLAGS_SHIELD = 8
} Mapping;

long locations[9];
int maxSize;

char itemName[60][40];
int quantItems[60];
int newQuantItems[60];
int numberOfItems;

int rupeeValue;
int rupees;

int numberOfWeapons;
int numberOfBows;
int numberOfShields;

int quantMod[60];
int new_quantMod[60];
int modNames[60];
int new_modNames[60];


int maxArrows;
int version;

FILE *fp;

int setFile();
void getData();
void writeFile();

#endif
