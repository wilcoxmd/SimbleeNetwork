//SimbleeNetwork attempts to provide a networking library based on
//the SimbleeCOM library. SimbleeCOM provides a reliable method for 
//point to point communication between two simblees, but as of yet no
//network topology has been defined, and there is no ability to target 
//specific nodes. The SimbleNetwork library attempts to provide a small 
//star network structure as well as functions for sending to specific nodes 
//or a group of nodes. 

//Currently, a network consists of one master node, and each master can support a group 
//of up to 5 children. 

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
//		12			  DATA_TYPE		//DATA_TYPE defines what type of message we will be handling
//		13			  DATA_BYTE_0	
//		14			  DATA_BYTE_1	//DATA_BYTE contains the packet data to be interepreted by the recipient

//TO DO: 
//1. test for larger packet sizes to hold more data. Currently based off of other 
//		Simblee examples, which use a 15 total byte packets. RFduinos have a 20byte packet size, so that may be another option.	
//2. Mesh Network structure definitions. 



#include "Arduino.h"

#ifndef SimbleeNetwork_h
#define SimbleeNetwork_h

#include <stddef.h>
#include <stdint.h>

//DATA_TYPE definitions
#define NORMAL_DATA 0
#define ACK 	1
#define ADDRESS_SEARCH 2
#define ADDRESS_RESPONSE 3

//DATA definitions
//could also put into a header for custom builds
//including LED commands here for convience for others.
#define LIGHT_ON 0x01
#define LIGHT_OFF 0x02

#define MAX_GROUP_SIZE 6

#define ACK_WAIT_TIMEOUT 2500 //test this to see if we get good responses. 


class SimbleeNetwork
{
	public: 

	//Constructor
	SimbleeNetwork();

	//starts SimbleeCOM library and establishes the ESN and SENDER_ID properties of this radio
	void begin();

	//stops using the simbleeCOM library for lower power consumption
	void end();

	//a function to join an existing network
	//will search for any existing networks using close proximity mode
	//and then join store the network IDs
	//NOTE: for use by light radios only
	void join();


	//function to send data to a specific target address
	//set target address to 0 to broadcast to all radios in the 
	//TO DO: return a bool success message to ensure message was transmitted
	bool send(uint16_t data, uint32_t targetAddress);

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

	//add new ESNs to our known network list
	int addESN(uint32_t ESN);

	//search for new addresses of nodes to add to our group.
	int searchForAddress(uint32_t targetAddress);

	//pull the SENDER_ID from a payload
	uint32_t findSender(const char* payload);

	//send our address in response to an address request
	int sendAddress(uint16_t data, uint32_t targetAddress);

	//check our known ESNs to see if we already know about this one
	bool isAddressKnown(uint32_t ESN);

	const uint32_t* sendToAllAddresses(uint16_t data);


	//variable to store this radio's ESN (ID)
	uint32_t myESN;

	//array stores all the ESNs that belong to a group. groupESNs[0] is reserved for the 
	//ESN of the master node and establishes the GROUP_ID
	//TO DO: put this in EEPROM so that we don't lose addresses on a reset
	uint32_t groupESNs[MAX_GROUP_SIZE] = {0,0,0,0,0,0};

	//array stores missed acks each time we send a broadcast message. A pointer to this array 
	//is returned by sendToAllAdresses() so that we can know which nodes 
	//did not receive our message
	uint32_t missedAcks[MAX_GROUP_SIZE] = {0,0,0,0,0,0};
  

	bool iAmMaster = false;

	volatile bool ACKed = false;
	volatile uint32_t ACKedBy_Address;

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