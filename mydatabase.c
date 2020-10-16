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
    fclose(fp);
    return pointer;
}

int load_query(Query* querylist, int max_length){
    FILE *fp = fopen(QUERY_PATH, "r");
    unsigned int number;
    unsigned char tech;
    int pointer = 0;
    while(fscanf(fp, "%u, %c", &number, &tech) != EOF){
        // printf("%d\n", pointer);
        Query* cur_p = querylist + pointer;
        cur_p->phone_number = number;
        cur_p->tech = tech;
        pointer += 1;
        // printf("%u, %d\n", number, tech);
    }
    fclose(fp);
    return pointer;
}