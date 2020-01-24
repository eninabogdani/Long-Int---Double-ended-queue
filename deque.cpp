// Implementation file for the Deque class.

template <class Object>
Deque<Object>::Deque( ) {                          // the constructor
  front = back = NULL;
}

template <class Object>
Deque<Object>::Deque( const Deque &rhs ) {         // the copy constructor
  front = back = NULL;
  *this = rhs;
}

template <class Object>
Deque<Object>::~Deque( ) {                         // the destructor
  clear( );
}

template <class Object>
bool Deque<Object>::isEmpty( ) const {             // check if a deque is empty
  return front == NULL;
}

template <class Object>
int Deque<Object>::size( ) const {                 // retrieves # deque nodes
  int i = 0;
  for ( DequeNode *ptr = front; ptr != NULL; ptr = ptr->next ) // traverse que
    ++i;

  return i;
}

template <class Object>
const Object &Deque<Object>::getFront( ) const {   // retrieve the front node
  if (isEmpty( ))
    throw "empty queue";
  return front->item;
}

template <class Object>
const Object &Deque<Object>::getBack( ) const {    // retrieve the tail node
  if (isEmpty( ))
    throw "empty queue";
  return back->item;
}

template <class Object>
void Deque<Object>::clear( ) {          // clean up all entries
  while (!isEmpty( ))                   // dequeue till the queue gets empty
    removeFront( );
}

// add a new node to front
// node being added is passed as parameter
template <class Object>
void Deque<Object>::addFront(const Object &obj) {
  DequeNode* newPtr = new DequeNode;
  newPtr->item = obj;
  if (!isEmpty()) {
    newPtr->prev = NULL;
    newPtr->next = front;
    front->prev = newPtr;
    front = newPtr;
  } else {
    newPtr->next = NULL;
    newPtr->prev = NULL;
    front = back = newPtr;
  }
}

// add a new node to tail
// node being added is passed as parameter
template <class Object>
void Deque<Object>::addBack(const Object &obj) { 
  DequeNode* newPtr = new DequeNode;
  newPtr->item = obj;
  if (!isEmpty()) {
    newPtr->prev = back;
    newPtr->next = NULL;
    back->next = newPtr;
    back = newPtr;
  } else {
    newPtr->next = NULL;
    newPtr->prev = NULL;
    back = front = newPtr;
  } 
}

// remove the front node
template <class Object>
Object Deque<Object>::removeFront( ) {
  if (isEmpty( ))
    throw "empty queue";
  
  Object frontItem = front->item;
  DequeNode* del = front;
  if (front == back) {  // if there is only one node, remove that
    front = back = NULL;
  } else {
    front->prev = NULL;
    front = front->next;
    //del->next = NULL;
  }

  delete del;
  return frontItem;
}

// remove the tail node
template <class Object>
Object Deque<Object>::removeBack( ) {
  if (isEmpty( ))
    throw "empty queue";
  
  Object backItem = back->item;
  DequeNode* del = back;
  if (front == back) {
    front = back = NULL;
  } else {
    back->prev->next = NULL;
    back = back->prev;
  }
  
  delete del;
  return backItem;
}

// assign the given Deque to the current object
template <class Object>
const Deque<Object> &Deque<Object>::operator=(const Deque &rhs) {
  if ( this != &rhs ) { // avoid self assignment
    clear( );
    for ( DequeNode *rptr = rhs.front; rptr != NULL; rptr = rptr->next )
      addBack( rptr->item );
  }
  return *this;
}


