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
protected:
    struct Node {
    public:
        T data;
        Node * next = nullptr;
        Node * prev = nullptr;
        
    public:
        Node(T data) : data(data) {}
    };
    
    Node * head = nullptr;
    Node * tail = nullptr;
    
/*
    Subclass Interfacing
 */
protected:
    void deInit() {
        auto current = head;
        while (current) {
            auto trash = current;
            current = current->next;
            delete trash;
        }
        head = nullptr;
        tail = nullptr;
    }
    
    void append(Node * node) {
        if (head == nullptr) {
            head = node;
            tail = node;
        } else if (head == tail) {
            tail = node;
            head->next = tail;
            tail->prev = head;
        } else {
            auto last = tail;
            tail = node;
            tail->prev = last;
            last->next = tail;
        }
        
        size++;
    }
    
    bool invalidateIndex(int index) {
        if (index < 0 || index > size - 1) {
            String error = "Index out of range: " + String(index);
            Serial.println(error);
            return false;
        }
        
        return true;
    }
    
/*
    Public methods
 */
public:
    int size = 0;
    
    LinkedList<T>(){
        
    };
    
    // Copy Constructor
    LinkedList<T>(const LinkedList<T> &obj) {
        auto source = obj.head;
        while (source) {
            auto node = new Node(*source);
            node->prev = nullptr;
            node->next = nullptr;
            append(node);
            source = source->next;
        }
    }
    
    // Copy Assignment
    LinkedList<T> & operator =(LinkedList<T> const & rhs) {
        deInit();
        extend(rhs);
        return * this;
    }
    
    ~LinkedList<T>() {
        deInit();
    }

    bool isEmpty() {
        return size == 0;
    }
    
    void append(T data) {
        auto node = new Node(data);
        append(node);
    }
    
    void extend(LinkedList<T> const & other) {
        auto source = other.head;
        while (source) {
            auto node = new Node(*source);
            node->next = nullptr;
            node->prev = nullptr;
            append(node);
            source = source->next;
        }
    }
    
    Node * searchFromHead(int index) {
        if (invalidateIndex(index) == false) {
            return nullptr;
        }
        
        auto current = head;
        int i = 0;
        while(i != index) {
            current = current->next;
            i++;
        }
        
        return current;
    }
    
    Node * searchFromTail(int index) {
        if (invalidateIndex(index) == false) {
            return nullptr;
        }
        
        auto current = tail;
        int i = size - 1;
        while (i != index) {
            current = current->prev;
            i--;
        }
        
        return current;
    }
    
    T & get(int index) {
        // if (invalidateIndex(index) == false) {
        //     return T();
        // }
        auto target = index < size /2 ? searchFromHead(index) : searchFromTail(index);
        return (target->data);
    }

    bool remove(int index) {
        if (invalidateIndex(index) == false) {
            return false;
        }
        
        // Cut the searching time by 50% on average
        auto target = index < size / 2 ? searchFromHead(index) : searchFromTail(index);    
        if (head == tail) {
            // Only one element in the list. Target is automatically head for a valid index.
            head = nullptr;
            tail = nullptr;
        } else if (index == 0 && size == 2) {
            // special case where we are at head and only two nodes left
            head = tail;
            tail->prev = nullptr;
        } else if (index == 0) {
            // at head
            head = head->next;
        } else if (target->next) {
            // at body
            auto prev = target->prev;
            auto next = target->next;
            prev->next = next;
            next->prev = prev;
        } else {
            // at tail
            auto prev = target->prev;
            tail = prev;
            tail->next = nullptr;
        }

        size--;
        delete target;
        return true;
    }
    
    void printNodes() {
        auto current = head;
        int i = 0;
        while (current) {
            current = current->next;
            i++;
        }
    }
};
