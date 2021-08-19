// Name: HARSH RANA
// ID - 100351582
// CPSC 2150, Assignment #5

#include "SList.h"
#include <climits>
#include <cassert>
#include <iostream>
#include <cmath>


#include <ctime>   // time
#include <iomanip> // setw


// separates the elements when using the operator << 
const std::string SList::SEP = std::string(" ");
/**
 * Creates an empty list using nodes
 * POSTCONDITION: an empty skip list is created
 */
SList::SList(){
    upperHead = nullptr;
    DumUpperHead = new UpperNode{nullptr,upperHead};// dummy head for ease in inserting

    head = nullptr;
    DumHead = new Node{INT_MIN,head};// dummy head for ease in inserting

    outputTopList = false;// by default output the lower list
}

/**
 * Checks if the list is empty.
 * POSTCONDITION: Returns true if the list is empty or vice versa.
 */
bool SList::isEmpty() const{
    return head == nullptr;
}

//HELPER FOR numberOfElements
/**
 * get the length of the list p
 */
int SList::length(Node* nodeInHand){
    if (nodeInHand == nullptr){
      return 0;
    }
    nodeInHand = nodeInHand->link;
    return (1 + length(nodeInHand));
}

int SList::length(UpperNode* nodeInHand){
    if (nodeInHand == nullptr){
      return 0;
    }
    nodeInHand = nodeInHand->link;
    return (1 + length(nodeInHand));
}

/**
 * return the number of elements in the list
 * POSTCONDITION: An int value equivalent to the length of the list is returned.
 */
int SList::numberOfElements() const{
    Node* nodeInHand = head;
    return length(nodeInHand);
}

/**
 * used to keep a track of which list to output.
 * POSTCONDITION: If true, upperlist will be output when the output operator is called
 */
void SList::setTopListOutput(bool topListDumping){
    outputTopList = topListDumping;

}

/**
 * used to keep a find out which list will be output.
 * POSTCONDITION: returns true if upper list will be output on calling the output operator
 */
bool SList::getTopListOutput() const{
    return outputTopList;
}

//OUTPUT OPERATOR
/**
 *outputs elements of the list in a stream in the exact same order as they are present
 * eg 1 2 3 
 * POSTCONDITION: output in the order in which they are in the list
 * POSTCONDITION: outputs the upper list if setTopListOutput(true) has been used,else outputs lower lsit
 */
std::ostream& operator<< (std::ostream& out, const SList& list){
    if(list.outputTopList == false){
        SList::Node* refference = list.head;
        while(refference != nullptr){
        out << refference->item << SList::SEP;
        refference = refference->link;
        }
    }
    else{
        SList::UpperNode* refference = list.upperHead;
        while(refference != nullptr){
        out << refference->item->item << SList::SEP;
        refference = refference->link;
        }
    }

    return out;
}

/**
 * Deletes each and every element from the list.
 * POSTCONDITION: The provided list is now empty.
 */
SList::Node* SList::deleteList(Node* p){
    if(p == nullptr){
        return nullptr;
    }
    Node* toBeRemoved = p;
    p = p->link;
    delete toBeRemoved;
    toBeRemoved = nullptr;
    return deleteList(p);

}

/**
 * Deletes each and every element from the upper list.
 * POSTCONDITION: The provided list is now empty.
 * NOTE: two different functions amde because upper and lower functions are not the same
 */
SList::UpperNode* SList::deleteUpperList(UpperNode* p){
    if(p == nullptr){
        return nullptr;
    }
    UpperNode* toBeRemoved = p;
    p = p->link;
    delete toBeRemoved;
    toBeRemoved = nullptr;
    return deleteUpperList(p);

}

/**
 * Resets the skip list by deleting all the data present.
 * The list is still useable after calling reset();
 * POSTCONDITION: The list has now been renewed. all old data erased!
 */
void SList::reset(){
    upperHead = deleteUpperList(upperHead);
    head = deleteList(head);
    // all the memory has been deallocated 
    // dummy head nodes and head nodes are reset to make the list reusable
    upperHead = nullptr;
    DumUpperHead = new UpperNode{nullptr,upperHead};// dummy head for ease in inserting

    head = nullptr;
    DumHead = new Node{INT_MIN,head};// dummy head for ease in inserting
}

//DESTRUCTOR
/**
 * Is called automatically when the program terminates to cleanup.
 * POSTCONDITION: Deletes a list.
 */
SList::~SList(){
    upperHead = deleteUpperList(upperHead);
    head = deleteList(head);

    outputTopList = false;
}

/**
 * fixes, that is, rearranges, reorders and even adds new nodes in the upper list 
 * if necessary.
 * PRECONDITION: Provided input is correct
 * POSTCONDITION: All the elements in the list are now evenly spaced and there are sqrt(n) nodes
 */
void SList::fixUpperList(int upperLen){
    UpperNode* upperInHand = DumUpperHead;
    Node* nodeInHand = DumHead;
    int n = upperLen+1;
    if(upperLen == 0){// special case when first node is added
    // this is the only time we worry about the first node in the upper list.
    //In all other cases we ignore the first node and fix the rest.
    // This is done because the first node's element will always be -
    // - the head of the lower list
        upperInHand->link = new UpperNode{nodeInHand, upperInHand->link};
    }
    else{
        upperInHand = upperInHand->link;
        nodeInHand = nodeInHand->link;
        int j = 0;
        while(nodeInHand != nullptr){
            if(j == n){
                if(upperInHand->link == nullptr){
                    upperInHand->link = new UpperNode{nodeInHand, upperInHand->link};
                }
                else{
                    upperInHand = upperInHand->link;
                    upperInHand->item = nodeInHand;
                }
                j = 0;
            }
            nodeInHand = nodeInHand->link;
            j++;
        }
    }
    // first node of the upper list will be fixed after exiting this function
}

/**
 * Inserts a new element into the skip list in ascending order
 * Takes care of the upper level aswell.
 * POSTCONDITION: x is inserted in appropriate position to keep the skip list ascending.
 */
void SList::insert(int x){
    Node* nodeInHand = DumHead;
    while(nodeInHand->link != nullptr && nodeInHand->link->item < x){
        nodeInHand = nodeInHand->link;
    }
    nodeInHand->link = new Node{x,nodeInHand->link};
    head = DumHead->link;

    int lowerLen = length(head);
    int upperLen = length(upperHead);

    //std::cout << "added in lower current length = " << len << std::endl;

    if(floor(sqrt(lowerLen)) == upperLen+1){
        fixUpperList(upperLen);
        //std::cout << "added in upper current length = " << upperLen << std::endl;
    }
    
    // 1st node will always point at the head
    upperHead = DumUpperHead->link;
    // this line is necessary as while creation,
    // dummyupperhead->link was pointing to nullptr && upperhead was pointing to dummyupperhead->link which was also null pointer
    // I have realised that these two null pointers aren't the same. a null pointer is a null pointer.
    // I donot know if I am making sense but after creation, if I change dummyupperhead->link, my upper head won't change.
    upperHead->item = head;// takes care of the first element of upper list

}

/**
 * searches for the given element in the Skip list.
 * POSTCONDITION: Returns true if the element was found in the skip list else returns false.
 */
bool SList::search(int x) const{
    
    UpperNode* upperInHand = DumUpperHead;
    Node* nodeInHand = DumHead;
    if(nodeInHand == nullptr){
        return false;
    }// means there is atleast one element in both the lists
    while(upperInHand->link != nullptr && upperInHand->link->item->item <= x){
        if(upperInHand->link->item->item == x){
            return true;
        }
        upperInHand = upperInHand->link;
        nodeInHand = upperInHand->item;
    }
    while(nodeInHand != nullptr && nodeInHand->item <= x){
        if(nodeInHand->item == x){
            return true;
        }
        nodeInHand = nodeInHand->link;
    }
    return false;
}

//copies all the elements of the provied list.
SList::Node* SList::copyList(const Node* nodeInHand){
    if(nodeInHand == nullptr){
        return nullptr;
    }
    return new Node{nodeInHand->item,copyList(nodeInHand->link)};
}

//copies all the elements of the upper list.
SList::UpperNode* SList::copyUpperList(const UpperNode* nodeInHand){
    if(nodeInHand == nullptr){
        return nullptr;
    }
    return new UpperNode{nodeInHand->item,copyUpperList(nodeInHand->link)};
}

//ASSIGNMENT OPERATOR
/**
 * deletes old data and copies new data into the list.
 * PRECONDITION: Sufficient heap memory is present
 * POSTCONDITION: provided list is over-written with the contents of current list
 */     
SList& SList::operator = (const SList& rhs){
    if(this->head != nullptr){
        this->reset();
    }
    head = copyList(rhs.head);
    upperHead = copyUpperList(rhs.upperHead);
    
    DumUpperHead = new UpperNode{nullptr,upperHead};// dummy head for ease in inserting
    DumHead = new Node{INT_MIN,head};// dummy head for ease in inserting
    return *this;
}

//COPY CONSTRUCTOR
/**
 * creates a copy of the provided list
 * PRECONDITION: Sufficient heap memory is present
 * POSTCONDITION: A copy of the list is created
 */
SList::SList(const SList& rhs){
    head = copyList(rhs.head);
    upperHead = copyUpperList(rhs.upperHead);

    DumUpperHead = new UpperNode{nullptr,upperHead};// dummy head for ease in inserting
    DumHead = new Node{INT_MIN,head};// dummy head for ease in inserting
}

int main(){
   SList list; // the home made skip list, used later for the tests too

   // made an array of test cases to make them easier to use
   const int TRIAL_CASES = 5;
   int TRIALS[TRIAL_CASES] = {1000,2000,4000,8000,16000};

   // this is one large array which will be filled each time when rq.
   const int SIZE = 16000;
   int A[SIZE];

   // bool to store the return value of search 
   bool answer;
   double itetrations = 1000.00;// A double so it doesnt cause problems while dividing


   std::cout << "\t"<< "FOR AN ARRAY SORTED IN ASCENDING ORDER" << std::endl ;
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //ASCENDING ORDER
   for(int i = 0; i < SIZE; i++){// array being filled in ascending order
      A[i] = i;
   }

   for(int j = 0; j < TRIAL_CASES; j++){// makes sure each test case (n) is run
      for(int i = 0; i < TRIALS[j]; i++){// only inserts n elements into the skip list. n is the test case
         list.insert(A[i]);
      }

      unsigned int start = clock();// starts counting time

      for(int i = 0; i < itetrations; i++){// iterates the same process 
         for(int m = 0; m < TRIALS[j]; m++){// to make sure each element is searched one by one from the array.
         answer = list.search(A[m]);
         }
      }
    
      unsigned int stop = clock();// this will be the time taken.

      double diff = static_cast<double>(stop-start);
      double timeTakenPerSearch = (diff/TRIALS[j])/itetrations;//multiple divisions to make sure we get average time of each search

      std::cout << "Average time to perform a search in " << TRIALS[j] <<" elements = " << timeTakenPerSearch << std::endl;
      list.reset();// list is reset so it can be filled with new elements again
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   std::cout << "\n" << "\t"<< "FOR AN ARRAY SORTED IN DESCENDING ORDER" << std::endl ;
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //DESCENDING ORDER
   
   int val = SIZE;// largest value to start from
   for(int i = 0; i < SIZE ; i++){// array being filled in descending order
      A[i] = val;
      val--;// keeps reducing as we move forward.
   }

   for(int j = 0; j < TRIAL_CASES; j++){// makes sure each test case (n) is run
      for(int i = 0; i < TRIALS[j]; i++){// only inserts n elements into the skip list. n is the test case
         list.insert(A[i]);
      }

      unsigned int start = clock();
      for(int i = 0; i < itetrations; i++){
         for(int m = 0; m < TRIALS[j]; m++){    // NOTHING NEW
             answer = list.search(A[m]);
         }
      }
    
      unsigned int stop = clock();
      double diff = static_cast<double>(stop-start);
      double timeTakenPerSearch = (diff/TRIALS[j])/itetrations;

      std::cout << "Average time to perform a search in " << TRIALS[j] <<" elements = " << timeTakenPerSearch << std::endl;
      list.reset();
   }

    
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   std::cout << "\n" << "\t"<< "FOR AN ARRAY SORTED IN RANDOM ORDER" << std::endl ;
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //RANDOM ORDER
   
   srand (time(NULL));

   for(int i = 0; i < SIZE ; i++){// array being filled in random order
       A[i] = (rand()%300); // mod with 300 so some values repete, they are small, and in a mixed order
   }

   for(int j = 0; j < TRIAL_CASES; j++){// makes sure each test case (n) is run
      for(int i = 0; i < TRIALS[j]; i++){// only inserts n elements into the skip list. n is the test case
         list.insert(A[i]);
      }

      unsigned int start = clock();
      for(int i = 0; i < itetrations; i++){
         for(int m = 0; m < TRIALS[j]; m++){    // NOTHING NEW
             answer = list.search(A[m]);
         }
      }
    
      unsigned int stop = clock();
      double diff = static_cast<double>(stop-start);
      double timeTakenPerSearch = (diff/TRIALS[j])/itetrations;

      std::cout << "Average time to perform a search in " << TRIALS[j] <<" elements = " << timeTakenPerSearch << std::endl;
      list.reset();
   }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   std::cout << "\n" << "\t"<< "SEARCHING THE FIRST ELEMENT IN THE LIST" << std::endl ;
   // BEST CASE

   // array already has random values

   A[0] = 1;//first element is now 1

   for(int j = 0; j < TRIAL_CASES; j++){// makes sure each test case (n) is run
      for(int i = 0; i < TRIALS[j]; i++){// only inserts n elements into the skip list. n is the test case
         list.insert(A[i]);
      }

      unsigned int start = clock();
      for(int i = 0; i < itetrations; i++){
         answer = list.search(1); // Always search for the first element
      }
    
      unsigned int stop = clock();
      double diff = static_cast<double>(stop-start);
      double timeTakenPerSearch = (diff/TRIALS[j])/itetrations;

      std::cout << "Average time to perform a search in " << TRIALS[j] <<" elements = " << timeTakenPerSearch << std::endl;
      list.reset();
   }

    
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


   return 0;
}

