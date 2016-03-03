// NodeFuncsLib.h
//#ifndef NODEFUNCS_H
#define NODEFUNCS_H

#include <string>
#include <iostream>
#include <queue>
#include <algorithm>	// Library for 'copy' 
#include <climits>		// Library for 'INT_MAX'
using namespace std;

namespace NodeFuncs{
	class MyNodeFuncs{
		public: 
			/* Structure representing each state that the 8-puzzle could being in */			
			struct Node{					
				int *state;			// The state that current Node is in	
				Node *parentNode;	// Pointer to parent Node of current Node
				string action;		// Last action taken to get to curr state
	
				int path_cost;		// Cost of path to current state of Node	
				int depth;			// Depth of current Node
				int cost2go;		// Estimated cost of path left from current state to goal state
			};

			/* Constructs a new Node and returns a pointer to the new Node */
			Node *construct_node(int *state, Node *parentState, string action, int path_cost, int depth, int cost2go);

			/* Performs an A* Star search given a current state and returns a pointer to the goal Node.
				Returns NULL if not found. 
			*/	
			Node *AStarSearch(Node *current, priority_queue<Node> *explored, priority_queue<Node> *frontier);

			/* Performs an IDDFS search given a current state and returns a pointer the goal Node.  
				Returns NULL if not found 
			*/
			Node *IDDFS(Node *current,  priority_queue<Node> *frontier, int cutoff);

			/* Returns the cost to go given a current state */
			int Cost2Go(Node *current);

			/* Determine whether state {@code compState} is in priority_queue {@code q}. Returns NULL if not found */
			Node *InQueue(Node *compState, priority_queue<Node> *q);

			/* Swaps a given state contained within priority_queue {@code q} with state {@code swapState} */
			void Swap(Node *swapState, priority_queue<Node> *q);

			/* Compares states {@code cmp1} and {@code cmp2). Returns boolean value. */			
			bool CompareStates(Node *cmp1, Node *cmp2);

			/* Prints the given {@code state} to console */
			void print_to_screen(Node *state);

			/* Prints information about given {@code state} as well as action performed to achieve state. */ 
			void print_node_meta(Node *state, string action);;

			/* Flips two tiles given within state of tiles {@code stateTiles} in the direction of {@code actionReq} */
			void flip_tiles(int *stateTiles, string actionReq);

			/* Returns pointer to the child Node of state {@code currNode} with the given action {@code actionReq} performed */
			Node *ChildNode(Node *currNode, string actionReq);

			/* Checks whether given state {@code current} is the desired goal state. Goal state is always {0,1,2,3,4,5,6,7,8,9} */
			bool GoalCheck(Node *current);

			/* Prints the states reached from state {@code n} to desired goal state. */
			void Solution(Node *n);
			
	};
}

//#endif
