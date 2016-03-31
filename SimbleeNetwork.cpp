#include "SimbleeNetwork.h"
#include "SimbleeCOM.h"

//#define TARGET_ID 0xD35EF11E
//#define TARGET_ID 0xD35EF11E 	//the real id

SimbleeNetwork::SimbleeNetwork()
{

}	

void SimbleeNetwork::begin()
{
	SimbleeCOM.begin();

	myESN = SimbleeCOM.getESN();

	SENDER_ID_0 = myESN & 0x000000FF;
	SENDER_ID_1 = (myESN & 0X0000FF00) >> 8;
	SENDER_ID_2 = (myESN & 0X00FF0000) >> 16;
	SENDER_ID_3 = (myESN & 0XFF000000) >> 24;
}

void SimbleeNetwork::join()
{

}

void SimbleeNetwork::setAsMaster()
{
	groupESNs[0] = myESN;

	GROUP_ID_0 = myESN & 0x000000FF;
	GROUP_ID_1 = (myESN & 0X0000FF00) >> 8;
	GROUP_ID_2 = (myESN & 0X00FF0000) >> 16;
	GROUP_ID_3 = (myESN & 0XFF000000) >> 24;

}

int SimbleeNetwork::send(uint16_t data, uint32_t targetAddress)
{
	char TARGET_ID_0 = targetAddress & 0x000000FF;
	char TARGET_ID_1 = (targetAddress & 0X0000FF00) >> 8;
	char TARGET_ID_2 = (targetAddress & 0X00FF0000) >> 16;
	char TARGET_ID_3 = (targetAddress & 0XFF000000) >> 24;

	//Place holder for now. can create diffrent data types later
	char DATA_TYPE = 0;

	char DATA_BYTE_0 = data & 0x00FF;
	char DATA_BYTE_1 = (data & 0xFF00) >> 8;

	//construct payload packet
	char payload[] = {GROUP_ID_0, GROUP_ID_1, GROUP_ID_2, GROUP_ID_3, SENDER_ID_0, SENDER_ID_1, SENDER_ID_2, SENDER_ID_3, TARGET_ID_0, TARGET_ID_1, TARGET_ID_2, TARGET_ID_3, DATA_TYPE, DATA_BYTE_0, DATA_BYTE_1};
	
	//send payload packet over simbleeCOM
	SimbleeCOM.send(payload, sizeof(payload));


}

bool SimbleeNetwork::sendACK(uint32_t targetAddress)
{

	char TARGET_ID_0 = targetAddress & 0x000000FF;
	char TARGET_ID_1 = (targetAddress & 0X0000FF00) >> 8;
	char TARGET_ID_2 = (targetAddress & 0X00FF0000) >> 16;
	char TARGET_ID_3 = (targetAddress & 0XFF000000) >> 24;

	//send ACK type message
	char DATA_TYPE = ACK;
	char DATA_BYTE_0 = 0;
	char DATA_BYTE_1 = 0;

	//construct ACK payload packet
	char payload[] = {GROUP_ID_0, GROUP_ID_1, GROUP_ID_2, GROUP_ID_3, SENDER_ID_0, SENDER_ID_1, SENDER_ID_2, SENDER_ID_3, TARGET_ID_0, TARGET_ID_1, TARGET_ID_2, TARGET_ID_3, DATA_TYPE, DATA_BYTE_0, DATA_BYTE_1};

	SimbleeCOM.send(payload, sizeof(payload));
}


bool SimbleeNetwork::checkAddress(const char* payload)
{
	//reconstruct target address from payload
	uint32_t target_address = 0;
	target_address |= (payload[11] << 24);
	target_address |= (payload[10] << 16);
	target_address |= (payload[9] << 8);
	target_address |= (payload[8] 	  );

	uint32_t sender_id = 0;
	sender_id |= (payload[7] << 24);
	sender_id |= (payload[6] << 16);
	sender_id |= (payload[5] << 8);
	sender_id |= (payload[4] 	  );

	uint8_t data_type = readDataType(payload);

	//check for our address or a braodcast message (address == 0)
	if ((target_address == myESN || target_address == 0) && (data_type != ACK))
	{	
		//send ACK here
		sendACK(sender_id);
		return 1;
	}
	else if (target_address == myESN && data_type == ACK) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

uint8_t SimbleeNetwork::readDataType(const char* payload)
{
	//reconstruct DATA_TYPE from packet
	uint8_t dataType = 0;
	dataType |= (payload[12] 	 );

	return dataType; 
}

uint16_t SimbleeNetwork::readData(const char* payload)
{
	//reconstruct data from packet
	uint16_t data = 0;
	data |= (payload[14] << 8);
	data |= (payload[13] 	 );

	return data; 
}

































