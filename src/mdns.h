#if !defined(MDNS)
#define MDNS

#include <unistd.h>

#define MDNS_PORT 5353
#define MDNS_GROUP "224.0.0.251"

typedef struct mdns_header
{
    uint16_t transaction_id;
    uint16_t flags;
    uint16_t question_count;
    uint16_t answer_count;
    uint16_t authority_count;
    uint16_t additional_count;
} mdns_header_t;

typedef struct mdns_question
{
    uint8_t* name;
    uint16_t type;
    uint16_t _class;
} mdns_question_t;

typedef struct mdns_answer
{
    uint8_t* name;
    uint16_t type;
    uint16_t _class;
    uint32_t ttl;
    uint16_t data_lenght;
    uint8_t * resource_record_data;    
} mdns_answer_t;

typedef struct mdns_authority
{
    uint8_t* name;
    uint16_t type;
    uint16_t _class;
    uint32_t ttl;
    uint16_t data_lenght;
    uint8_t * resource_record_data;    
} mdns_authority_t;

typedef struct mdns_additional
{
    uint8_t* name;
    uint16_t type;
    uint16_t _class;
    uint32_t ttl;
    uint16_t data_lenght;
    uint8_t * resource_record_data;    
} mdns_additional_t;

#endif // MDNS
