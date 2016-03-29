//Simblee Network & Packet structure for Luna Lights

//Each Packet is 15 bytes and organized as follows:
// 
//  Byte #		|	  Purpose
//--------------------------------
//		0			  GROUP_ID_0		//GROUP_ID is defined by the 
//		1			  GROUP_ID_1		//ESN of the pressure sensor in each group
//		2			  GROUP_ID_2		
//		3			  GROUP_ID_3	
//		4			  SENDER_ID_0
//		5			  SENDER_ID_1		//SENDER_ID is defined by 
//		6			  SENDER_ID_2		//the ESN of the light or sensor sending the message
//		7			  SENDER_ID_3
//		8			  TARGET_ID_0
//		9			  TARGET_ID_1		//TARGET_ID is defined by 
//		10			  TARGET_ID_2		//the ESN of the intended message receipient
//		11			  TARGET_ID_3	
//		12			  REQUEST_TYPE		//REQUEST_TYPE defines what 
//		13			  REQUEST_DATA_0	type of message we will be handling
//		14			  REQUEST_DATA_1	//REQUEST_DATA contains the packet data to be interepreted by the recipient

#include "Arduino.h"

#ifndef SimbleeNetwork_h
#define SimbleeNetwork_h

#include <stddef.h>
#include <stdint.h>

#define MAX_PACKET_DATA_SIZE 2  //max number of bytes of packet data


class SimbleeNetwork
{
	public: 

	//Constructor
	SimbleeNetwork(int pin);

	//a function to begin a new network based on a sensor radio's ESN
	//NOTE: for use by controller radios only
	void begin();

	//a function to join an existing network
	//will search for any existing networks using close proximity mode
	//and then join store the network IDs
	//NOTE: for use by light radios only
	void join();


	//function to send data to a specific target address
	//set target address to 0 to broadcast to all radios in the group
	int send(uint16_t data, uint32_t targetAddress);

	//function to check for a target address match to our own address
	bool checkAddress(const char* payload);

	//variable to store this radio's ESN (ID)
	uint32_t myESN;



	private:

	char SENDER_ID_0;
	char SENDER_ID_1;
	char SENDER_ID_2;
	char SENDER_ID_3;
	
	char GROUP_ID_0;
	char GROUP_ID_1;
	char GROUP_ID_2;
	char GROUP_ID_3;



};




#endif