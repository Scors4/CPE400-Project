/*******************************************************************************
Name: Justin Haghighi & David Hull
Due Date: 2018-12-05
Class: CPE 400.1001
Instructor: Dr. Sengupta
Purpose: This file implements the Packet class
*******************************************************************************/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <bitset>
#include "Packet.h"

// Default constructor
Packet::Packet() {
	to = 0;
	from = 0;
	flags = 0x00;
	dataSize = 0;
	data = NULL;
}

// Parameterized constructor
Packet::Packet(int pTo, int pFrom, int pDataSize, char pFlags, char* pData) {
	to = pTo;
	from = pFrom;
	flags = pFlags;
	dataSize = pDataSize;
	if (dataSize > 0)
	{
		data = new char[dataSize + 1];
		std::strcpy(data, pData);
	}
}

// Copy constructor
Packet::Packet(const Packet &copyPacket) {
	to = copyPacket.to;
	from = copyPacket.from;
	flags = copyPacket.flags;
	dataSize = copyPacket.dataSize;
	if (dataSize > 0)
	{
		data = new char[dataSize + 1];
		std::strcpy(data, copyPacket.data);
	}
}

// Destructor
Packet::~Packet() {
	to = 0;
	from = 0;
	flags = 0x00;
	if (dataSize > 0)
	{
		//data = nullptr;
		//dataSize = 0;
	}
}

// Implementation of accessor functions
int Packet::getTo() {
	return to;
}

int Packet::getFrom() {
	return from;
}

char Packet::getFlags() {
	return flags;
}

int Packet::getDataSize() {
	return dataSize;
}

char* Packet::getData() {
	return data;
}

// Implementation of mutator functions
void Packet::setTo(int pTo) {
	to = pTo;
}

void Packet::setFrom(int pFrom) {
	from = pFrom;
}

void Packet::setManFlags(char pFlags) {
	flags = pFlags;
}

void Packet::setDataSize(int pDataSize) {
	dataSize = pDataSize;
}

void Packet::setData(char* pData, int pDataSize) {
	setDataSize(pDataSize);
	data = new char[dataSize + 1];
	std::strcpy(data, pData);
}

// Auto flag manipulation implementation
/*******************************************************************************
 Set flags
*******************************************************************************/
void Packet::setAllFlags() {
	// Binary: 1111,1111
	// Hex: 11

	flags |= 0xFF; // sets all bits
}

// Sets a single bit according to what position of the binary is desired
void Packet::setFlagsBit(int bitPosition) {
	flags |= 1 << bitPosition;
}

void Packet::setACK() {
	// Binary: XXXX,XXX1
	setFlagsBit(0);
}

void Packet::setCTRL() {
	// Binary: XXXX,XX1X
	setFlagsBit(1);
}

void Packet::setFail() {
	// Binary: XXXX,X1XX
	setFlagsBit(2);
}

void Packet::setBit3() {
	// Binary: XXXX,1XXX
	setFlagsBit(3);
}

void Packet::setBit4() {
	// Binary: XXX1,XXXX
	setFlagsBit(4);
}

void Packet::setBit5() {
	// Binary: XX1X,XXXX
	setFlagsBit(5);
}

void Packet::setBit6() {
	// Binary: X1XX,XXXX
	setFlagsBit(6);
}

void Packet::setBit7() {
	// Binary: 1XXX,XXXX
	setFlagsBit(7);
}

/*******************************************************************************
 Clear flags
*******************************************************************************/
void Packet::clearAllFlags() {
	// Binary: 0000,0000
	// Hex: 00

	flags &= 0x00; // clears all bits
}

// Clears a single bit according to what position of the binary is desired
void Packet::clearFlagsBit(int bitPosition) {
	flags &= ~(1 << bitPosition);
}

void Packet::clearACK() {
	// Binary: XXXX,XXX0
	clearFlagsBit(0);
}

void Packet::clearCTRL() {
	// Binary: XXXX,XX0X
	clearFlagsBit(1);
}

void Packet::clearFail() {
	// Binary: XXXX,X0XX
	clearFlagsBit(2);
}

void Packet::clearBit3() {
	// Binary: XXXX,0XXX
	clearFlagsBit(3);
}

void Packet::clearBit4() {
	// Binary: XXX0,XXXX
	clearFlagsBit(4);
}

void Packet::clearBit5() {
	// Binary: XX0X,XXXX
	clearFlagsBit(5);
}

void Packet::clearBit6() {
	// Binary: X0XX,XXXX
	clearFlagsBit(6);
}

void Packet::clearBit7() {
	// Binary: 0XXX,XXXX
	clearFlagsBit(7);
}

/*******************************************************************************
Check flags
*******************************************************************************/
// This function is the base of all the flag set functions except allSet() and
// allClear(). It checks if the K-th bit is true and returns 1 if it is
// and 0 if it is not set.
bool Packet::checkFlagsBit(int position) {
	int mask;

	mask = (flags >> (position));
	if (mask & 1) {
		return 1;
	}
	else {
		return 0;
	}
}

// Checks if all flags are set: returns true if all set, false if not
bool Packet::allSet() {
	if (flags == (char)0xFF) { // flags is type char
		return 1;
	}
	else {
		return 0;
	}
}

// Checks if all flags are clear: returns true if all clear, false if not
bool Packet::allClear() {
	if (flags == (char)0x00) {
		return 1;
	}
	else {
		return 0;
	}
}

// Returns true if the flag ACK is set, false if it is clear
bool Packet::isACKSet() {
	bool result = checkFlagsBit(0);

	return result;
}

// Returns true if the flag CTRL is set, false if it is clear
bool Packet::isCTRLSet() {
	bool result = checkFlagsBit(1);

	return result;
}

// Returns true if the flag Fail is set, false if it is clear
bool Packet::isFailSet() {
	bool result = checkFlagsBit(2);

	return result;
}

// Returns true if the flag Bit3 is set, false if it is clear
bool Packet::isBit3Set() {
	bool result = checkFlagsBit(3);

	return result;
}

// Returns true if the flag Bit4 is set, false if it is clear
bool Packet::isBit4Set() {
	bool result = checkFlagsBit(4);

	return result;
}

// Returns true if the flag Bit5 is set, false if it is clear
bool Packet::isBit5Set() {
	bool result = checkFlagsBit(5);

	return result;
}

// Returns true if the flag Bit6 is set, false if it is clear
bool Packet::isBit6Set() {
	bool result = checkFlagsBit(6);

	return result;
}

// Returns true if the flag Bit7 is set, false if it is clear
bool Packet::isBit7Set() {
	bool result = checkFlagsBit(7);

	return result;
}

// Implementation of miscellaneous utilites 
// Prints the packet's variables
void Packet::printPacket() {
	std::cout << "To: " << to << "\n";
	std::cout << "From: " << from << "\n";
	std::cout << "DataSize: " << dataSize << "\n";
	std::cout << "Flags: " << flags << "\n";

	std::cout << "Data: ";
	int index;
	if (dataSize > 0)
	{
		char* homePtr = data; // prevents print after null char with subseq. prints
		for (index = 0; index < dataSize + 1; index++) {
			std::cout << *data;
			data++;
		}
		data = homePtr;
		homePtr = NULL;
	}

	std::cout << "\n";
}

// Prints the character represented by flags out in binary
void Packet::printFlagsBinary() {
	std::cout << "flags = " << std::bitset<8>(flags) << "\n";
}

bool Packet::getFlag(char flag)
{
	return (flags & flag);
}

void Packet::delete_data()
{
	if (data != nullptr)
	{
		delete data;
		data = nullptr;
		dataSize = 0;
	}
}