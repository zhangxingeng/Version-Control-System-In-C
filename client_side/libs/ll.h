#ifndef LL_H
#define LL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#include "files_io.h"

struct File;

union Data{
		int num;
		void* ptr;
} data;

struct Node {
	union Data data;
	struct Node* next;
	struct Node* prev;
};


struct Node* prepend(struct Node* head, union Data data);
struct Node* append(struct Node* head, union Data data);
void free_node(struct Node* ptr, void (*free_data)());

struct Node* delete_head(struct Node* head, void (*free_data)());
struct Node* delete_tail(struct Node* head, void (*free_data)());
struct Node* delete_node(struct Node* head, struct Node* to_delete, void (*free_data)());

int traverse(struct Node* head, void (*fun)(struct Node*));
struct Node* search(struct Node* head, union Data to_find, int(*comp)());
struct Node* merge_sort(struct Node* head, int(*comp)());


#endif