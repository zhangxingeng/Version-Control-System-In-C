#include "ll.h"


/**
 * prepend new node
 * return the new head
 */
struct Node* prepend(struct Node* head, union Data data){
  struct Node *new_node;
  new_node = (struct Node*)calloc(1, sizeof(struct Node));
  new_node->data = data;

  if (head == NULL){return new_node;}
  new_node->next = head;
  head->prev = new_node;
  return new_node;
}


/**
 * 	free a node with its content
 */
void free_node(struct Node* ptr, void (*free_data)()){
	free_data(ptr->data);
	free(ptr);
}


/**
 * append new node
 * return the new head
 */
struct Node* append_at_tail(struct Node* tail, struct Node* head, struct Node* nxt){
	if(tail == NULL){return nxt;}
	else{
		tail->next = nxt;
		nxt->prev = tail;
		return head;
	}
}

/**
 * take a node out from a ll
 * return NULL unless head is changed
 */
struct Node* node_extract(struct Node* node, struct Node* head){
	struct Node* res = head;
	if(head == node){
		if(node->next == NULL){res = NULL;}
		res = node->next;
	}
	struct Node* nxt = node->next;node->next = NULL;
	struct Node* prv = node->prev; node->prev = NULL;
	if(nxt != NULL){nxt->prev = prv;}
	if(prv != NULL){prv->next = nxt;}
	return res;
}

struct Node* add_before(struct Node* node, struct Node* before, struct Node* head){
	if(node == NULL){return before;}
	struct Node* res = head, *prev = NULL;
	if(node->prev != NULL){prev = node->prev;}
	if(prev != NULL){
		prev->next = before; before->prev = prev;
		node->prev = before; before->next = node;
	}
	else{
		res = before;
		before->prev = node; node->next = before;
	}
	return res;
}

struct Node* delete_head(struct Node* head, void (*free_data)()){
  if(head == NULL){return head;}

  struct Node* new_head = head->next;
  new_head->prev = NULL;
  free_node(head, free_data);
  return new_head;
}


struct Node* delete_tail(struct Node* head, void (*free_data)()){
	if (head == NULL){return NULL;}
	struct Node* curr = head;
	struct Node* prev = NULL;
	while(curr->next != NULL){prev = curr; curr = curr->next;}
	free_data(curr);
	if(prev == NULL){return NULL;}
	prev->next = NULL;
	return head;
}

/*
 * delete a node pointed at
 * return the head of a list
 */
struct Node* delete_node(struct Node* head, struct Node* to_delete, void (*free_data)()){
	if(to_delete == NULL){return head;}
	struct Node* prv = to_delete->prev, *nxt = to_delete->next;
	if(prv == NULL && nxt == NULL){head = NULL;}
	if(prv != NULL){prv->next = nxt;}else{head = nxt;}
	if(nxt != NULL){nxt->prev = prv;}
	free_node(to_delete, free_data);
	return head;
}



/**
 * traverse a linked list and apply fun to all node
 * return applied nodes number
 */
int traverse(struct Node* head, void (*fun)(struct Node*)){
  struct Node* curr = head; int cnt = 0;
  if (curr == NULL){return cnt;}

  while (curr->next != NULL){fun(curr); cnt++; curr = curr->next;}
  return cnt;
}


/**
 * search for a node with value to_find
 */
struct Node* search(struct Node* head, union Data to_find, int(*comp)()){
	struct Node* curr = head;
	if(curr == NULL){return NULL;}
	do{
		if(comp(curr->data, to_find)== 0){return curr;}
		curr = curr->next;
	}while(curr->next != NULL);
	return NULL;
}

/*
 * Split a doubly linked list (DLL) into 2 DLLs of
 * half sizes
 */

struct Node* cut_to_half(struct Node* head){
    struct Node* fast = head,*slow = head;
    while (fast->next && fast->next->next){
        fast = fast->next->next;
        slow = slow->next;
    }
    struct Node* second_half = slow->next;
    slow->next = NULL;
    return second_half;
}

/* Function to merge two linked lists
 *
 */
struct Node* merge_to_one(struct Node* first, struct Node* second, int(*comp)()){
    if (first == NULL){return second;}
    if (second == NULL){return first;}

    if (comp(first->data, second->data) < 0){	//pick smaller
        first->next = merge_to_one(first->next,second, comp);
        first->next->prev = first;
        first->prev = NULL;
        return first;
    }else{
        second->next = merge_to_one(first,second->next, comp);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}
/*
 * Function to do merge sort
 */
struct Node* merge_sort(struct Node* head, int(*comp)()){
    if (head == NULL || head->next == NULL){return head;}

    struct Node* second = cut_to_half(head);
    head = merge_sort(head, comp);
    second = merge_sort(second, comp);
    return merge_to_one(head, second, comp);
}






