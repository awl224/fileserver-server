#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pack109.hpp>

 #ifndef LLNODE_H
 #define LLNODE_H

namespace linkedlist{
class Node {
    public:
	    File item;
	    struct Node*  next;
        Node(File item);
		~Node(); //must create recursive delete
};
}

linkedlist::Node::Node(File item){
	this->item = item;
	this->next = NULL;
}

linkedlist::Node::~Node(){
	if(this->next != NULL){
		delete this->next;
	}
}
#endif