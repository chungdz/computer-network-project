#define FILE_PATH "Verification_Database.txt"
#define QUERY_PATH "query.txt"
#define ACC_PER ((unsigned short)0xFFF8)
#define NOT_PAID ((unsigned short)0xFFF9)
#define NOT_EXIST ((unsigned short)0xFFFA)
#define ACCESS_OK ((unsigned short)0xFFFB)

typedef struct{
    unsigned int phone_number;
    unsigned char tech;
    int paid;
}RegisterInfo;

typedef struct{
    unsigned int phone_number;
    unsigned char tech;
}Query;

int load_data(RegisterInfo* database, int max_length);
int load_query(Query* querylist, int max_length);
unsigned short search_data(RegisterInfo* database, int max_len, Query query);