#define FILE_PATH "Verification_Database.txt"
#define QUERY_PATH "query.txt"
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