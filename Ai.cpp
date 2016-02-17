#include <string>
#include <iostream>
#include <queue>
#include <algorithm>	// Library for 'copy' 

using namespace std;

/*
struct Node{					
	int state;
	Node* parentNode;
	string actions[4];
	int path_cost;
	int depth;
	int cost2go;

};
*/

struct Node{					
	int *state;		// The state that current Node is in	
	Node *parentNode;	// Pointer to parent Node of current Node
	
	/* Order of string array 'actions: {"up", "down", "left", "right"}
		String value for action will be "" if action not possible
		i.e. {"", "down", "left", "right"} if can't move up.
	*/
	//string *actions;	// Actions current state can take 
	string action;		// Last action taken to get to curr state

	int path_cost;		// Cost of path to current state of Node	
	int depth;		// Depth of current Node
	int cost2go;		// Estimated cost of path left from current state to goal state
};

Node *construct_node(int *state, Node *parentState, string action, int path_cost, int depth, int cost2go);

//TODO::
//Node ChildNode(Node n, action[goalstate]){
//	return NULL;	
//}
/*
Node ChildNode(Node n, string action, Node stateNode[]) {
	Node emptyNode;
	if (action == "left") {
		// Checks if n is in the left column
		if (n.state == 0 || n.state == 3 || n.state == 6) {
			return emptyNode;
		}
		else {
			// Left child of n
			return stateNode[n.state - 1];
		}
	}
	if (action == "right") {
		// Checks if n is in the right column
		if (n.state == 2 || n.state == 5 || n.state == 8) {
			return emptyNode;
		}
		else {
			// Right child of n
			return stateNode[n.state + 1];
		}
	}
	if (action == "up") {
		// Checks if n is in the upper row
		if (n.state == 0 || n.state == 1 || n.state == 2) {
			return emptyNode;
		}
		else {
			// Upper child of n
			return stateNode[n.state - 3];
		}
	}
	if (action == "down") {
		// Checks if n is in the lower row
		if (n.state == 6 || n.state == 7 || n.state == 8) {
			return emptyNode;
		}
		else {
			// Lower child of n
			return stateNode[n.state + 3];
		}
	}
}

//Check to see if state is goal state	
bool Goal_achieved(int state[]){
	bool equal = true;
	int goal[] ={0,1,2,3,4,5,6,7,8};
	int x = 0;
	while(x < 9){
		if(state[x] != goal[x]){
			equal = false;
		}		
	}
	return equal;
}

//Output::output path
Node Solution(Node n){	
	if(n.parentNode != NULL){
		Solution(*n.parentNode);
	}
	return n;
}

void print_to_screen(Node nodes[]){
	for(int i = 0; i < 9; ){
		for(int k = 0; k < 3; k++){
			cout << nodes[i].state << " ";
			i++;
		}
		cout << endl;
	}	
}*/

void print_to_screen(Node *state){
	cout << "State: " << endl;
	for(int i = 0; i<9; ){
		for(int j = 0; j<3; j++){		
			cout << state->state[i] << " ";
			i++;
	   	}
		cout << endl;
	}
}

void flip_tiles(int *stateTiles, string actionReq){
	/* Find blank tile. In this case, blank tile is
	      gauranteed, so no error checking needed.
	*/	
	int blankLocation;
	for (int i = 0; i<9; i++){
		if (stateTiles[i] == 0){
			blankLocation = i;
		}
	}

	/* Switch tiles. Actions requested are required to 
	      be valid before this function is called, so 
	      no error checking needed.
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
		// TODO error message?
	}
}

Node *ChildNode(Node *currNode, string actionReq){
	/* Node to be returned as child Node. 
	   Will be empty if action is not allowed */
	Node *childNode;		
	int *stateTiles = new int[9];
	
	/* Creates a copy of the current node's state
	      and saves it into the new child node */
	// TODO is there a better way to do this?
	int *oldStateTiles = currNode->state;
	int *end = &oldStateTiles[9];
	int *start = &oldStateTiles[0];	
	std::copy(start, end, stateTiles);  
	//childNode->state = stateTiles;

	// TODO How will we do these? dummy values for now
	int path_cost = (currNode->path_cost)++;
	int depth = (currNode->depth)++;
	int cost2go = (currNode->cost2go)++;
	
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
		// TODO error message? 
	}


	return childNode;
}

int main(){

	// Initialize beginning state
	int initState[] = {5,0,4,2,1,3,6,7,8};	
	Node *startNode = construct_node(initState, NULL, "", 0, 0, 0);

	// Initialize goal state
	int goalState[] = {0,1,2,3,4,5,6,7,8};

	// TODO remove test
	//print_to_screen(startNode);

	// Define explored and frontier priority queues
	priority_queue<int> explored;
	priority_queue<int> frontier;

	// Generate child nodes
	Node *child = ChildNode(startNode, "down");
	
	cout<<"Start: "<<endl;
	print_to_screen(startNode);
	cout<<"Child: "<<endl;
	print_to_screen(child);

	/*
	int state[] = {5,0,4,2,1,3,6,7,8};
	Node stateNode[9];
	priority_queue<int> explored;
	priority_queue<int> frontier;
	string act[] = {"up", "down", "left", "right"};
 	
 	/**
 	* Instantiating new nodes in Array
	*
 	for(int i = 0; i < 9; i++){
 		stateNode[i].state = state[i];
 		stateNode[i].parentNode = NULL;
 		stateNode[i].path_cost = 1;
 	}

 	print_to_screen(stateNode);

 	return 0;
	*/

	// Deallocation of memory
	delete startNode;
	delete child;

 	return 0;
}

Node *construct_node(int *state, Node *parentState, string action, int path_cost, int depth, int cost2go){

	// Allocate memory for new node
	Node *newNode = new Node;	

	newNode->state = state;
	newNode->parentNode = parentState;
	newNode->action = action;
	newNode->path_cost = path_cost;
	newNode->depth = depth;
	newNode->cost2go = cost2go;

	return newNode;
}

