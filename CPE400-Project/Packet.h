/*******************************************************************************
Name: Justin Haghighi & David Hull
Due Date: 2018-12-05
Class: CPE 400.1001
Instructor: Dr. Sengupta
Purpose: This file is the header to the Packet class
*******************************************************************************/
#ifndef _PACKET_H
#define _PACKET_H

#define ACK_FLAG 0x01
#define CTRL_FLAG 0x02
#define FAIL_FLAG 0x04


class Packet {
private:
	int to;        // integer specifies the destination node's number
	int from;      // integer specifies the source node's number
	char flags;    // char is one byte; each bit represents a flag   
	int dataSize; // the size of the data string
	char* data;    // the contents of the data string
public:
	// Constructors
	Packet();
	Packet(int pTo, int pFrom, int pdataSize, char pFlags, char* pData);
	Packet(const Packet &copyPacket);

	// Destructor
	~Packet();

	// Accessor functions
	int getTo();
	int getFrom();
	char getFlags();
	int getDataSize();
	char* getData();

	// Mutator functions
	void setTo(int);
	void setFrom(int);
	void setManFlags(char); // manually set flags
	void setDataSize(int);
	void setData(char*, int); // int calls setDataSize 

	// Automatic flag manipulation
	/*************************************************************************
	Set flags
	*************************************************************************/
	// Set all flags
	void setAllFlags(); // 1111,1111 = 0xFF
	// Set ACK bit - bit 0 (right-most)
	void setACK(); // XXXX,XXX1
	// Set internal/control packet bit - bit 1
	void setCTRL(); // XXXX,XX1X
	// Set fail bit - bit 2
	void setFail(); // XXXX,X1XX
	// Set undefined bit - bit 3
	void setBit3(); // XXXX,1XXX
	// Set undefined bit - bit 4
	void setBit4(); // XXX1,XXXX
	// Set undefined bit - bit 5
	void setBit5(); // XX1X,XXXX
	// Set undefined bit - bit 6
	void setBit6(); // X1XX,XXXX
	// Set undefined bit - bit 7 (left-most)
	void setBit7(); // 1XXX,XXXX

	/*************************************************************************
	Clear flags
	*************************************************************************/
	// Clear all flags
	void clearAllFlags(); // 0000,0000 = 0x00
	// Clear ACK bit - bit 0 (right-most)
	void clearACK(); // XXXX,XXX0
	// Clear internal/control packet bit - bit 1
	void clearCTRL(); // XXXX,XX0X
	// Clear fail bit - bit 2
	void clearFail(); // XXXX,X0XX
	// Clear undefined bit - bit 3
	void clearBit3(); // XXXX,0XXX
	// Clear undefined bit - bit 4
	void clearBit4(); // XXX0,XXXX
	// Clear undefined bit - bit 5
	void clearBit5(); // XX0X,XXXX
	// Clear undefined bit - bit 6
	void clearBit6(); // X0XX,XXXX
	// Clear undefined bit - bit 7 (left-most)
	void clearBit7(); // 0XXX,XXXX

	// Miscellaneous utilities
	// Prints the packet's variables
	void printPacket();

	bool getFlag(char flag);

	//Deallocation of the data c_string taken out of the destructor.
	void delete_data();

};

#endif