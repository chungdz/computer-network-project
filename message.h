typedef struct{
    unsigned short start_id;
    unsigned char client_id;
    unsigned short data_type;
    unsigned char sequence_number;
    unsigned char length;
    unsigned short end_id;
    unsigned short error_type;
    unsigned short reject_sub_node;
    unsigned int total_length;
    char *message;
}Message;

#define START_PACKAGE ((unsigned short)0xFFFF)
#define END_PACKAGE ((unsigned short)0xFFFF)
#define MSG_DATA ((unsigned short)0xFFF1)
#define MSG_ACK ((unsigned short)0xFFF2)
#define MSG_REJECT ((unsigned short)0xFFF3)
#define OUT_OF_SEQUENCE ((unsigned short)0xFFF4)
#define LENGTH_MISMATCH ((unsigned short)0xFFF5)
#define END_OF_PACKET_MISSING ((unsigned short)0xFFF6)
#define DUPLICATE_PACKET ((unsigned short)0xFFF7)
#define DEFAULT_MSG_LEN 1024
#define CLIENT_ID  0

//packing struct into char* array
void packing(Message info, char *send_buf);
//unpacking char* array into struct
Message unpacking(char * package);
//check package content
void check_package(char* package);
//initialize the data message
Message new_data(char* mesg, char sequence_num);
