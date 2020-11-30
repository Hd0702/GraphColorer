// CLASS TEMPLATE CREDIT GOES TO https://github.com/SloCompTech/QList
// I edited this template class for the purposes of the project
#ifndef HW3_LINKEDLIST
#define HW3_LINKEDLIST

#ifndef NULL
#define NULL 0
#endif

template<class T>
class LinkedList
{
public:
    typedef struct node
    {
        T item;
        node *next,*prev;
    }node;

private:
    int len; // Size of list 
    node *start,*end; // Pointers to start and end

public:
    LinkedList(); // Class constructor
    ~LinkedList(); // Class destructor

    node& push_back(const T i); // Push item at the back of list
    node& push_front(const T i);// Push item at the front of the list
    void pop_back(); // Pops item from back
    void pop_front(); // Pops item from front
    LinkedList<T>::node * front(); // get item from front
    LinkedList<T>::node * back(); // get item from back
    int size(); // Returns size of list
    void clear(); // Clears list
    void remove(LinkedList<T>::node *);
    void clear(unsigned int index); // Clears list
    T get(unsigned int index); // Get item at given index 
    T& at(unsigned int index); // Get item at given index
    //CONTAINS
    bool contains(T);
    // Array operator
    T& operator[](unsigned int index);
    const T& operator[](unsigned int index) const; // Not realy needed

    // Non - critical functions
    int length();
    int indexOf(T val);

};



// Constructor
template<class T>
LinkedList<T>::LinkedList()
{
    len = 0;
    start = NULL;
    end = NULL;
}

// Destructor
template<class T>
LinkedList<T>::~LinkedList()
{
    clear();
}

// Push at front
template<class T>
typename LinkedList<T>::node & LinkedList<T>::push_front(const T i)
{
    node *tmp = new node;
    tmp->item = i;
    tmp->next = NULL;
    tmp->prev = NULL;

    if(start==NULL) // If list is empty
    {
        start = tmp;
        end = tmp;
    }
    else // Insert at start
    {
        tmp->next = start;
        start->prev = tmp;
        start = tmp;
    }
    len++; // Increase size counter
    return *tmp;
}

// Push at back
template<class T>
typename LinkedList<T>::node & LinkedList<T>::push_back(const T i)
{
    node *tmp = new node;
    tmp->item = i;
    tmp->next = NULL;
    tmp->prev = NULL;

    if(end==NULL) // If list is empty
    {
        start = tmp;
        end = tmp;
    }
    else // Insert at the end
    {
        tmp->prev = end;
        end->next = tmp;
        end = tmp;
    }
    len++; // Increase size counter
    return *tmp;
}

// Pop from front
template<class T>
void LinkedList<T>::pop_front()
{
    if(start!=NULL)
    {
        node *tmp = start;
        start = start->next;
        if(start!=NULL) // Re-link next item to NULL
            start->prev = NULL;
        else // List became empty so we need to clear end
            end = NULL;
        delete tmp;
        len--; // Decrease counter
    }
}

// Pop from back
template<class T>
void LinkedList<T>::pop_back()
{
    if(end!=NULL)
    {
        node *tmp = end;
        end = end->prev;
        if(end!=NULL) //Re-link previous item to NULL
            end->next = NULL;
        else // List became empty so we need to clear start
            start = NULL;
        delete tmp;
        len--; // Decrease counter
    }
}

template<class T>
void LinkedList<T>::remove(LinkedList<T>::node * listNode) {
    if (listNode == start) {
        pop_front();
    } else if (listNode == end) {
        pop_back();
    } else {
        if(listNode->prev!=NULL)
            listNode->prev->next = listNode->next;
        else
            start = listNode->next;

        if(listNode->next!=NULL)
            listNode->next->prev = listNode->prev;
        else
            end = listNode->prev;

        len--; // Decrease counter
        delete listNode; // Delete item
    }
}

template<class T>
bool LinkedList<T>::contains(T checkItem) {
    node *tmp = start;
    while(tmp!=NULL)
    {
        if (tmp->item == checkItem) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

// Get item from front
template<class T>
typename LinkedList<T>::node * LinkedList<T>::front()
{
    return start;
}

//Get item from back
template<class T>
typename LinkedList<T>::node * LinkedList<T>::back()
{
    return end;
}

// Get size
template<class T>
int LinkedList<T>::size()
{
    return this->len;
}

// Clear list
template<class T>
void LinkedList<T>::clear()
{
    node *tmp = start;
    while(start!=NULL)
    {
        tmp = start;
        start = start->next;
        delete tmp; // Delete item
        len--; // Decrease counter
    }
    end = NULL;
}
template<class T>
void LinkedList<T>::clear(unsigned int index)
{
    node *tmp = start;
    for(int i=0;i<=index&&tmp!=NULL;i++)
    {
        if(i==index)
        {
            if(tmp->prev!=NULL)
                tmp->prev->next = tmp->next;
            else
                start = tmp->next;

            if(tmp->next!=NULL)
                tmp->next->prev = tmp->prev;
            else
                end = tmp->prev;

            len--; // Decrease counter
            delete tmp; // Delete item
            break;
        }
        else
            tmp=tmp->next;
    }
}

// Get at index
template<class T>
T LinkedList<T>::get(unsigned int index)
{
    node *tmp = start;
    for(int i=0;i<=index&&tmp!=NULL;i++)
    {
        if(i==index)
            return tmp->item;
        else
            tmp=tmp->next;
    }
    //TODO: Catch error when index is out of range
}

template<class T>
T& LinkedList<T>::at(unsigned int index)
{
    node *tmp = start;
    for(int i=0;i<=index&&tmp!=NULL;i++)
    {
        if(i==index)
            return tmp->item;
        else
            tmp=tmp->next;
    }
    //TODO: Catch error when index is out of range
}

// Get length
template<class T>
int LinkedList<T>::length()
{
    return this->len;
}

// Get index of value
template<class T>
int LinkedList<T>::indexOf(T val)
{
    for(int i=0;i<this->size();i++)
        if(this->at(i) == val)
            return i;
    return -1;
}

// Array operators
template<class T>
T& LinkedList<T>::operator[](unsigned int index)
{
    node *tmp = start;
    for(int i=0;i<=index&&tmp!=NULL;i++)
    {
        if(i==index)
            return tmp->item;
        else
            tmp=tmp->next;
    }
    //TODO: Catch error when index is out of range
}


template<class T>
const T& LinkedList<T>::operator[](unsigned int index) const
{
    node *tmp = start;
    for(int i=0;i<=index&&tmp!=NULL;i++)
    {
        if(i==index)
            return tmp->item;
        else
            tmp=tmp->next;
    }
    //TODO: Catch error when index is out of range
}
#endif