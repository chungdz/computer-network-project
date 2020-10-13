#include "message.h"
#include<stddef.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void packing(Message info, char* send_buf){

    // printf("total %d\n", info.total_length);
    // package = (char*) malloc(info.total_length * sizeof(char));
    // if(package == NULL){
    //     printf("allocate memory fail\n");
    //     exit(0);
    // }

    char* p = send_buf;
    *((short*) p) = info.start_id;
    p += 2;
    *p = info.client_id;
    p += 1;
    *((short*) p) = info.data_type;
    p += 2;

    if(info.data_type == MSG_DATA){
        *p = info.sequence_number;
        p += 1;
        *p = info.length;
        p += 1;
        memcpy(p, info.message, info.length);
        p += info.length;
    }
    else if(info.data_type == MSG_ACK){
        *p = info.sequence_number;
        p += 1;
    }
    else if(info.data_type == MSG_REJECT){
        *((short*) p) = info.error_type;
        p += 2;
        *p = info.sequence_number;
        p += 1;
    }

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

Message new_data(char* mesg, char sequence_num){
    Message cur_pack;
    cur_pack.start_id = START_PACKAGE;
    cur_pack.client_id = CLIENT_ID;
    cur_pack.data_type = MSG_DATA;
    cur_pack.sequence_number = sequence_num;

    int real_length = 0;
    char *tmp = mesg;
    while(tmp[real_length] != '\0'){
        real_length++;
    }
    real_length += 1;
    // printf("real length: %d\n", real_length);
    cur_pack.length = real_length;
    cur_pack.message = mesg;
    cur_pack.end_id = END_PACKAGE;

    if(cur_pack.data_type == MSG_DATA){cur_pack.total_length = 9 + cur_pack.length;}
    else if(cur_pack.data_type == MSG_ACK){cur_pack.total_length = 8;}
    else if(cur_pack.data_type == MSG_REJECT){cur_pack.total_length = 10;}
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

    if(cur_pack.data_type == MSG_DATA){
        cur_pack.sequence_number = *p;
        p += 1;
        cur_pack.length = *p;
        p += 1;

        cur_pack.message = p;
        int real_length = 0;
        char *tmp = p;
        while(tmp[real_length] != '\0'){
            real_length++;
        }
        real_length += 1;
        // printf("real length: %d\n", real_length);

        if(real_length != cur_pack.length){
            cur_pack.error_type = LENGTH_MISMATCH;
            cur_pack.length = real_length;
            printf("length error\n");
        }
        p += cur_pack.length;
    }
    else if(cur_pack.data_type == MSG_ACK){
        cur_pack.sequence_number = *p;
        p += 1;
    }
    else if(cur_pack.data_type == MSG_REJECT){
        cur_pack.reject_sub_node = *((short*) p);
        p += 2;
        cur_pack.sequence_number = *p;
        p += 1;
    }

    cur_pack.end_id = *((short*) p);
    if(cur_pack.end_id != END_PACKAGE){
        cur_pack.error_type = END_OF_PACKET_MISSING;
    }

    return cur_pack;
}