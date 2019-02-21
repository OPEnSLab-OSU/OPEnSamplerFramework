//
//  LinkedList.hpp
//  OPEnSamplerFramework
//
//  Created by Kawin on 2/10/19.
//  Copyright © 2019 Kawin. All rights reserved.
//

#pragma once

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node * next = nullptr;
        Node(T data) : data(data) {

        }

        ~Node() {
            next = nullptr;
        }

        bool hasNext() {
            return next != nullptr;
        }
    };
    
    Node * head = nullptr;
    
public:
    int size = 0;
    
    ~LinkedList<T>() {
        Node * currentPtr = head;
        while (currentPtr) {
            Node * next = currentPtr;
            delete currentPtr;
            currentPtr = next->next;
        }
        
        head = nullptr;
    }
    
    /*
     Using new keyword to allocate memory in the heap.
     If new keyword is not used, the object will be destroyed when it goes out of scope.
     */
    void append(T data) {
        if (head) {
            Node * currentPtr = head;
            while (currentPtr->next) {
                currentPtr = currentPtr->next;
            }
            
            currentPtr->next = new Node(data);
        } else {
            head = new Node(data);
        }
        
        size = size + 1;
    }
    
    T get(int index) {
        if (index < 0 || index > size - 1) {
            String errorMessage = "Index out of range [" + String(index) + "]";
            Serial.println(errorMessage);
            return T();
        }
        
        // From now on, head is guaranteed to be initialized.
        int i = 0;
        Node* currentPtr = head;
        while (i != index) {
            currentPtr = currentPtr->next;
            i++;
        }
        return (currentPtr->data);
    }

    bool isEmpty() {
        return size == 0;
    }
    
    /*
     If prevPtr == null, that means we are  at the head.
     If we are not at the beginning of the linked list, set the prev node's
     next to the next node of the current pointer. If we are at the head, set
     head to the next node.
     */
    bool remove(int index) {
        if (index < 0 || index > size - 1) {
            return false;
        }
        
        int i = 0;
        Node * prevPtr = nullptr;
        Node * currentPtr = head;
        while (i != index) {
            prevPtr = currentPtr;
            currentPtr = currentPtr->next;
            i++;
        }
        
        if (prevPtr == nullptr) {
            head = head->next;
        } else {
            prevPtr->next = currentPtr->next;
        }
        
        size--;
        delete currentPtr;
        return true;
    }
};
