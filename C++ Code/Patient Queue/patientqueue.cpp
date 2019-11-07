/* Assignment: Patient Queue
 * Partners: Zackery Bangs, Alex Little
 * Section Leader: Evin Yang
 * File Name: patientqueue.cpp
 * -----------------------------
 * This file completes the PatientQueue assignment.
 *
 * PatientQueue is a priority queue of Patient objects. As an object is added to the queue, it is then organized by its priority relative to its parent.
 * If the queue is called as a Min Heap, each Patient should be organized to where the parent is of higher priority than each child below it. If the
 * queue is called as a Max Heap, then the parent should be of lower priority. As an object is organized, the queue reevaluates itself to ensure that each
 * object remains at its proper place in the structure.
 */


#include "patientqueue.h"

/**
 * @brief PatientQueue::PatientQueue - creates a new instance of a priority queue that is either a min heap or a max heap based on the boolean value passed in
 * @param isMinHeap - boolean that determines if this particular instance of a priority queue is a min heap or a max heap
 */
PatientQueue::PatientQueue(bool isMinHeap) {
    _capacity = 10;
    _patients = new Patient[_capacity];
    _isMinHeap = isMinHeap;
    _size = 0;
}

/**
 * @brief PatientQueue::~PatientQueue - Erases the current queue from the heap.
 */
PatientQueue::~PatientQueue() {
    delete[] _patients;
}

/**
 * @brief PatientQueue::bubbleUpConditions - this function allows the queue to compare
 * 		Patient objects to their parents and move them up the queue if necessary
 * @param int i - integer position of Patient object in the queue
 * @return true or false
 */
bool PatientQueue::bubbleUpConditions(int i){
    bool childLower = (_patients[i].priority < _patients[i / 2].priority);
    bool childEqual = (_patients[i].priority == _patients[i / 2].priority);
    bool parentLower = !childLower;
    bool childNameFirst = (_patients[i].name < _patients[i / 2].name);
    bool parentNameFirst = !childNameFirst;


    if(childLower) {
        return true;
    } else if (childEqual && childNameFirst){
        return true;
    } else if (parentLower) {
        return false;
    } else if (childEqual && parentNameFirst){
        return false;
    }

    throw "none work";
}

/**
 * @brief PatientQueue::bubbleDownConditions - this function allows the queue to compare
 * 		Patient objects to their parents and move them up the queue if necessary
 * @param int i - integer position of Patient object in the queue
 * @return true or false
 */
bool PatientQueue::bubbleDownConditions(int i) {
    bool firstChildLower = (_patients[i * 2 + 1].priority < _patients[i * 2].priority);
    bool firstChildEqual = (_patients[i * 2 + 1].priority == _patients[i * 2].priority);
    bool secondChildLower = !firstChildLower;
    bool secondNameFirst = (_patients[i * 2 + 1].name < _patients[i * 2].name);
    bool firstNameFirst = !secondNameFirst;

    if(firstChildLower) {
        return true;
    } else if(firstChildEqual && secondNameFirst) {
        return true;
    } else if(secondChildLower){
        return false;
    } else if(firstChildEqual && firstNameFirst) {
        return false;
    }

    throw "none work";
}

/**
 * @brief PatientQueue::bestChildConditions
 * @param int i - integer position of Patient object in the queue
 * @param bestChild
 * @return true or false
 */
bool PatientQueue::bestChildConditions(int i, int bestChild) {
    bool childLower = (_patients[i].priority > _patients[bestChild].priority);
    bool childEqual = (_patients[i].priority == _patients[bestChild].priority);
    bool parentLower = !childLower;
    bool childNameFirst = (_patients[i].name > _patients[bestChild].name);
    bool parentNameFirst = !childNameFirst;

    if(childLower) {
        return true;
    } else if (childEqual && childNameFirst){
        return true;
    } else if (parentLower) {
        return false;
    } else if (childEqual && parentNameFirst){
        return false;
    }

    throw "none work";
}

/**
 * @brief PatientQueue::organizeHeap - Takes in a given index and moves the patient of that index to its correct location by first 'bubbling  up' by
 * checking the parents of the element, then it moves the element at the given index down the queue by 'bubbling down' through the children of the index.
 * @param int i - integer position of Patient object in the queue
 */
void PatientQueue::organizeHeap(int i) {
    // bubble up
    int start = i;
    while (i / 2 > 0) {
        if ((bubbleUpConditions(i) && _isMinHeap) || (!bubbleUpConditions(i) && !_isMinHeap)) {
            swap(_patients[i], _patients[i / 2]);
        }

        i = i / 2;
    }
    i = start;

    // bubble down
    while ( i * 2 <= _size && i != 0) {
        int bestChild;
        if (((bubbleDownConditions(i) && _isMinHeap) || (!bubbleDownConditions(i) && !_isMinHeap)) && (i * 2 + 1 <= _size))  {
           bestChild = i * 2 + 1;
        } else {
            bestChild = i * 2;
        }

        if ((bestChildConditions(i, bestChild) && _isMinHeap) || (!bestChildConditions(i, bestChild) && !_isMinHeap)) {
            swap(_patients[i], _patients[bestChild]);
        }

        i = bestChild;
    }
}

/**
 * @brief PatientQueue::changePriority - changes the priority for the Patient in the queue
 * @param value - the name of the intended Patient
 * @param newPriority - the new priority to be assigned to the Patient
 */
void PatientQueue::changePriority(string value, int newPriority) {
    Patient temp;
    for (int i = 1; i < _capacity; i++) {
        if (_patients[i].name == value) {
            _patients[i].priority = newPriority;
            organizeHeap(i);
            return;
        }
    }
    throw "Name not found!"
}

/**
 * @brief PatientQueue::clear - Clears the contents of the current Priority Queue without deleting the queue from the heap.
 */
void PatientQueue::clear() {
    for (int i = 1; i <= _size; i++) {
        _patients[i] = Patient();
    }
    _size = 0;
}

/**
 * @brief PatientQueue::debug
 */
void PatientQueue::debug() {
    // empty
}

/**
 * @brief PatientQueue::dequeue - if the queue is not empty, it will take the topmost element (the greatest or least element depending on whether it is a
 * 		min or max heap), removes it, and returns the name of the patient.
 * @return - string representing the name of the patient.
  */
string PatientQueue::dequeue() {
    if (isEmpty()) {
        throw "Can't dequeue an empty queue.";
    }

    Patient first = _patients[1];
    _patients[1] = _patients[_size];
    _patients[_size] = Patient();
    _size--;
    organizeHeap(1);

    return first.name;
}

/**
 * @brief PatientQueue::enqueue - adds a new Patient to the queue
 * @param value - the name of the Patient to be added
 * @param priority - the priority of the added Patient
 */
void PatientQueue::enqueue(string value, int priority) {
    if (_capacity == _size+1) {
        Patient* bigger = new Patient[2 * _capacity];
        for (int i = 1; i <= _size; i++) {
            bigger[i] = _patients[i];
        }
        delete[] _patients;
        _patients = bigger;
        _capacity *= 2;
    }
    _size++;
    _patients[_size] = Patient(value, priority);
    organizeHeap(_size);
}

/**
 * @brief PatientQueue::isEmpty - checks if the queue is empty.
 * @return - Returns true if the size of the queue is 0.
 */
bool PatientQueue::isEmpty() const {
    if (_size == 0) return true;
    return false;
}

/**
 * @brief PatientQueue::peek - If the queue is not empty, it will return the name of the first patient in the queue.
 * @return - returns a string representing the name of the first patient.
 */
string PatientQueue::peek() const {
    if (isEmpty()) {
        throw "Can't peek at an empty queue!";
    }
    return _patients[1].name;
}

/**
 * @brief PatientQueue::peekPriority - If the queue is not empty, it will return the priority of the first patient in the queue.
 * @return Returns the integer value of the priority of the first patient in the queue.
 */
int PatientQueue::peekPriority() const {
    if (isEmpty()) {
        throw "Can't peek at an empty queue!";
    }
    return _patients[1].priority;
}

/**
 * @brief PatientQueue::size - Returns the size of the queue as an integer.
 * @return - an integer value representing the size of the queue.
 */
int PatientQueue::size() const {
    return _size;
}

/**
 * @brief operator << - prints out the patients in the queue.
 * @param out - An ostream variable that stores what is being printed.
 * @param queue - The queue that is being printed out.
 * @return out - an ostream operator
 */
ostream& operator <<(ostream& out, const PatientQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        out << queue._patients[1].name << " (" << queue._patients[1].priority << ")";
        for (int i = 2; i <= queue.size(); i++) {
            out << ", " << queue._patients[i].name << " (" << queue._patients[i].priority << ")";
        }
    } else {
        out << "";
    }
    out << "}";
    return out;
}
