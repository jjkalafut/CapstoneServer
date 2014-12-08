#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "MyForm.h"
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtppacket.h"
#include "..\..\CapstoneHeaders\pandaheader.h"
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

//alloe use of old function for simplicity


static RTPSession	sess;
char				devices_str[NUM_MAX_DEVICES][STR_BUFF_SZ];
RTPIPv4Address		devices_addr[NUM_MAX_DEVICES];
char				client_str[NUM_MAX_CLIENTS][STR_BUFF_SZ];
RTPIPv4Address		clients_addr[NUM_MAX_CLIENTS];
uint8_t				clients_map[NUM_MAX_CLIENTS][NUM_MAX_DEVICES]; //maps client idx to up to 10 device idx. uints are TRUE is sending there, FALSE if not
uint8_t				num_devices;
uint8_t				num_clients;
RTPIPv4Address	*	my_address;

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
		if (clients_map[pd->cl_array_idx][i] == TRUE){
			sess.AddDestination(devices_addr[i]);
		}
	}
	sess.SendPacket(&(pd->data), 1000); //todo ensure correct legth
}
void send_ack(uint8_t * idx){
	sess.SendPacketEx(idx, 1, PANDA_ACK, 0, 0);
}

void MyForm::set_address(uint32_t addr){
	//printf("setting IP to %i \n", addr);
	my_address = &RTPIPv4Address(addr, 6000);
}
void send_nack(uint32_t * addr){
	sess.SendPacketEx(addr, sizeof(RTPIPv4Address), PANDA_NACK, 0, 0);
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

				uint16_t idx = 0;
				uint8_t placed = FALSE;
				signupPacket fake;
				signupPacket * sup = &fake;
				// You can examine the data here
				//printf("Got packet !\n");
				switch (header){
				case AUDIO: { forwardAudio(pack); break; }

				case SET_NAME: {
					placed = FALSE;
					printf("Got a name captain... \n");
					sup = (signupPacket *)pack->GetPayloadData();
					uint8_t ret;
					for (idx = 0; idx < num_clients; idx++){
						if (strcmp(sup->name, client_str[idx]) == 0){
							placed = TRUE;
							ret = idx;
							break;
						}
					}
					if (!placed && num_clients < 10){
						strcpy_s(client_str[num_clients], STR_BUFF_SZ, sup->name);
						RTPIPv4Address tmp = RTPIPv4Address(sup->address, 6000);
						memcpy(&clients_addr[num_clients], &tmp, sizeof(RTPIPv4Address));
						ret = num_clients;
						num_clients++;
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
					sess.AddDestination(RTPIPv4Address(sup->address, 6000));
					send_ack(&ret);
					sess.DeleteDestination(RTPIPv4Address(sup->address, 6000));
					break;
					}

				case SET_DEV: {
					placed = FALSE;
					sup = (signupPacket *)pack->GetPayloadData();
					uint8_t ret;
					for (idx = 0; idx < num_devices; idx++){
						if (strcmp(sup->name, devices_str[idx]) == 0){
							placed = TRUE;
							ret = idx;
							break;
						}
					}
					if (!placed && num_devices < 10){
						strcpy_s(sup->name, STR_BUFF_SZ, devices_str[num_devices]);
						memcpy(&devices_addr[num_devices], &RTPIPv4Address(sup->address, 6000), sizeof(RTPIPv4Address));
						ret = num_devices;
						num_devices++;
					}

					this->Devices->BeginUpdate();
					this->Devices->DataSource = (gcnew cli::array< System::Object^  >(10) {
						gcnew String(&devices_str[0][0]),
						gcnew String(&devices_str[1][0]),
						gcnew String(&devices_str[2][0]),
						gcnew String(&devices_str[3][0]),
						gcnew String(&devices_str[4][0]),
						gcnew String(&devices_str[5][0]),
						gcnew String(&devices_str[6][0]),
						gcnew String(&devices_str[7][0]),
						gcnew String(&devices_str[8][0]),
						gcnew String(&devices_str[9][0]),
					});
					this->Devices->EndUpdate();
					sess.AddDestination(RTPIPv4Address(sup->address, 6000));
					send_ack(&ret);
					sess.DeleteDestination(RTPIPv4Address(sup->address, 6000));
					break;
					}
				case GET_DEV: {
					//printf("GOT ID GET DEV \n");
					devPacket devp;
					for (idx = 0; idx < NUM_MAX_DEVICES; idx++){
						if (idx < NUM_MAX_DEVICES){
							strcpy_s(&devp.devices[idx][0], STR_BUFF_SZ, &devices_str[idx][0]);
						}
						else{
							strcpy_s(&devp.devices[idx][0], 2, "\0");
						}
					}
					//printf("%i", (uint32_t *)pack->GetPayloadData());
					uint32_t temp_32;
					memcpy(&temp_32, (uint32_t*)pack->GetPayloadData(), 4);
					RTPIPv4Address tmp = RTPIPv4Address( temp_32, 6000);
					//printf(" %u \n", temp_32 );
					checkerror( sess.AddDestination(tmp));
					checkerror( sess.SendPacketEx(&devp, sizeof(devPacket), SET_DEV, 0, 0) );
					checkerror( sess.DeleteDestination(tmp) );
				}
				case SET_OUT: {
					
					selectPacket * sp;
					sp = (selectPacket *)pack->GetPayloadData();
					//printf(" setting outputs for client %u \n", sp->cl_array_idx);
					//saftey check
					if (sp->cl_array_idx < num_devices){
						uint8_t dev_idx;
						//printf("yep");
						for (dev_idx = 0; dev_idx < num_devices; dev_idx++){
							//printf(" device %u is %c", dev_idx, sp->selected[dev_idx]);
							memcpy_s(&clients_map[sp->cl_array_idx][dev_idx], 1, &(sp->selected[dev_idx]), 1);
							if (clients_map[sp->cl_array_idx][dev_idx] == 1){
								printf(" now selected device %u \n", dev_idx);
							}
						}
					}
					
					break;
				}
				default:{
					//printf("Can't ID Packet...");
				}
				}

				// we don't longer need the packet, so
				// we'll delete it				
				sess.DeletePacket(pack);
			}
		} while (sess.GotoNextSourceWithData());
	}

	sess.EndDataAccess();
}



bool getMyIP( RTPIPv4Address * myIP)
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
	struct in_addr addr;
	for (int i = 0; host->h_addr_list[i] != 0; ++i) {
		
		memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
		printf( "%i, %i \n", inet_ntoa(addr));		
	}
	memcpy(myIP, &addr, sizeof(RTPIPv4Address));
	//Obtain the computer's IP
	//RTPIPv4Address addr( ((struct in_addr *)(host->h_addr))->S_un.S_addr, 6000);
	//memcpy(myIP, &addr, sizeof(RTPIPv4Address));
	//printf("%i", host->h_addr);
#ifdef WIN32
	WSACleanup();
#endif
	return true;
}


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	
	RTPIPv4Address ret_addr;
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2, 2), &dat);
#endif // WIN32

	//locals
	uint16_t portbase, destport;
	uint32_t destip;
	int status, i, idx;

	num_devices = 10;
	num_clients = 0;

	// Enabling Windows XP visual effects before any controls are created
	//MessageBox::Show("Oppening a Port to Listen...");
	//
	for (idx = 0; idx < NUM_MAX_DEVICES; idx++){
		sprintf_s(devices_str[idx], "device %i \0", idx);
		//strcpy_s(, 2, snprint);
	}

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

	checkerror(status);
	sess.SetDefaultPayloadType(0);
	checkerror(status);
	sess.SetDefaultMark(0);
	checkerror(status);
	sess.SetDefaultTimestampIncrement(1);
	checkerror(status);

	RTPIPv4Address addr(destip, destport);

	status = sess.AddDestination(addr);
	checkerror(status);
	
	//status = getMyIP( &ret_addr );
	//status = sess.SendPacketEx( &ret_addr, sizeof(ret_addr),0,0, 0, AVAILABLE, 0, 0 );
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