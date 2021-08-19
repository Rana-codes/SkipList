// NAME = HARSH RANA;

#ifndef SLIST_H
#define SLIST_H
// Build  a skip list
// CPSC 2150, Assignment #5, February 2021
// Gladys Monagan

#include <iostream>

/*
* A skip list that keeps the values in ascending order.
* It supports insertion into the skip list but no deletions.
*/
class SList {
   public:
      // create an empty list
      SList();

      // return true if the list is empty, false otherwise
      bool isEmpty() const;

      // insert an element in ascending order
      void insert(int x);

      // return the number of elements in the skip list 
      int numberOfElements() const;

      // return true if x is found in the list, false otherwise
      bool search(int x) const;
          
      // delete/deallocate the skip list completely and reset to an empty list
      // leave the getTopListOutput() as is
      void reset();
      
      // overloaded assignment operator      
      SList& operator = (const SList&);
      
      // copy constructor
      SList(const SList&); 
      
      // destructor
      ~SList();

      // output the list in ascending order with SList::SEP after each element
      // precondition:
      //    if setTopListOutput(true) has been called previously,
      //          only the elements of the top list are output
      //    if setTopListOutput(false) is called previously (the default value)
      //          all the elements of the bottom list are output
      // 
      // the empty list outputs nothing, not even SList::SEP
      friend std::ostream& operator<< (std::ostream&, const SList&);

      // string that separates the values of the list when outputting
      static const std::string SEP;

      // mutator method that determines if the top list or the bottom list is output
      // postcondition:
      //    after setTopListOutput(true), the top list will be output with <<
      //    after setTopListOutput(false), all the elements are output with <<
      void setTopListOutput(bool topListDumping);

      // return true implying that the top list should be printed with <<
      // return false implying that all the elements will be printed with <<
      bool getTopListOutput() const;
       
   private:
      struct Node {
         int item;
         Node* link;
      };

      struct UpperNode {
         Node* item;
         UpperNode* link;
      };

      UpperNode* DumUpperHead;
      Node* DumHead;

      UpperNode* upperHead;
      Node* head;

      bool outputTopList;

      // remove and dispose of every node of p and return the nullptr
      static Node* deleteList(Node* nodeInHand);

      // deletes the upper list passed
      static UpperNode* deleteUpperList(UpperNode* nodeInHand);

      // fixes/ arranges/ appends the upper list as per the requirements.
      void fixUpperList(int upperLen);

      // get the length of the list p
      static int length(Node *nodeInHand); 

      //gets the length of upper list 
      static int length(UpperNode* nodeInHand);

      //copies all the elements of the provied list.
      static Node* copyList(const Node* nodeInHand);

      //copies all the elements of the upper list.
      static UpperNode* copyUpperList(const UpperNode* nodeInHand);
};
#endif  // SLIST_H
