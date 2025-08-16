#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <iostream>
#include <string>


template <typename T>

class LinkedList {
    
    public:

        class Node{

            public:

                //data and next
                T* data;
                Node *next;

                
                //constructor
                Node(T* D, Node * N) : data(D), next(N){}

                //destructor
                ~Node(){

                    delete data;
                    data = nullptr;
                    next = nullptr;

                }

        };




        //head
        LinkedList<T>::Node * head;

        //sets the head to nullptr
        LinkedList() : head(nullptr){}

        

        Node * getHead(){

            return head;  
        }


        //adds to the front of the list
        void addFront(T * ptr){

            Node *temp = new Node(ptr, head);

            head = temp;
            
            temp = nullptr;

        }


        //adds to the back of the list
        void addBack(T* ptr){


            if(head == nullptr){

                head = new Node(ptr, nullptr);

                return;
            }


            Node *ptr1 = head;

            while(ptr1->next != nullptr){

                ptr1 = ptr1->next;
            }

            ptr1->next = new Node(ptr, nullptr);
            

            ptr1 = nullptr;

        }


        // void remove(const T & value){

        //     //if list is empty
        //     if(head == nullptr){

        //         return;
        //     }

        //     //if im deleting the head of the list
        //     if(head->data == value){


        //         Node * temp = head;

        //         head = head->next;

        //         delete temp;

        //         temp = nullptr;

        //         return;
        //     }

        //     //deleting anything else

        //     Node *ptr = head->next;

        //     while(ptr->next != nullptr){

        //         if(ptr->next->data == value){

        //             Node * temp = ptr->next;

        //             ptr->next = temp->next;

        //             delete temp;

        //             temp = nullptr;

        //         }

        //         ptr = ptr->next;


        //     }

        //     ptr = nullptr;

        // }

        //specifically for TimeSeries Removal
        // void remove(std::string seriesCode){

        //     //if list is empty
        //     if(head == nullptr){

        //         return;
        //     }

        //     //if im deleting the head of the list
        //     if(head->data.getSeriesCode() == seriesCode){


        //         Node * temp = head;

        //         head = head->next;

        //         delete temp;

        //         temp = nullptr;

        //         return;
        //     }

        //     //deleting anything else

        //     Node *ptr = head->next;

        //     while(ptr != nullptr && ptr->next != nullptr){

        //         if(ptr->next->data == seriesCode){

        //             Node * temp = ptr->next;

        //             ptr->next = temp->next;

        //             delete temp;

        //             temp = nullptr;

        //         }

        //         ptr = ptr->next;


        //     }

        //     ptr = nullptr;

        // }

        ~LinkedList(){

            if(head == nullptr){

                return;
            }

            Node * ptr1 = head;
            

            while(ptr1 != nullptr){

                Node *ptr2 = ptr1->next;

                delete ptr1;

                ptr1 = ptr2;

                
            }
            head = nullptr;
        }

};

#endif