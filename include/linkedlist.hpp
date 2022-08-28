#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <llnode.hpp>
#include <vector>

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

namespace linkedlist{
class LinkedList {
	private:
		Node* head;
	    Node* tail;
    public:
		int size;
        LinkedList();
        LinkedList(LinkedList *list);
		~LinkedList();
		int insertAtTail(File item);
        int insertAtHead(File item);
		int insertAtIndex(int index, File item);
		File removeTail();
		File removeHead();
		File removeAtIndex(int index);
		bool removeItem(string item);
		bool contains(string item);
		File itemAtIndex(int index);
		void print();
};
}
//Initialize List
linkedlist::LinkedList::LinkedList() {
	this->head = NULL;
	this->tail = NULL;
	this->size = 0;
}

linkedlist::LinkedList::LinkedList(LinkedList *list) {
	this->head = list->head;
	this->tail = list->tail;
	this->size = list->size;
}

linkedlist::LinkedList::~LinkedList(){
	delete this->head;

}

//Add new item at the end of list.
int linkedlist::LinkedList::insertAtTail(File item) {
	Node* node = new Node(item);
	//if list is empty.
	if(this->head == NULL) {
		this->head = node;
		this->tail = node;
	} else {
		this->tail->next = node;
		this->tail	= this->tail->next;
	}	
	this->size++;
	return 0;	
}

//Add new item at beginning of the list.
int linkedlist::LinkedList::insertAtHead(File item) {
	Node* node = new Node(item);
	//if list is empty.
	if(this->head == NULL) {
		this->head = node;
		this->tail = node;
	} else {
		node->next	= this->head;
		this->head	= node;
	}		
	this->size++;
	return 0;
}

//Add new item at a certain index of the list.
int linkedlist::LinkedList::insertAtIndex(int index, File item) {
	Node* to_insert = new Node(item);
	int i = 0;
	Node* prev;
	Node* node = this->head;
	while (node != NULL) {
		if (i == index) {
			prev->next = to_insert;
			to_insert->next = node;
			this->size++;
			return 0;
		} else if (i > index) {
			return 0;
		} else {
			i++;
			prev = node;
			node = node->next;
		}
	}	
	return	0; 
}

File linkedlist::LinkedList::removeAtIndex(int index) {
  int i = 0;
  Node* prev;
  Node* node = this->head;
  while (node != NULL) {
    if (i == index) {
      prev->next = node->next;
		File item = node->item;
		delete node;
		this->size--;
    	return item;
    } else if (i > index) {
    	// List is too short
		std::vector<unsigned char> vect;
		return File{NULL, vect};
    } else {
      i++;
      prev = node;
      node = node->next;
    }
  }
  std::vector<unsigned char> vect;
  return File{NULL, vect};
}

//Delete item from the end of list.
File linkedlist::LinkedList::removeTail() {
	Node *temp;
	int i = 0;

	File item;

	if(this->tail == NULL) {	
		// List is Empty	
		std::vector<unsigned char> vect;
  		return File{NULL, vect};
	}
	else {
		temp = this->head;

		// Iterate to the end of the list
		while(temp->next != this->tail) { 
			temp = temp->next;
		}

		item = this->tail->item;

		Node* old_tail = this->tail;
		this->tail = temp;
		this->tail->next = NULL;	
		delete old_tail;
		this->size--;
	}	
	return item;
}

File linkedlist::LinkedList::removeHead() {
	File item;
	if(this->head == NULL) {	
		// List is Empty
		std::vector<unsigned char> vect;
  		return File{NULL, vect};
	} else {
		item = this->head->item;
		Node* old_head = this->head;
		this->head = this->head->next;	
    	old_head->next = NULL;
		delete old_head;
		this->size--;
	}	
	return item;
}

bool linkedlist::LinkedList::removeItem(string item){
	Node* curr = this->head;
	Node* prev = NULL;
	
	while(curr != NULL){
		if(curr->item.name.compare(item) == 0){
			if(prev == NULL){
				this->head = NULL;
				this->size--;
				return true;
			}
			prev->next = curr->next;
			this->size--;
			return true;
		}
		prev = curr;
		curr = curr->next;
	}
	return false;
}

bool linkedlist::LinkedList::contains(string item){
	Node* curr = this->head;
	while(curr != NULL){
		if(curr->item.name.compare(item) == 0){
			return true;
		}
		curr = curr->next;
	}
	return false;
}

File linkedlist::LinkedList::itemAtIndex(int index){
	Node* current = this->head;
	for(int i = 0;i<index;i++){
		if(current->next!=NULL){
			current = current->next;
		}
		else{
			exit(1);
		}
	}
	return current->item;
}


void linkedlist::LinkedList::print() {
	Node* node;

  // Handle an empty node. Just print a message.
	if(this->head == NULL) {
		printf("\nEmpty List\n");
		return;
	}
	
  // Start with the head.
	node = (Node*) this->head;

	printf("\nList: \n\t"); 
	while(node != NULL) {
		//printf("[ %x ]", node->item);
		std::cout << "[" << std::hex << node->item.name << "]";

    // Move to the next node
		node = (Node*) node->next;

		if(node !=NULL) {
			printf("-->");
    }
	}
	printf("\n\n");
}
#endif