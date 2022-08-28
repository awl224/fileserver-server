#include <linkedlist.hpp>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string>
class HashMap {
  private:
    // The backbone of the hash map. This is an array of Linked List pointers.
    linkedlist::LinkedList** array;

    // The number of buckets in the array
    std::size_t size; 

    // Generate a prehash for an item with a given size
    unsigned long prehash(string item);

  public:
    
    // Initialize an empty hash map, where size is the number of buckets in the array
    HashMap(std::size_t size);
    HashMap();
    // Free all memory allocated by the hash map
    ~HashMap();

    // Hash an unsigned long into an index that fits into a hash map
    unsigned long hash(string item);

    // Insert item in the map. Return true if the item was inserted, false if it wasn't (i.e. it was already in the map)
    // Recalculate the load factor after each successful insert (round to nearest whole number).
    // If the load factor exceeds 70 after insert, resize the table to hold twice the number of buckets.

    // Inserts the k/v pair into the hash map. Returns true if the key already existed,
    // and replaces the stored value with the supplied value. Returns false if the key did not exist already.
    bool insert(string key, File value);

    //Returns the file associated with the supplied key. If the key doesn't exist, this function throws an exception.
    File get(string key);

    // Remove an item from the map. Return true if it was removed, false if it wasn't (i.e. it wasn't in the map to begin with)
    bool remove(string item);

    // Return true if the item exists in the map, false otherwise
    bool contains(string item);

    // Resize the underlying table to the given size. Recalculate the load factor after resize
    void resize(std::size_t new_size);

    // Returns the number of items in the hash map
    std::size_t len();

    // Returns the number of items the hash map can hold before reallocating
    std::size_t capacity();

    // Print Table. You can do this in a way that helps you implement your hash map.
    void print();

};


unsigned long HashMap::prehash(string item) {
    int hashVal = 0;
    for(int i = 0; i < item.length(); i++){
      hashVal += item[i];
    }
    return hashVal;
}

HashMap::HashMap(size_t size){
  this->array = new linkedlist::LinkedList*[size];
  for(int i =0; i<size; i++){
    this->array[i] = new linkedlist::LinkedList();
  }
  this->size=size;
}

HashMap::HashMap(){
  this->array = new linkedlist::LinkedList*[10];
  for(int i =0;i<10;i++)
  {
    this->array[i] = new linkedlist::LinkedList();
  }
  this->size=10;
}

HashMap::~HashMap()
{
  for(int i = 0;i<this->size;i++)
  {
    array[i]->~LinkedList();
  }
}

unsigned long HashMap::hash(string item)
{
  unsigned long pre = this->prehash(item);
  unsigned long s = (unsigned long)(pre%this->size);
  return s;
}

bool HashMap::insert(string key, File item)
{
  if(this->contains(key)){ //key is in hash map
    unsigned long hashed = hash(key);
    linkedlist::LinkedList ll = this->array[hashed];
    for(int i = 0; i < ll.size; i++){
      if(ll.itemAtIndex(i).name.compare(key) == 0){
        ll.itemAtIndex(i) = item;
        return true;
      }
    }
    printf("ERROR in contains of HashMap/LinkedList\n");
  }
  unsigned long hashed = hash(key);
  this->array[hashed]->insertAtTail(item);
  int buckets = 0;
  for(int i =0;i<this->size;i++)
  {
    if(this->array[i]->size>0)
    {
      buckets++;
    }
  }
  double loadfactor = (double)(buckets)/(double)(this->size);
  if(loadfactor>=0.70)
  {
    this->resize(this->size*2);
  }
  return false;
}

File HashMap::get(string key){
  if(!this->contains(key)){
    throw(1);
  }
  unsigned long hashed = hash(key);
  linkedlist::LinkedList ll = this->array[hashed];
  for(int i = 0; i < ll.size; i++){
    if(ll.itemAtIndex(i).name.compare(key) == 0){
      return ll.itemAtIndex(i);
    }
  }
  printf("ERROR in HashMap/LinkedList contains\n");
  throw(1);
}

bool HashMap::remove(string item)
{
  if(this->contains(item)==false)
  {
    return false;
  }
  unsigned long hashed = hash(item);
  bool s = this->array[hashed]->removeItem(item);
  if(s==0)
  {
    return false;
  }
  return true;
}

bool HashMap::contains(string item)
{
  unsigned long hashed = hash(item);
  bool res = this->array[hashed]->contains(item);
  return res;
}

void HashMap::resize(size_t new_size)
{
  linkedlist::LinkedList** old = this->array;
  File element[this->len()];
  size_t limit = this->size;
  size_t currentindex = 0;
  for(size_t i =0;i<limit;i++)
  {
    if(this->array[i]->size>0)
    {
      for(int j = 0;j<this->array[i]->size;j++)
      {
        element[currentindex++] = this->array[i]->itemAtIndex(j);
      }
    }
  }
  this->size = new_size;
  this->array = new linkedlist::LinkedList*[new_size];
  for(int i =0;i<new_size;i++)
  {
    this->array[i] = new linkedlist::LinkedList();
  }
  for(int i =0;i<currentindex;i++)
  {
    this->insert(element[i].name, element[i]);
  }
  for(int i = 0;i<limit;i++)
  {
    old[i]->~LinkedList();
  }
}

size_t HashMap::len()
{
  size_t len = 0;
  for(int i =0;i<this->size;i++)
  {
    len += this->array[i]->size;
  }
  return len;
}


size_t HashMap::capacity()
{
  return (size_t)(0.7*(double)this->size) - 1;
}

void HashMap::print()
{
  for(int i =0;i<this->size;i++)
  {
    printf("List %d: ",i+1);
    this->array[i]->print();
    printf("\n");
  }
}
