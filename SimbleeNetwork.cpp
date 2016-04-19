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
	SimbleeCOM.mode = LONG_RANGE;
    SimbleeCOM.txPowerLevel = +4; //default value is +4 (-20, -16, -12, -8, -4, 0, +4)

	myESN = SimbleeCOM.getESN();

	SENDER_ID_0 = myESN & 0x000000FF;
	SENDER_ID_1 = (myESN & 0X0000FF00) >> 8;
	SENDER_ID_2 = (myESN & 0X00FF0000) >> 16;
	SENDER_ID_3 = (myESN & 0XFF000000) >> 24;
}

void SimbleeNetwork::end()
{
	SimbleeCOM.end();
}


void SimbleeNetwork::join()
{

}

void SimbleeNetwork::setAsMaster()
{
	iAmMaster = true;
	groupESNs[0] = myESN;

	GROUP_ID_0 = myESN & 0x000000FF;
	GROUP_ID_1 = (myESN & 0X0000FF00) >> 8;
	GROUP_ID_2 = (myESN & 0X00FF0000) >> 16;
	GROUP_ID_3 = (myESN & 0XFF000000) >> 24;

}

bool SimbleeNetwork::send(uint16_t data, uint32_t targetAddress)
{
	char TARGET_ID_0 = targetAddress & 0x000000FF;
	char TARGET_ID_1 = (targetAddress & 0X0000FF00) >> 8;
	char TARGET_ID_2 = (targetAddress & 0X00FF0000) >> 16;
	char TARGET_ID_3 = (targetAddress & 0XFF000000) >> 24;

	char DATA_TYPE = NORMAL_DATA;

	char DATA_BYTE_0 = data & 0x00FF;
	char DATA_BYTE_1 = (data & 0xFF00) >> 8;

	//construct payload packet
	char payload[] = {GROUP_ID_0, GROUP_ID_1, GROUP_ID_2, GROUP_ID_3, SENDER_ID_0, SENDER_ID_1, SENDER_ID_2, SENDER_ID_3, TARGET_ID_0, TARGET_ID_1, TARGET_ID_2, TARGET_ID_3, DATA_TYPE, DATA_BYTE_0, DATA_BYTE_1};
	
	
	
	//send payload packet over simbleeCOM
	uint32_t startTime = millis();


	if (targetAddress > 0)
	{
		while((millis() - startTime < (ACK_WAIT_TIMEOUT+100)) && !ACKed)
		{
			Serial.print("message sending to...."); Serial.println(targetAddress, HEX);
			SimbleeCOM.send(payload, sizeof(payload));
			delay(25);  //need to delay here to create a listening period
				   		//setting this too low does not allow the radio to stop
				  		//long enough to pick up an ACK message. 
				   		//25ms tested to be the magic number
		}

		if (ACKed)
		{
			Serial.println("ACKed!");
			ACKed = false;
			return true;
		}
		else
		{
			Serial.println("Not Acked... :(");
			return false;
		}
	
	}
	if (targetAddress == 0)
	{	
		sendToAllAddresses(data);
	}
	


}
 
const uint32_t* SimbleeNetwork::sendToAllAddresses(uint16_t data)
{	
	int numberOfNodes = 0;	
	int numberOfAcks = 0;

	for (int i = 1; i < (MAX_GROUP_SIZE - 1); i++)
	{
		if (groupESNs[i] > 0)
		{
			numberOfNodes++;

			char TARGET_ID_0 = groupESNs[i] & 0x000000FF;
			char TARGET_ID_1 = (groupESNs[i] & 0X0000FF00) >> 8;
			char TARGET_ID_2 = (groupESNs[i] & 0X00FF0000) >> 16;
			char TARGET_ID_3 = (groupESNs[i] & 0XFF000000) >> 24;

			char DATA_TYPE = NORMAL_DATA;

			char DATA_BYTE_0 = data & 0x00FF;
			char DATA_BYTE_1 = (data & 0xFF00) >> 8;

			//construct payload packet
			char payload[] = {GROUP_ID_0, GROUP_ID_1, GROUP_ID_2, GROUP_ID_3, SENDER_ID_0, SENDER_ID_1, SENDER_ID_2, SENDER_ID_3, TARGET_ID_0, TARGET_ID_1, TARGET_ID_2, TARGET_ID_3, DATA_TYPE, DATA_BYTE_0, DATA_BYTE_1};

			uint32_t startTime = millis();
			while((millis() - startTime < (ACK_WAIT_TIMEOUT+100)) && !ACKed)
			{
				SimbleeCOM.send(payload, sizeof(payload));
				delay(25);  //need to delay here to create a listening period
					   		//setting this too low does not allow the radio to stop
					  		//long enough to pick up an ACK message. 
					   		//25ms tested to be the magic number
			}
			if (ACKed)
			{
				Serial.println("ACKed!");
				numberOfAcks++;
				missedAcks[i] = 0;
				ACKed = false;
			}
			else
			{
				missedAcks[i] = groupESNs[i];
				Serial.print("Not Acked... :( by ");Serial.println(missedAcks[i],HEX);
			}
		}
	}
	return missedAcks;
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

	//reconstruct group id from payload
	uint32_t group_id = 0;
	group_id |= (payload[3] << 24);
	group_id |= (payload[2] << 16);
	group_id |= (payload[1] << 8);
	group_id |= (payload[0] 	  );

	//reconstruct who this message came from from payload
	uint32_t sender_id = 0;
	sender_id |= (payload[7] << 24);
	sender_id |= (payload[6] << 16);
	sender_id |= (payload[5] << 8);
	sender_id |= (payload[4] 	  );

	uint8_t data_type = readDataType(payload);

	//check for our address or a braodcast message (address == 0)
	if ((target_address == myESN || (target_address == 0 && group_id == groupESNs[0])) && (data_type == NORMAL_DATA))
	{	
		//send ACK here
		sendACK(sender_id);
		return 1;
	}
	else if (target_address == myESN && data_type == ACK) 
	{
		return 1;
	}
	else if ((target_address == myESN || target_address == 0) && data_type == ADDRESS_SEARCH && iAmMaster)
	{
		sendACK(sender_id);
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

int SimbleeNetwork::addESN(uint32_t ESN)
{
	for (int i = 0; i < MAX_GROUP_SIZE; i++)
	{
		if (groupESNs[i] == 0)
		{
			groupESNs[i] = ESN;
			//return which array index we're putting the new ESN into
			return i;
		}
	}
}


int SimbleeNetwork::searchForAddress(uint32_t targetAddress)
{
	//Send to target address
	char TARGET_ID_0 = targetAddress & 0x000000FF;
	char TARGET_ID_1 = (targetAddress & 0X0000FF00) >> 8;
	char TARGET_ID_2 = (targetAddress & 0X00FF0000) >> 16;
	char TARGET_ID_3 = (targetAddress & 0XFF000000) >> 24;

	//Place holder for now. can create diffrent data types later
	char DATA_TYPE = ADDRESS_SEARCH;

	char DATA_BYTE_0 = 0xAA;
	char DATA_BYTE_1 = 0;

	//construct payload packet
	char payload[] = {GROUP_ID_0, GROUP_ID_1, GROUP_ID_2, GROUP_ID_3, SENDER_ID_0, SENDER_ID_1, SENDER_ID_2, SENDER_ID_3, TARGET_ID_0, TARGET_ID_1, TARGET_ID_2, TARGET_ID_3, DATA_TYPE, DATA_BYTE_0, DATA_BYTE_1};
	
	//send payload packet over simbleeCOM
	SimbleeCOM.send(payload, sizeof(payload));
}

uint32_t SimbleeNetwork::findSender(const char* payload)
{
	uint32_t sender_id = 0;
	sender_id |= (payload[7] << 24);
	sender_id |= (payload[6] << 16);
	sender_id |= (payload[5] << 8);
	sender_id |= (payload[4] 	  );	

	return sender_id;
}

int SimbleeNetwork::sendAddress(uint16_t data, uint32_t targetAddress)
{
	char TARGET_ID_0 = targetAddress & 0x000000FF;
	char TARGET_ID_1 = (targetAddress & 0X0000FF00) >> 8;
	char TARGET_ID_2 = (targetAddress & 0X00FF0000) >> 16;
	char TARGET_ID_3 = (targetAddress & 0XFF000000) >> 24;

	//Place holder for now. can create diffrent data types later
	char DATA_TYPE = 3;

	char DATA_BYTE_0 = 0x51;
	char DATA_BYTE_1 = 0;

	//construct payload packet
	char payload[] = {GROUP_ID_0, GROUP_ID_1, GROUP_ID_2, GROUP_ID_3, SENDER_ID_0, SENDER_ID_1, SENDER_ID_2, SENDER_ID_3, TARGET_ID_0, TARGET_ID_1, TARGET_ID_2, TARGET_ID_3, DATA_TYPE, DATA_BYTE_0, DATA_BYTE_1};
	
	//send payload packet over simbleeCOM
	uint32_t startTime = millis();
	while((millis() - startTime < (ACK_WAIT_TIMEOUT+100)) && !ACKed)
	{
		SimbleeCOM.send(payload, sizeof(payload));
		delay(25); //need to delay here to create a listening period
				   //setting this too low does not allow the radio to stop
				   //long enough to pick up an ACK message. 
				   //25ms tested to be the magic number
		//ACKed will be set by our main file. A little sloppy, but couldn't move the
		//SimbleeCOM receive function into the library.
		if (ACKed)
  		{
	    	Serial.println("ACKed!");
	    	ACKed = false; //reset ack flag
	    	return true; //return true so that main file knows we were acked
  		}
  		else
  		{
	    	Serial.println("not ACKed.... :(");
  		}
	}
}

bool SimbleeNetwork::isAddressKnown(uint32_t ESN)
{
	bool addressIsKnown = false;

	for (int i = 0; i < MAX_GROUP_SIZE; i++)
	{
		if (groupESNs[i] == ESN)
		{
			addressIsKnown = true;
			break;
		}
	}

	return addressIsKnown;
}

































