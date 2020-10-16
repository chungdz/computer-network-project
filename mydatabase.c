#include <stdio.h>
#include <stdlib.h>
#include "mydatabase.h"

int load_data(RegisterInfo* database, int max_length){
    FILE *fp = fopen(FILE_PATH, "r");

    unsigned int number;
    unsigned char tech;
    int paid;
    int pointer = 0;
    while(fscanf(fp, "%u, %c, %d", &number, &tech, &paid) != EOF){
        // printf("%d\n", pointer);
        RegisterInfo* cur_p = database + pointer;
        cur_p->phone_number = number;
        cur_p->tech = tech;
        cur_p->paid = paid;
        pointer += 1;
        // printf("%u, %d, %d\n", number, tech, paid);
    }

    return pointer;
}