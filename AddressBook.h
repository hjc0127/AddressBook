// AddressBook.h
#ifndef _ADDRESSBOOK_H
#define _ADDRESSBOOK_H
#define NAME 11
#define ADDRESS 64
#define TELEPHONENUMBER 12
#define EMAILADDRESS 64

// 자료형선언
typedef signed long int Long;
typedef struct _personal {
	char name[NAME];
	char address[ADDRESS];
	char telephoneNumber[TELEPHONENUMBER];
	char emailAddress[EMAILADDRESS];
}Personal;
typedef struct _addressBook {
	Personal(*personals);
	Long capacity;
	Long length;
}AddressBook;

// 함수선언
void Create(AddressBook *addressBook, Long capacity);
Long Load(AddressBook *addressBook);
Long Record(AddressBook *addressBook, char(*name), char(*address), char(*telephoneNumber), char(*emailAddress));
void Find(AddressBook *addressBook, char(*name), Long *(*indexes), Long *count);
Long Correct(AddressBook *addressBook, Long index, char(*address), char(*telephoneNumber), char(*emailAddress));
Long Erase(AddressBook *addressBook, Long index);
void Arrange(AddressBook *addressBook);
void Destroy(AddressBook *addressBook);
Long Save(AddressBook *addressBook);
# endif 	// _ADDRESSBOOK_H#pragma once
