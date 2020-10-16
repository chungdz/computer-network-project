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

Message reply_data(Message received_data){
    Message cur_pack;
    cur_pack.start_id = START_PACKAGE;
    cur_pack.client_id = received_data.client_id;
    cur_pack.sequence_number = received_data.sequence_number;
    if(received_data.error_type == 0){
        cur_pack.data_type = MSG_ACK;
        cur_pack.error_type = 0;
    } 
    else{
        cur_pack.data_type = MSG_REJECT;
        cur_pack.error_type = received_data.error_type;
    }
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