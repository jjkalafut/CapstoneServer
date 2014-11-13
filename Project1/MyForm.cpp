#include "MyForm.h"
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtppacket.h"
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

static RTPSession sess;
char client_str[10][30];
//
// This function checks if there was a RTP error. If so, it displays an error
// message and exists.
//

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
/********************************************************************
*						GET Packets to ID new Sources
*
*
*
********************************************************************/
void getSrcPackets(char* str, uint32_t str_buff_sz, bool* update){

	sess.BeginDataAccess();
	// check incoming packets
	if (sess.GotoFirstSourceWithData())
	{
		do
		{
			RTPPacket *pack;

			while ((pack = sess.GetNextPacket()) != NULL)
			{
				// You can examine the data here
				printf("Got packet !\n");

				// we don't longer need the packet, so
				// we'll delete it
				if (strlen((char*)pack->GetPayloadData()) < str_buff_sz){
					strcpy_s(str, 30, (char*)pack->GetPayloadData());
				}
				sess.DeletePacket(pack);
			}
		} while (sess.GotoNextSourceWithData());
	}

	sess.EndDataAccess();
}
void MyForm::checkSrc(){
	char tmp[30];
	bool upd;
	uint16_t idx, idx2, idx3;

	getSrcPackets(&tmp[0], 29, &upd);
	if (upd == TRUE){
		upd = FALSE;
		for (idx = 0; idx < 10; idx++){
			if (strcmp(tmp, client_str[idx])){
				upd = TRUE;
			}
		}
		if (upd == TRUE){
			for (idx2 = 0; idx2 < 10; idx2++){
				if (strcmp("", client_str[idx])){
					strcpy_s(client_str[idx], tmp);
					break;
				}
			}
		}
		this->Clients->Items->AddRange(gcnew cli::array< System::Object^  >(10) {
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
	}
	
}
[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
#ifdef WIN32
	WSADATA dat;
	WSAStartup(MAKEWORD(2, 2), &dat);
#endif // WIN32

	uint16_t portbase, destport;
	uint32_t destip;
	int status;
	// Enabling Windows XP visual effects before any controls are created
	MessageBox::Show("Oppening a Port to Listen...");
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//establish rtp protocols
	inet_pton(AF_INET, "192.168.1.27", &destip);
	destip = ntohl(destip);

	portbase = 6000;
	destport = 64;

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

	printf("\nSending packet \n");

	// send the packet
	status = sess.SendPacket((void *)"1234567890", 10, 0, false, 10);
	checkerror(status);



	//find all decives


	// Create the main window and run it
	Application::Run(gcnew MyForm());

	sess.BYEDestroy(RTPTime(10, 0), 0, 0);

#ifdef WIN32
	WSACleanup();
#endif // WIN32
	return 0;
}