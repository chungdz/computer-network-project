#define FILE_PATH "Verification_Database.txt"
typedef struct{
    unsigned int phone_number;
    unsigned char tech;
    int paid;
}RegisterInfo;

int load_data(RegisterInfo* database, int max_length);