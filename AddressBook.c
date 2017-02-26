// AddressBook.c
/*
파일이름: AddressBook.c
기능: 주소록을 관리한다. (Create, Record, Find, Correct, Erase, Arrange, Destroy)
작성자: 조형진
작성일자: 2107/02/24
*/

// 매크로선언
#include "AddressBook.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

/*
함수이름: Create
기능: 할당량만큼 새표를 만든다.
입력: capacity(할당량)
출략: 없음
*/
void Create(AddressBook *addressBook, Long capacity) {
	addressBook->personals = (Personal(*))calloc(capacity, sizeof(Personal));
	addressBook->capacity = capacity;
	addressBook->length = 0;
}

/*
함수이름: Load
기능: AddressBook.dat 파일로부터 주소록을 불러온다.
입력: 없음
출략: count
*/
Long Load(AddressBook *addressBook) {
	Personal personal;
	Personal(*temp);
	Long flag;
	Long i;
	Long j;
	Long index;
	Long count = 0;
	FILE *file;
	file = fopen("AddressBook.dat", "rb");
	if (file != NULL) {
		flag = fread(&personal, sizeof(Personal), 1, file);
		while (!feof(file) && flag != 0) {
			i = 0;
			while (i<addressBook->capacity&&strcmp(addressBook->personals[i].name, "") != 0) {
				i++;
			}
			if (i >= addressBook->capacity) {
				temp = (Personal(*))calloc(addressBook->capacity + 1, sizeof(Personal));
				j = 0;
				while (j < addressBook->length) {
					temp[j] = addressBook->personals[j];
					j++;
				}
				if (addressBook->personals != NULL) {
					free(addressBook->personals);
					addressBook->personals = NULL;
				}
				addressBook->personals = temp;
				addressBook->capacity++;
			}
			index = i;
			addressBook->personals[index] = personal;
			addressBook->length++;
			count++;
			flag = fread(&personal, sizeof(Personal), 1, file);
		}
		fclose(file);
	}
	return count;
}

/*
함수이름: Record
기능: 성명, 주소, 전화번호, 이메일주소를 입력하여 주소록에 저장한다.
입력: name, address, telephoneNumber, emailAddress
출력: index(줄의 위치)
*/

Long Record(AddressBook *addressBook, char(*name), char(*address), char(*telephoneNumber), char(*emailAddress)) {
	Personal(*temp);
	Long index;
	Long i = 0;
	Long j;
	while (i < addressBook->capacity && strcmp(addressBook->personals[i].name, "") != 0) {
		i++;
	}
	if (i >= addressBook->capacity) {
		temp = (Personal(*))calloc(addressBook->capacity + 1, sizeof(Personal));
		for (j = 0; j < addressBook->length; j++) {
			temp[j] = addressBook->personals[j];
		}
		if (addressBook->personals != NULL) {
			free(addressBook->personals);
		}
		addressBook->personals = temp;
		addressBook->capacity++;
	}
	index = i;
	strcpy(addressBook->personals[index].name, name);
	strcpy(addressBook->personals[index].address, address);
	strcpy(addressBook->personals[index].telephoneNumber, telephoneNumber);
	strcpy(addressBook->personals[index].emailAddress, emailAddress);
	addressBook->length++;
	return index;
}

/*
함수이름: Find
기능: 개인의 성명을 입력받아 주소록에 같은 성명이 있는지 확인하여 해당하는 줄의 위치들과 개수를 출력한다.
입력: name
출력: indexes, count
*/
void Find(AddressBook *addressBook, char(*name), Long *(*indexes), Long *count) {
	Long i = 0;
	Long j = 0;
	*count = 0;
	if (addressBook->length > 0) {
		*indexes = (Long(*))calloc(addressBook->length, sizeof(Long));
	}
	while (i < addressBook->length) {
		if (strcmp(addressBook->personals[i].name, name) == 0) {
			(*indexes)[j] = i;
			(*count)++;
			j++;
		}
		i++;
	}
}


/*
함수이름: Correct
기능: index에 해당하는 개인의 주소록을 수정한다.
입력: index, address, telephoneNumber, emailAddress
출력: none
*/
Long Correct(AddressBook *addressBook, Long index, char(*address), char(*telephoneNumber), char(*emailAddress)) {
	strcpy(addressBook->personals[index].address, address);
	strcpy(addressBook->personals[index].telephoneNumber, telephoneNumber);
	strcpy(addressBook->personals[index].emailAddress, emailAddress);
	return index;
}

/*
함수이름: Erase
기능: index에 해당하는 개인의 주소록을 삭제한다.
입력: index
출력: index(=0)
*/
Long Erase(AddressBook *addressBook, Long index) {
	Personal(*temp) = { 0 };
	Long i;
	Long j;
	if (addressBook->capacity > 1) {
		temp = (Personal(*))calloc(addressBook->capacity - 1, sizeof(Personal));
	}
	for (i = 0; i < index; i++) {
		temp[i] = addressBook->personals[i];
	}
	j = i;
	for (++i; i < addressBook->length; i++) {
		temp[j] = addressBook->personals[i];
		j++;
	}
	if (addressBook->personals != NULL) {
		free(addressBook->personals);
		addressBook->personals = NULL;
	}
	if (addressBook->capacity > 1) {
		addressBook->personals = temp;
	}
	index = -1;
	addressBook->length--;
	addressBook->capacity--;

	return index;
}

/*
함수이름: Arrange
기능: 주소록을 오름차순으로 정렬한다.
입력: 없음
출략: 없음
*/
void Arrange(AddressBook *addressBook) {
	Long i;
	Long j;
	Personal temp;
	for (i = 0; i < addressBook->length - 1; i++) {
		for (j = 0; j < addressBook->length - i - 1; j++) {
			if (strcmp(addressBook->personals[j].name, addressBook->personals[j + 1].name) > 0) {
				temp = addressBook->personals[j];
				addressBook->personals[j] = addressBook->personals[j + 1];
				addressBook->personals[j + 1] = temp;
			}
		}
	}
}

/*
함수이름: Save
기능: AddressBook.dat 파일에 주소록을 저장한다.
입력: 없음
출략: count
*/
Long Save(AddressBook *addressBook) {
	//Long i;
	Long count = 0;
	Long index = 0;
	FILE *file;
	file = fopen("AddressBook.dat", "wb");
	if (file != NULL) {
		//while (i < addressBook->length) {
		//	fwrite(addressBook->personals + i, sizeof(Personal), 1, file);
		//	count++;
		//	i++;
		//}
		fwrite(addressBook->personals, sizeof(Personal), addressBook->length, file);
		fclose(file);
	}
	return count;
}

/*
함수이름: Destroy
기능: 표를 삭제한다.
입력: 없음
출력: 없음
*/
void Destroy(AddressBook *addressBook) {
	if (addressBook->personals != NULL) {
		free(addressBook->personals);
		addressBook->personals = NULL;
	}
}
