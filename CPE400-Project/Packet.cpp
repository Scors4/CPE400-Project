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

void Packet::setACK() {
	// Binary: XXXX,XXX1

	// to be completed
}

void Packet::setCTRL() {
	// Binary: XXXX,XX1X

	// to be completed
}

void Packet::setFail() {
	// Binary: XXXX,X1XX

	// to be completed
}

void Packet::setBit3() {
	// Binary: XXXX,1XXX

	// to be completed
}

void Packet::setBit4() {
	// Binary: XXX1,XXXX

	// to be completed
}

void Packet::setBit5() {
	// Binary: XX1X,XXXX

	// to be completed
}

void Packet::setBit6() {
	// Binary: X1XX,XXXX

	// to be completed
}

void Packet::setBit7() {
	// Binary: 1XXX,XXXX

	// to be completed
}

/*******************************************************************************
 Clear flags
*******************************************************************************/
void Packet::clearAllFlags() {
	// Binary: 0000,0000
	// Hex: 00

	flags &= ~0xFF; // clears all bits
}

void Packet::clearACK() {
	// Binary: XXXX,XXX0

	// to be completed
}

void Packet::clearCTRL() {
	// Binary: XXXX,XX0X

	// to be completed
}

void Packet::clearFail() {
	// Binary: XXXX,X0XX

	// to be completed
}

void Packet::clearBit3() {
	// Binary: XXXX,0XXX

	// to be completed
}

void Packet::clearBit4() {
	// Binary: XXX0,XXXX

	// to be completed
}

void Packet::clearBit5() {
	// Binary: XX0X,XXXX

	// to be completed
}

void Packet::clearBit6() {
	// Binary: X0XX,XXXX

	// to be completed
}

void Packet::clearBit7() {
	// Binary: 0XXX,XXXX

	// to be completed
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

bool Packet::getFlag(char flag)
{
	return (flags & flag);
}

void Packet::delete_data()
{
	delete data;
	data = nullptr;
	dataSize = 0;
}