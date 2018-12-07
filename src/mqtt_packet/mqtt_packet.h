#ifndef __MQTT_PACKET_H__
#define __MQTT_PACKET_H__

#define MQTT_PACKET_LEN_BIT_NUM (7)
#define MQTT_PACKET_LEN_MASK    (1 << MQTT_PACKET_LEN_BIT_NUM)

#define MQTT_PACKET_MAX_LEN     (4)


enum errors
{
	MQTTPACKET_BUFFER_TOO_SHORT = -2,
	MQTTPACKET_READ_ERROR = -1,
	MQTTPACKET_READ_COMPLETE
};


typedef enum mqtt_packet_type
{
    MQTT_PACKET_TYPE_RESERVED    = 0,
    MQTT_PACKET_TYPE_CONNECT     = 1,
    MQTT_PACKET_TYPE_CONNACK     = 2,
    MQTT_PACKET_TYPE_PUBLISH     = 3,
    MQTT_PACKET_TYPE_PUBACK      = 4,
    MQTT_PACKET_TYPE_PUBREC      = 5,
    MQTT_PACKET_TYPE_PUBREL      = 6,
    MQTT_PACKET_TYPE_PUBCOMP     = 7,
    MQTT_PACKET_TYPE_SUBSCRIBE   = 8,
    MQTT_PACKET_TYPE_SUBACK      = 9,
    MQTT_PACKET_TYPE_UNSUBSCRIBE = 10,
    MQTT_PACKET_TYPE_UNSUBACK    = 11,
    MQTT_PACKET_TYPE_PINGREQ     = 12,
    MQTT_PACKET_TYPE_PINGRESP    = 13,
    MQTT_PACKET_TYPE_DISCONNECT  = 14,
    MQTT_PACKET_TYPE_MAX         = 15, 
} mqtt_packet_type_e; 

typedef struct mqtt_fix_head
{
    union
    {
        unsigned char byte;
        struct
        {
            unsigned char retain : 1;
            unsigned char qos : 2;
            unsigned char dup : 1;
            unsigned char type : 4;
        } bits;
    } mqtt_first_byte_u;
    unsigned char remaining_len[MQTT_PACKET_MAX_LEN];
} mqtt_fix_head_t;

#define MQTT_PROTO_NAME_LEN      (2)
#define MQTT_PROTO_NAME_SIZE     (4)
#define MQTT_PROTO_LEVEL         (4)

#define MQTT_STRING_LEN          (2)
#define MQTT_DATA_LEN            (2)
#define MQTT_QOS_LEN             (1)

#define MQTT_CONNECT_HEAD_INIT               \
    {{0, MQTT_PROTO_NAME_SIZE}, {'M', 'Q', 'T', 'T'}, \
        MQTT_PROTO_LEVEL, 0, 0}

typedef struct mqtt_connect_head
{
    unsigned char proto_len[MQTT_PROTO_NAME_LEN];
    unsigned char proto_name[MQTT_PROTO_NAME_SIZE];
    unsigned char proto_level;
    union
    {
        unsigned char connect_flag;
        struct
        {
            unsigned char reserved : 1;
            unsigned char clean_session : 1;
            unsigned char will_flag : 1;
            unsigned char will_qos : 2;
            unsigned char will_retain : 1;
            unsigned char psd_flag : 1;
            unsigned char user_name_flag : 1;
        } bits;
    } mqtt_connect_flag_u;
    unsigned short keep_alive;
} mqtt_connect_head_t;

typedef struct mqtt_connect_payload
{
    char *client_id;
    char *will_topic;
    char *will_msg;
    char *user_name;
    char *password;
} mqtt_connect_payload_t;

typedef struct mqtt_connect_opt
{
    mqtt_connect_head_t connect_head;
    mqtt_connect_payload_t connect_payload;
} mqtt_connect_opt_t;

typedef struct mqtt_publish_head
{
    char *topic;
	unsigned short topic_len;
	unsigned short packet_id;
} mqtt_publish_head_t;

typedef struct mqtt_publish_payload
{
    char *msg;
	size_t msg_len;
} mqtt_publish_payload_t;


typedef struct mqtt_publish_opt
{
    mqtt_publish_head_t publish_head;
	mqtt_publish_payload_t publish_payload;
    unsigned char dup;
    unsigned char qos;
    unsigned char retain;
} mqtt_publish_opt_t;

typedef struct mqtt_subscribe_head
{
    unsigned short packet_id;
} mqtt_subscribe_head_t;

typedef struct mqtt_subscribe_payload
{
    unsigned char count;
    char **topic;
	unsigned char qoss[0];
} mqtt_subscribe_payload_t;

typedef struct mqtt_subscribe_opt
{
	mqtt_subscribe_head_t subscribe_head;
	mqtt_subscribe_payload_t subscribe_payload;
} mqtt_subscribe_opt_t;


#define MQTT_CONNECT_OPT_INIT {{{0, MQTT_PROTO_NAME_SIZE}, {'M', 'Q', 'T', 'T'}, \
        MQTT_PROTO_LEVEL, 0, 0}, {NULL, NULL, NULL, NULL, NULL}}


//int mqtt_encode_len(unsigned char *buf, int len);


int mqtt_encode_fixhead(unsigned char *buf, unsigned char type, unsigned char dup, 
                                unsigned char qos, unsigned char retain, int remaining_len);

int mqtt_encode_connect(unsigned char *buf, int buf_len, mqtt_connect_opt_t *options);

int mqtt_encode_publish(unsigned char *buf, int buf_len, mqtt_publish_opt_t *options);
int mqtt_encode_subscribe(unsigned char *buf, int buf_len, mqtt_subscribe_opt_t *options);

int mqtt_encode_ping(unsigned char *buf, int buf_len);


int mqtt_decode_fixhead(unsigned char *buf, unsigned char *type, unsigned char *dup, 
                                unsigned char *qos, unsigned char *retain, int *remaining_len);


int mqtt_decode_publish(unsigned char *buf, int buf_len, mqtt_publish_opt_t *options);


#endif