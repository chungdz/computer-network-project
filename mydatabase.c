#include <stdio.h>
#include <stdlib.h>
#include "mydatabase.h"

int load_data(RegisterInfo* database, int max_length){
    FILE *fp = fopen(FILE_PATH, "r");
    unsigned int number;
    unsigned int tech;
    int paid;
    int pointer = 0;
    while(fscanf(fp, "%u, %d, %d", &number, &tech, &paid) != EOF){
        // printf("%d\n", pointer);
        RegisterInfo* cur_p = database + pointer;
        cur_p->phone_number = number;
        cur_p->tech = (unsigned char)tech;
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
    unsigned int tech;
    int pointer = 0;
    while(fscanf(fp, "%u, %d", &number, &tech) != EOF){
        // printf("%d\n", pointer);
        Query* cur_p = querylist + pointer;
        cur_p->phone_number = number;
        cur_p->tech = (unsigned char)tech;
        pointer += 1;
        // printf("%u, %d\n", number, tech);
    }
    fclose(fp);
    return pointer;
}

unsigned short search_data(RegisterInfo* database, int max_len, Query query){
    // printf("%u, %d\n", query.phone_number, query.tech);
    unsigned short status = NOT_EXIST;
    for(int i = 0;i < max_len;i++){
        if(database[i].phone_number == query.phone_number && database[i].tech == query.tech){
            if(database[i].paid == 1){status = ACCESS_OK;}
            else{status = NOT_PAID;}
        }
    }
    return status;
}