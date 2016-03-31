//Simblee Network & Packet structure for Luna Lights

//Each Packet is 15 bytes and organized as follows:
// 
//  Byte #		|	  Purpose
//--------------------------------
//		0			  GROUP_ID_0		//GROUP_ID is defined by the 
//		1			  GROUP_ID_1		//ESN of the master node in each group
//		2			  GROUP_ID_2		
//		3			  GROUP_ID_3	
//		4			  SENDER_ID_0
//		5			  SENDER_ID_1		//SENDER_ID is defined by 
//		6			  SENDER_ID_2		//the ESN of the node sending the message
//		7			  SENDER_ID_3
//		8			  TARGET_ID_0
//		9			  TARGET_ID_1		//TARGET_ID is defined by 
//		10			  TARGET_ID_2		//the ESN of the intended message receipient
//		11			  TARGET_ID_3	
//		12			  DATA_TYPE		//DATA_TYPE defines what 
//		13			  DATA_BYTE_0	type of message we will be handling
//		14			  DATA_BYTE_1	//DATA_BYTE contains the packet data to be interepreted by the recipient

//TO DO: 
//1. test for larger packet sizes to hold more data. Currently based off of other 
//		Simblee examples, which use a 15 total byte packet		


#include "Arduino.h"

#ifndef SimbleeNetwork_h
#define SimbleeNetwork_h

#include <stddef.h>
#include <stdint.h>

//DATA_TYPE definitions
#define ACK 	1



class SimbleeNetwork
{
	public: 

	//Constructor
	SimbleeNetwork();

	//starts SimbleeCOM library and establishes the ESN and SENDER_ID properties of this radio
	void begin();

	//a function to join an existing network
	//will search for any existing networks using close proximity mode
	//and then join store the network IDs
	//NOTE: for use by light radios only
	void join();


	//function to send data to a specific target address
	//set target address to 0 to broadcast to all radios in the 
	//TO DO: return a bool success message to ensure message was transmitted
	int send(uint16_t data, uint32_t targetAddress);

	//function to check for a target address match to our own address
	bool checkAddress(const char* payload);

	//function to determine DATA_TYPE from payload
	uint8_t readDataType(const char* payload);

	//function to read in received data
	uint16_t readData(const char* payload);

	//establish this radio as a master node. must call this function to start communications
	//for a group. Only one master node allowed per group. Master nodes
	//are responsible for managing known addresses within a group
	void setAsMaster();

	//function to send an ACK back to a node to acknowledge packet transmission
	bool sendACK(uint32_t targetAddress);

	//variable to store this radio's ESN (ID)
	uint32_t myESN;

	//array stores all the ESNs that belong to a group. groupESNs[0] is reserved for the 
	//ESN of the master node and establishes the GROUP_ID
	uint32_t groupESNs[6];


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