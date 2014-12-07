#include "MyForm.h"
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtppacket.h"
#include "pandaheader.h"
#ifndef WIN32
	#include <netinet/in.h>
	#include <arpa/inet.h>
#else
	#include <winsock2.h>
#endif // WIN32
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace Project1;
using namespace jrtplib;

static RTPSession	sess;
char				devices_str[NUM_MAX_DEVICES][STR_BUFF_SZ];
RTPIPv4Address		devices_addr[NUM_MAX_DEVICES];
char				client_str[NUM_MAX_CLIENTS][STR_BUFF_SZ];
RTPIPv4Address		clients_addr[NUM_MAX_CLIENTS];
uint8_t				clients_map[NUM_MAX_CLIENTS][NUM_MAX_DEVICES]; //maps client idx to up to 10 device idx. uints are TRUE is sending there, FALSE if not
uint8_t				num_devices;
uint8_t				num_clients;

/*----------------------------------------------------------------------------
*
*					Check Errors 
*
-----------------------------------------------------------------------------*/
void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		Sleep(4000);
		exit(-1);
	}
}

void MyForm::send_packet(){
	printf("\nSending packet \n");
	checkerror( sess.SendPacket((void *)"1234567890", 10, 0, false, 10) );
}
void forwardAudio( RTPPacket * pack){
	int i;
	pandaPacketData * pd = (pandaPacketData *) (pack->GetPayloadData());
	sess.ClearDestinations();
	for (i = 0; i < 10; i++){
		if (clients_map[pd->dev_array_idx][i] == TRUE){
			sess.AddDestination(devices_addr[i]);
		}
	}
	sess.SendPacket(&(pd->data), 1000); //todo ensure correct legth
}
/********************************************************************
*						GET Packets to ID new Sources
*
*
*
********************************************************************/
void MyForm::pollPackets(){

	sess.BeginDataAccess();
	// check incoming packets
	if (sess.GotoFirstSourceWithData())
	{
		do
		{
			//TODO Here is all the code for packet switching based on headers. 
			RTPPacket *pack;

			while ((pack = sess.GetNextPacket()) != NULL)
			{
				uint16_t header;
				header = pack->GetExtensionID();
				// You can examine the data here
				//printf("Got packet !\n");
				switch (header){
				case AUDIO: forwardAudio(pack); break;

				case SET_NAME: 
					if (strlen((char*)pack->GetPayloadData()) < STR_BUFF_SZ){
						char tmp[STR_BUFF_SZ];
						uint16_t idx;
						uint8_t placed = FALSE;
						strcpy_s( tmp, strlen((char*)pack->GetPayloadData()), (char*)pack->GetPayloadData());
						for (idx = 0; idx < num_devices; idx++){
							if (strcmp(tmp, client_str[idx]) == 0){
								placed = TRUE;
								break;
							}
						}
						if (!placed && num_devices < 10){
							strcpy_s(tmp, strlen(tmp), client_str[num_devices]);
							num_devices++;
						}
						this->Clients->BeginUpdate();
						this->Clients->DataSource = (gcnew cli::array< System::Object^  >(10) {
							gcnew String(&client_str[0][0]),
							gcnew String(&client_str[1][0]),
							gcnew String(&client_str[2][0]),
							gcnew String(&client_str[3][0]),
							gcnew String(&client_str[4][0]),
							gcnew String(&client_str[5][0]),
							gcnew String(&client_str[6][0]),
							gcnew String(&client_str[7][0]),
							gcnew String(&client_str[8][0]),
							gcnew String(&client_str[9][0]),
						});
						this->Clients->EndUpdate();
					}
					break;
				}
				// we don't longer need the packet, so
				// we'll delete it
				
				sess.DeletePacket(pack);
			}
		} while (sess.GotoNextSourceWithData());
	}

	sess.EndDataAccess();
}
void MyForm::checkSrc(){


	
	
}

bool getMyIP(IPv4 * myIP)
{
	char szBuffer[1024];

#ifdef WIN32
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return false;
#endif


	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return false;
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return false;
	}

	//Obtain the computer's IP
	myIP->b1 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
	myIP->b2 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
	myIP->b3 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
	myIP->b4 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;

#ifdef WIN32
	WSACleanup();
#endif
	return true;
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	IPv4 ret_addr;
	ret_addr.b1 = 0;
	ret_addr.b2 = 0;
	ret_addr.b3 = 0;
	ret_addr.b4 = 0;
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2, 2), &dat);
#endif // WIN32

	//locals
	uint16_t portbase, destport;
	uint32_t destip;
	int status, i;
	// Enabling Windows XP visual effects before any controls are created
	//MessageBox::Show("Oppening a Port to Listen...");
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//establish rtp protocols
	inet_pton(AF_INET, "239.255.255.250", &destip);
	destip = ntohl(destip);

	portbase = 6000;
	destport = 6000;

	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;

	sessparams.SetOwnTimestampUnit(1.0 / 10.0);

	sessparams.SetAcceptOwnPackets(true);
	transparams.SetPortbase(portbase);
	status = sess.Create(sessparams, &transparams);
	checkerror(status);

	RTPIPv4Address addr(destip, destport);

	status = sess.AddDestination(addr);
	checkerror(status);
	
	status = getMyIP( &ret_addr );
	status = sess.SendPacketEx( &ret_addr, sizeof(ret_addr),0,0, 0, AVAILABLE, 0, 0 );
	//printf("\nSending packet \n");

	// send the packet
	//status = sess.SendPacket((void *)"1234567890", 10, 0, false, 10);
	//checkerror(status);

	//Initialize client strings
	for (i = 0; i < 10; i++){
		strcpy_s(client_str[i],"\0");
	}


	//find all decives


	// Create the main window and run it
	Application::Run(gcnew MyForm());

	sess.BYEDestroy(RTPTime(10, 0), 0, 0);

#ifdef WIN32
	WSACleanup();
#endif // WIN32
	return 0;
}