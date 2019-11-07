/* Assignment: Patient Queue
 * Partners: Zackery Bangs, Alex Little
 * Section Leader: Evin Yang
 * File Name: patientqueue.h
 * ---------------------------
 * This file works with "patientqueue.cpp" to complete the Patient Queue assignment.
 *
 * PatientQueue.h initializes the Abstract Data Type (ADT) PatientQueue, which is an array
 * of Patient objects. This file acts as a main, with which PatientQueue can be implemented
 * and data may be accessed and modified by the user.
 */

#ifndef _patientqueue_h
#define _patientqueue_h

#include <iostream>
#include <string>
#include "patient.h"
using namespace std;

class PatientQueue {
public:
    PatientQueue(bool isMinHeap = true);
    ~PatientQueue();
    void changePriority(string value, int newPriority);
    void clear();
    void debug();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const PatientQueue& queue);

private:
    int _capacity;
    int _size;
    Patient* _patients;
    bool _isMinHeap;
    void switchPatients(int less, int more);
    void organizeHeap(int i);
    bool bubbleUpConditions(int i);
    bool bubbleDownConditions(int i);
    bool bestChildConditions(int i, int bestChild);
};

#endif
