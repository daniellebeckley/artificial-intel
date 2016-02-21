#include "NodeFuncsLib.h"

#include <string>
#include <iostream>
#include <queue>
#include <algorithm>	// Library for 'copy' 
#include <climits>		// Library for 'INT_MAX'
using namespace std;

namespace NodeFuncs{
	Node* MyNodeFuncs::construct_node(int *state, Node *parentState, string action, int path_cost, int depth, int cost2go){
		// Allocate memory for new node
		Node *newNode = new Node;	

		/* Assign values to newNode */
		newNode->state = state;
		newNode->parentNode = parentState;
		newNode->action = action;
		newNode->path_cost = path_cost;
		newNode->depth = depth;
		newNode->cost2go = cost2go;
	
		return newNode;
	}

	
	Node* MyNodeFuncs::AStarSearch(Node *current, priority_queue<Node> *explored, priority_queue<Node> *frontier){
		frontier = new priority_queue<Node>;
		explored = new priority_queue<Node>;

		/* Begin search with current node */
		frontier->push(*current);

		/* Search until no nodes are visible from current state */
		Node *curr = current;
		while(!frontier->empty()){
			curr = new Node(frontier->top());
		
			/* If current state == goal state, return current state */
			if (GoalCheck(curr)){
				cout << "You have reached the goal." << endl;
				return curr;	
			} 

			/* Remove 'curr' node from frontier, and move to explored queue */
			frontier->pop();
			explored->push(*curr);
			
			/* For each direction child of current state */
			string actions[] = {"up","down","left","right"};
			Node *child;
			for (int i=0; i<4; i++){
				// Retrieve child node of 'curr' for given action performed 
				child = ChildNode(curr, actions[i]);

				/* If child isn't Null.. */
				if (child != NULL){
					/* Update path_cost of child state */
					child->path_cost = child->cost2go + child->parentNode->path_cost + 1;
	
					/* If Child isn't in frontier or explored queues, add to frontier */
					if ((InQueue(child, frontier) == NULL) && (InQueue(child, explored) == NULL)){
						frontier->push(*child);
					}

					/* If child is in frontier, and path_cost value is greater than current, swap in frontier */					
					else if (InQueue(child, frontier) != NULL) {
						if ((InQueue(child, frontier)->path_cost) > (child->path_cost)){
							Swap(child, frontier);
						}				
					}
				}
				child = NULL;
			}
		}

		/* Return NULL if no goal state achieved */	
		return NULL;
	}

		
	Node *MyNodeFuncs::IDDFS(Node *current, priority_queue<Node> *frontier, int cutoff){
		frontier = new priority_queue<Node>;
		explored = new priority_queue<Node>;
		
		/* Begin search with current state */
		frontier->push(*current);
	
		/* Search until no nodes are visible from current state */
		Node *curr = current;
		Node *temp;
	
		/* While the frontier queue isn't empty, and cutoff depth hasn't been reached.. */
		while(!frontier->empty() && frontier->top().depth <= cutoff-1){
			// Get first value from frontier queue			
			curr = new Node(frontier->top());
	
			/* If current state == goal state, return */
			if (GoalCheck(curr)){
				cout << "You have reached the goal." << endl;
				return curr;	
			} 
	
			/* While the current state's depth != cutoff.. */
			while(curr->depth < cutoff){
				// Remove curr from frontier				
				frontier->pop();
			
				/* For each direction child of current state.. */
				string actions[] = {"up","down","left","right"};
				Node *child;
				for (int i=0; i<4; i++){
					child = ChildNode(curr, actions[i]);
					
					/* If child isn't NULL and depth is less than the cutoff limit.. */
					if (child != NULL && child->depth <= cutoff){
						/* If child is in frontier, update depth and push to frontier queue */
						if (InQueue(child, frontier) == NULL){
							child->depth = 1 + child->parentNode->depth;
							frontier->push(*child);
						}
						/* If child is in frontier, and depth is greater than current child depth, swap in frontier queue */
						else if ((InQueue(child, frontier)->depth) > (child->depth)){
							Swap(child, frontier);
						}
					}
				}
				
				/* Update 'curr' pointer top of frontier queue */
				curr = new Node(frontier->top());		
				child = NULL;
			}
			
			/* Returns goal state from frontier if found */			
			temp = new Node(frontier->top());
			while(temp->depth == cutoff && !frontier->empty()){
				if (GoalCheck(temp)){
					cout << "You have reached the goal." << endl;
					return temp;	
				}
				else{
					frontier->pop();
					temp = NULL;
				}
	
				temp = new Node(frontier->top());
			}
	
		}
		return NULL;
	}
	
	int MyNodeFuncs::Cost2Go(Node *current){
		int sum = 0;
	
		// Cost array for each number in state
		int costs[9][9] = {
			{ 0,1,2,1,2,3,2,3,4 }, //zero costs
			{ 1,0,1,2,1,2,3,2,3 }, //one costs
			{ 2,1,0,3,2,1,4,3,2 }, //two costs
			{ 1,2,3,0,1,2,1,2,3 }, //three costs
			{ 2,1,2,1,0,1,2,1,2 }, //four costs
			{ 3,2,1,2,1,0,3,2,1 }, //five costs
			{ 2,3,4,1,2,3,0,1,2 }, //six costs
			{ 3,2,3,2,1,2,1,0,1 }, //seven costs
			{ 4,3,2,3,2,1,2,1,0 }  //eight costs
		};
	
		/* Find manhattan distance */
		for (int i = 0; i < 9; i++) {
			int x = current->state[i];
			sum = sum + costs[i][x];
		}
	
		return sum;
	}

	Node *MyNodeFuncs::InQueue(Node *compState, priority_queue<Node> *q){
		Node *match = NULL;
		priority_queue<Node> *temp = new priority_queue<Node>;
		int qSize = q->size();	
	
		/* For each state in priority queue */
		for (int i = 0; i < qSize ; i++){	
			Node *curr = new Node(q->top());
			q->pop();
			bool tileMatch = CompareStates(compState, curr);
			temp->push(*curr);		
	
			/* State is the same */
			if (tileMatch){
				match = curr;
			}
		}
	
		/* Add values back to queue */
		for (int j=0; j < qSize; j++){
			Node *curr = new Node(temp->top());
			temp->pop();
			q->push(*curr);
		}
	
		return match;
	}

	void MyNodeFuncs::Swap(Node *swapState, priority_queue<Node> *q){
		priority_queue<Node> *temp;
		temp = new priority_queue<Node>;	

		/* Checks if state is in 'q', if so, swaps with 'swapState' */	
		for (int i=0; i < q->size(); i++){
			Node *curr = new Node(q->top());
			q->pop();		
			if (!CompareStates(swapState, curr)){
				temp->push(*curr);	
			}
			else{
				temp->push(*swapState);
			} 
		}
	
		q = temp;
	}

	bool MyNodeFuncs::CompareStates(Node *cmp1, Node *cmp2){
		int *cmpTiles1 = cmp1->state;
		int *cmpTiles2 = cmp2->state;
		bool tileMatch = true;
	
		/* Compares each tile in states arrays, if any don't match, return false */
		for(int i=0; i<9; i++){
			if (cmpTiles1[i] != cmpTiles2[i]){
				tileMatch = false;
			}
		}
	
		return tileMatch;
	}

	void MyNodeFuncs::print_to_screen(Node *state){
		cout << "Step " << state->depth <<": " << state->action << endl;				
		
		/* Print each state */
		for(int i = 0; i<9; ){
			for(int j = 0; j<3; j++){		
				cout << state->state[i] << " ";
				i++;
		   	}
			cout << endl;
		}
	}	

	void MyNodeFuncs::print_node_meta(Node *state, string action){
		/* Print info about 'state' and 'action' */
		cout << "-- For: " << action << endl;
		cout << "\tpath_cost: " << state->path_cost << endl;
		cout << "\tdepth: " << state->depth << endl;
		cout << "\tcost2go: " << state->cost2go << endl;
	}

	void MyNodeFuncs::flip_tiles(int *stateTiles, string actionReq){
		/* Find blank tile. In this case, blank tile is
		      gauranteed, so no error checking needed.
		*/	
		int blankLocation;
		for (int i = 0; i<9; i++){
			if (stateTiles[i] == 0){
				blankLocation = i;
			}
		}
	
		/* Switch tiles for given action, 'actionReq'.
		*/
		int oldTile;
		if (actionReq == "up") {
			oldTile = stateTiles[blankLocation-3];
			stateTiles[blankLocation-3] = 0;
			stateTiles[blankLocation] = oldTile;
		}
		else if (actionReq == "down") {
			oldTile = stateTiles[blankLocation+3];
			stateTiles[blankLocation+3] = 0;
			stateTiles[blankLocation] = oldTile;
		}
		else if (actionReq == "left") {
			oldTile = stateTiles[blankLocation-1];
			stateTiles[blankLocation-1] = 0;
			stateTiles[blankLocation] = oldTile;
		}
		else if (actionReq == "right") {
			oldTile = stateTiles[blankLocation+1];
			stateTiles[blankLocation+1] = 0;
			stateTiles[blankLocation] = oldTile;
		}
		else {
			cout << ("Error");
	    	exit (EXIT_FAILURE);
		}
	}
	
	Node *MyNodeFuncs::ChildNode(Node *currNode, string actionReq){
		/* Node to be returned as child Node. 
		   Will be empty if action is not allowed */
		Node *childNode;		
		int *stateTiles = new int[9];
		
		/* Creates a copy of the current node's state
		      and saves it into the new child node */
		int *oldStateTiles = currNode->state;
		int *end = &oldStateTiles[9];
		int *start = &oldStateTiles[0];	
		std::copy(start, end, stateTiles);  

		/* Copy current values from state 'currNode' */
		int path_cost = currNode->path_cost;
		int depth = currNode->depth;
		int cost2go = currNode->cost2go;
		depth++;
		
		/* Perform action required by flipping tile with adjacent direction
			Then, increment path_cost, depth and cost2go */
		if (actionReq == "up") {
			// Checks if 0 is in the top row
			if ((stateTiles[0] == 0)||(stateTiles[1] == 0)||(stateTiles[2] == 1)) {
				childNode = NULL;
			}
			else {
				// Left child of 0
				flip_tiles(stateTiles, actionReq);
				childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
			}
		}
		else if (actionReq == "down") {
			// Checks if 0 is in the bottom row
			if ((stateTiles[6] == 0)||(stateTiles[7] == 0)||(stateTiles[8] == 1)) {
				childNode = NULL;
			}
			else {
				// Right child of 0
				flip_tiles(stateTiles, actionReq);
				childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
			}
		}
		else if (actionReq == "left") {
			// Checks if 0 is in the left column
			if ((stateTiles[0] == 0)||(stateTiles[3] == 0)||(stateTiles[6] == 1)) {
				childNode = NULL;
			}
			else {
				// Upper child of 0
				flip_tiles(stateTiles, actionReq);
				childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
			}
		}
		else if (actionReq == "right") {
			// Checks if 0 is in the right column
			if ((stateTiles[2] == 0)||(stateTiles[5] == 0)||(stateTiles[8] == 1)) {
				childNode = NULL;
			}
			else {
				// Lower child of 0
				flip_tiles(stateTiles, actionReq);
				childNode = construct_node(stateTiles, currNode, actionReq, path_cost, depth, cost2go);
			}
		}
		else{
			cout << ("Error");
    			exit (EXIT_FAILURE);
		}
		
		/* Update 'cost2go' of childNode */
		if (childNode != NULL) {
			childNode->cost2go = Cost2Go(childNode);
		}
	
		return childNode;
	}

	bool MyNodeFuncs::GoalCheck(Node *current){
		int goal[] ={0,1,2,3,4,5,6,7,8};
		Node *goalState = construct_node(goal, NULL, "", 0, 0, 0);
		/* Return (current == goalState) */	
		return CompareStates(current, goalState);
	}


	void *MyNodeFuncs::Solution(Node *n){	
		/* Print each state from state 'n' to goal */
		if(n->parentNode != NULL){
			Solution(n->parentNode);
		}
		print_to_screen(n);
		return n;	
	}
}
