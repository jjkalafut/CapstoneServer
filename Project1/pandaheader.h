#define GET_DEV		0x0001
#define SET_DEV		0x0002
#define GET_NAME	0x0003
#define SET_NAME	0x0004 //client tells server to set its name
#define SET_OUT		0x0005
#define SET_ACK		0x0006
#define SET_NACK	0x0007
#define AUDIO		0x0008
#define AVAILABLE	0x0009

#define NUM_MAX_CLIENTS 10
#define NUM_MAX_DEVICES 10
#define STR_BUFF_SZ		30

struct IPv4
{
	unsigned char b1, b2, b3, b4;
};

struct pandaPacketData
{
	char		dev_array_idx; //gotten from the responce of client
	char		pack[3];
	uint8_t		data[1000];
};//keep size under 1500