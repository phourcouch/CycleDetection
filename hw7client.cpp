// CS311 YOSHII F23 hw7client.cpp

//==========================================
// HW#: HW7 DFS client using dgraph.h
// Name: Kayla Le
// File type: client hw7client.cpp 
//==========================================

// ** Be sure to include both dgraph  and stack
#include "dgraph.h"
#include "stack.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// This client uses the dgraph class and stack class
// to do depth first traversal of the graph stored in table.txt
int main()
{
  // ** copy here the algorithm (read carefully) in the HW7
  // assignment sheet and use that for the comments. 
  dgraph D;//dgraph object
  stack S;  //stack object 
  int currentVisitNum = 1;//visit number used to store into a vertex
  char removedChar; //holder for removed char from stack
  char removedAdj;//holder for removed char from adjecent slist object
  slist adjOnes; //slist object
  vector<char> path;//vector that holds path 
  char stackTop;//holds the topElem of the stack
 
   D.fillTable(); //filling table

  D.displayGraph();//displaying table

  S.push('A'); //push A onto stack to start
  
  while(!S.isEmpty()){//while stack is not empty
    
      S.pop(removedChar);//remove a vertex from the stack

      cout << "Removed " << removedChar << " from stack\n";//display vertex name

      path.push_back(removedChar);//adding to path

      D.displayPath(path);//displaying path 

       if(!D.isMarked(removedChar)){//if vertex is not marked -- starting of if

           D.visit(currentVisitNum,removedChar);//marking vertex
         
           cout << "Visit " << removedChar << " as " << currentVisitNum << endl; //informing user
     
           adjOnes = D.findAdjacency(removedChar);//get adjacency list of vertex
   
      	          if(adjOnes.isEmpty()){//if adjacency list is empty - inform used       		
			 cout << "Deadend reached - backup\n";//notifying client of deadend
		        path.pop_back();//removing from path 

			if(!S.isEmpty()){//if the stack is not empty, get the top element of the stack and then call endOfpath to check if this deadend is the end of a path
                        S.topElem(stackTop);
		        D.endOfPath( path,stackTop);
                         }
	           }
       		else{
       		 cout << "...pushing ";
        	 adjOnes.displayAll();//displaying adjacency list
         
		//pushing  adjcent verticies of current vertex  onto stack 
 		   for(int i = 0; i < D.findOutDegree(removedChar);i++){//loop through adjaceny list
        	      adjOnes.deleteRear(removedAdj);//deleting from rear and storing removed char into removedAdj
           	       S.push(removedAdj);  //pushing removed adjcent vertex into stoack
                 }
         
               }
       currentVisitNum++; //updating visit number
       }//end of if
     else{//inform user that a vertex has already been visited 
        cout << removedChar << " is marked already\n";//informing client
      
        path.pop_back();//removing from path
	D.detectCycle(removedChar,path);//calling detectCycle to see if the removedChar is currenlty in the path
        cout << "Backup\n";//infromaing client
    
     
          if(!S.isEmpty() && !D.allAdjOnesVisited(path.front())){//if the allAdjOnesVisited of the front of the path is not true and the stack is not empty, then get the top Element of the stack and call endofpath
             S.topElem(stackTop);
             D.endOfPath(path,stackTop);
          }
         else if(S.isEmpty() && D.allAdjOnesVisited(path.front())){//if the stack is empty and all of path front's adj vetices have been visited call end of path for the rest of the vertices 
           D.endOfPath(path,removedChar);//call end of path 
         }
     }//end of else 
   // } 
     S.displayAll();//displaying stack
     }//end of while  loop
   
  D.displayGraph(); //displaying graph with visit numbers for all vertices
}

//extra credit

//PURPSOEL: check if all of given vertex's adjOnes have been visited - if all visited return true, otherwise false
//PARA: vertex client wants to check if allAdjones are visited
bool dgraph::allAdjOnesVisited(char V){

   int charIndex = convertChartoInt(V);//convert given vertex to index slot
   int totalTrue = 0;//holds the amount of adjones that are visited 
   char removedAdj;//holds the char of removedAdj
   int adjIndex;//hold removedAdj's index
   slist adjOnes = Gtable[charIndex].adjacentOnes;//holds the slist of the given vertex adjacent ones 

     for(int i = 0; i < Gtable[charIndex].outDegree;i++){//loop for the amount of the outDegree of the given vertex 
        adjOnes.deleteRear(removedAdj);//get Adj  char from slist
        adjIndex = convertChartoInt(removedAdj);//get index of removedAdj char


       if(Gtable[adjIndex].visit != 0)//check if removed Adj is visited  - if visited add 1 to totalTrue 
         totalTrue++;
     }//end of for loop

     if(totalTrue == Gtable[charIndex].outDegree){//if the totalTrue and out degree of the given vertex are equal reutrn true- otherwise returrn false 
          return true;
        }
     else
       return false;
}
//PURPOSE: display current path
//PARA: vector of path
void dgraph::displayPath(const vector<char> v){

  cout << "Current Path: ";//inform client that this is the current path 

 for(int i = 0; i < (int)v.size();i++)//loop through all char in path (size)
   cout <<  v.at(i);//print out char at the location of i 

  cout << endl;
}
//PURPOSE: detect a  Cycle in graph
//PARA: char that you are checking if it is in current path, and also paassing the path 
void dgraph::detectCycle(char V, const vector<char> v){


  for(int i = (int)v.size()-1; i >= 0;i--){//loop through size of path - 1 because you dont want to count the char you are checking 

   if(v.at(i) == V)//if the given char from client is found in path print out that a cycle has been found
        cout << "@@@Cycle is found to it.\n";

   }


}

//PURPOSE: notifys client of end of path of a certain vertex
//PARA: pass path and the child of the vertex you are checking is end of path 
void dgraph::endOfPath( vector<char>& Path,char child){

  char removedAdj;//holds removed Adj char
 int totalTrue = 0;//holds the amount of adjOnes that are visited
   int charIndex = convertChartoInt(Path.back());//holds index of the char that is at the end of path
  int adjIndex;//holds index of removed Adj char
  bool childFound = false;//checking if removedAdj is equal to child 
  slist adjOnes = Gtable[charIndex].adjacentOnes;//get adjOnes list of parent (back of path)


   for(int i = 0; i < Gtable[charIndex].outDegree;i++){//loop through parent's out degrees 
     adjOnes.deleteRear(removedAdj);//get char from slist
     adjIndex = convertChartoInt(removedAdj);   //getting index of it


    if(removedAdj == child)//mark child found true if the current removedAdj is true 
      childFound = true;


     if(Gtable[adjIndex].visit != 0)//if the current adj vertex has been visited, add 1 to total amount of visited adj vertices 
      totalTrue++;
   }


   if(totalTrue == Gtable[charIndex].outDegree ){//if the amount of visited adj is equal to parents out degree, mark parent as end of path
      cout << "-------------------------------------\nEnd of Path from " << Path.back() << " so backup.\n-------------------------------------------\n";
      Path.pop_back();
    }

//if the total amount of visited adj is not equal to parents out degree or if the child wasnt found, return 
    if(totalTrue != Gtable[charIndex].outDegree ||  childFound != true )
    return;


   endOfPath(Path,child);//resurive so that you can go until no more end of path vertices in path 
}

