// AddressBook.c
/*
�����̸�: AddressBook.c
���: �ּҷ��� �����Ѵ�. (Create, Record, Find, Correct, Erase, Arrange, Destroy)
�ۼ���: ������
�ۼ�����: 2107/02/24
*/

// ��ũ�μ���
#include "AddressBook.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

/*
�Լ��̸�: Create
���: �Ҵ緮��ŭ ��ǥ�� �����.
�Է�: capacity(�Ҵ緮)
�ⷫ: ����
*/
void Create(AddressBook *addressBook, Long capacity) {
	addressBook->personals = (Personal(*))calloc(capacity, sizeof(Personal));
	addressBook->capacity = capacity;
	addressBook->length = 0;
}

/*
�Լ��̸�: Load
���: AddressBook.dat ���Ϸκ��� �ּҷ��� �ҷ��´�.
�Է�: ����
�ⷫ: count
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
�Լ��̸�: Record
���: ����, �ּ�, ��ȭ��ȣ, �̸����ּҸ� �Է��Ͽ� �ּҷϿ� �����Ѵ�.
�Է�: name, address, telephoneNumber, emailAddress
���: index(���� ��ġ)
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
�Լ��̸�: Find
���: ������ ������ �Է¹޾� �ּҷϿ� ���� ������ �ִ��� Ȯ���Ͽ� �ش��ϴ� ���� ��ġ��� ������ ����Ѵ�.
�Է�: name
���: indexes, count
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
�Լ��̸�: Correct
���: index�� �ش��ϴ� ������ �ּҷ��� �����Ѵ�.
�Է�: index, address, telephoneNumber, emailAddress
���: none
*/
Long Correct(AddressBook *addressBook, Long index, char(*address), char(*telephoneNumber), char(*emailAddress)) {
	strcpy(addressBook->personals[index].address, address);
	strcpy(addressBook->personals[index].telephoneNumber, telephoneNumber);
	strcpy(addressBook->personals[index].emailAddress, emailAddress);
	return index;
}

/*
�Լ��̸�: Erase
���: index�� �ش��ϴ� ������ �ּҷ��� �����Ѵ�.
�Է�: index
���: index(=0)
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
�Լ��̸�: Arrange
���: �ּҷ��� ������������ �����Ѵ�.
�Է�: ����
�ⷫ: ����
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
�Լ��̸�: Save
���: AddressBook.dat ���Ͽ� �ּҷ��� �����Ѵ�.
�Է�: ����
�ⷫ: count
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
�Լ��̸�: Destroy
���: ǥ�� �����Ѵ�.
�Է�: ����
���: ����
*/
void Destroy(AddressBook *addressBook) {
	if (addressBook->personals != NULL) {
		free(addressBook->personals);
		addressBook->personals = NULL;
	}
}
