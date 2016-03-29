#include "SimbleeNetwork.h"
#include "SimbleeCOM.h"

#define TARGET_ID 0xD35EF111
//#define TARGET_ID 0xD35EF11E 	//the real id:
SimbleeNetwork::SimbleeNetwork(int pin)
{
	pinMode(pin, OUTPUT);
	myESN = SimbleeCOM.getESN();
	SENDER_ID_0 = myESN & 0x000000FF;
	SENDER_ID_1 = (myESN & 0X0000FF00) >> 8;
	SENDER_ID_2 = (myESN & 0X00FF0000) >> 16;
	SENDER_ID_3 = (myESN & 0XFF000000) >> 24;


}	


void SimbleeNetwork::begin()
{

}

void SimbleeNetwork::join()
{

}

int SimbleeNetwork::send(uint16_t data, uint32_t targetAddress)
{
	char TARGET_ID_0 = TARGET_ID & 0x000000FF;
	char TARGET_ID_1 = (TARGET_ID & 0X0000FF00) >> 8;
	char TARGET_ID_2 = (TARGET_ID & 0X00FF0000) >> 16;
	char TARGET_ID_3 = (TARGET_ID & 0XFF000000) >> 24;

	char payload[] = {SENDER_ID_0, SENDER_ID_1, SENDER_ID_2, SENDER_ID_3, TARGET_ID_0, TARGET_ID_1, TARGET_ID_2, TARGET_ID_3};
	SimbleeCOM.send(payload, sizeof(payload));
	return 100;
}

bool SimbleeNetwork::checkAddress(const char* payload)
{
	uint32_t target_address = 0;
	target_address |= (payload[7] << 24);
	target_address |= (payload[6] << 16);
	target_address |= (payload[5] << 8);
	target_address |= (payload[4] 	  );

	if (target_address == myESN)
	{
		return 1;
	}
	else 
	{
		return 0;
	}


}







