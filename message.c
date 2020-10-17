#include "message.h"
#include<stddef.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void packing(Message info, char* send_buf){

    char* p = send_buf;
    *((short*) p) = info.start_id;
    p += 2;
    *p = info.client_id;
    p += 1;
    *((short*) p) = info.data_type;
    p += 2;
    *p = info.sequence_number;
    p += 1;
    *p = info.length;
    p += 1;
    *p = info.technology;
    p += 1;
    *((unsigned int*) p) = info.phone_number;
    p += 4;
    *((short*) p) = info.end_id;
    p += 2;

}

void check_package(char* package){
    int tlength = sizeof(package);
    for(int i = 0;i <= tlength;i++){
        printf("%x\n", package[i]);
    }
    printf("\n");
}

Message new_data(unsigned int phone_number, unsigned char tech, char sequence_num, unsigned short data_type){
    Message cur_pack;
    cur_pack.start_id = START_PACKAGE;
    cur_pack.client_id = CLIENT_ID;
    cur_pack.data_type = data_type;
    cur_pack.sequence_number = sequence_num;
    cur_pack.length = 14;
    cur_pack.phone_number = phone_number;
    cur_pack.technology = tech;
    cur_pack.end_id = END_PACKAGE;

    return cur_pack;
}

Message unpacking(char* package){
    char *p = package;

    Message cur_pack;
    cur_pack.error_type = 0;

    cur_pack.start_id  = *((short*) p);
    p += 2;
    cur_pack.client_id = *p;
    p += 1;
    cur_pack.data_type = *((short*) p);
    p += 2;
    cur_pack.sequence_number = *p;
    p += 1;
    cur_pack.length = *p;
    p += 1;
    cur_pack.technology = *p;
    p += 1;
    cur_pack.phone_number  = *((unsigned int*) p);
    p += 4;
    cur_pack.end_id = *((short*) p);
    if(cur_pack.end_id != END_PACKAGE){
        cur_pack.error_type = END_OF_PACKET_MISSING;
    }

    return cur_pack;
}

void break_message(unsigned char* buf, int error_type){
    if(error_type == 0){return;}
    else if(error_type == 1){
        // out of sequence
        unsigned char sequence_number = *(buf + 5);
        *(buf + 5) = sequence_number + 1;
    }
    else if(error_type == 2){
        // length mismatch
        unsigned char length = *(buf + 6);
        *(buf + 6) = length + 1;
    }
    else if(error_type == 3){
        unsigned char length = *(buf + 6);
        *(unsigned short*)(buf + 7 + length) = 0xFFF0;
    }
    else if(error_type == 4){
        unsigned char sequence_number = *(buf + 5);
        *(buf + 5) = sequence_number - 1;
    }
    return;
}

char* error_message(unsigned short error_code, char* emsg){
    switch(error_code){
        case OUT_OF_SEQUENCE: sprintf(emsg, "Out of Sequence"); break;
        case LENGTH_MISMATCH: sprintf(emsg, "Length mismatch"); break;
        case END_OF_PACKET_MISSING: sprintf(emsg, "End of packet missing"); break;
        case DUPLICATE_PACKET: sprintf(emsg, "Duplicate packet"); break;
    }
    return emsg;
}