#include <string>
#include <iostream>
#include <queue>
#include <algorithm>	// Library for 'copy' 
#include <climits>	// Library for 'INT_MAX'

using namespace std;

struct Node{					
	int *state;		// The state that current Node is in	
	Node *parentNode;	// Pointer to parent Node of current Node
	string action;		// Last action taken to get to curr state

	int path_cost;		// Cost of path to current state of Node	
	int depth;		// Depth of current Node
	int cost2go;		// Estimated cost of path left from current state to goal state
};

Node *construct_node(int *state, Node *parentState, string action, int path_cost, int depth, int cost2go);
Node *AStarSearch(Node *current, priority_queue<Node> *explored, priority_queue<Node> *frontier);
int Cost2Go(Node *current);
Node *InQueue(Node *compState, priority_queue<Node> *q);
bool CompareStates(Node *cmp1, Node *cmp2);
void Swap(Node *swapState, priority_queue<Node> *q);
void print_to_screen(Node *state);

// Override: Determine priority (in the priority queue)
bool operator<(const Node &a, const Node &b)
{
	int priorityA = a.path_cost;
	int priorityB = b.path_cost; 
	return priorityA < priorityB;
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
		cout << ("Error");
    		exit (EXIT_FAILURE);
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

	// TODO How will we do these? dummy values for now
	int path_cost = currNode->path_cost;
	int depth = currNode->depth;
	int cost2go = currNode->cost2go;
	//path_cost++;
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
	
	if (childNode != NULL) {
		childNode->cost2go = Cost2Go(childNode);
	}

	return childNode;
}

int Cost2Go(Node *current){
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

	for (int i = 0; i < 9; i++) {
		int x = current->state[i];
		sum = sum + costs[i][x];
	}

	return sum;
}

bool GoalCheck(Node *current){
	int goal[] ={0,1,2,3,4,5,6,7,8};
	Node *goalState = construct_node(goal, NULL, "", 0, 0, 0);
	return CompareStates(current, goalState);
}

void print_to_screen(Node *state){
	cout << "Step " << state->depth <<": " << state->action << endl;				
	for(int i = 0; i<9; ){
		for(int j = 0; j<3; j++){		
			cout << state->state[i] << " ";
			i++;
	   	}
		cout << endl;
	}
}

void print_node_meta(Node *state, string action){
	cout << "-- For: " << action << endl;
	cout << "\tpath_cost: " << state->path_cost << endl;
	cout << "\tdepth: " << state->depth << endl;
	cout << "\tcost2go: " << state->cost2go << endl;
}

Node *Solution(Node *n){	
	
	if(n->parentNode != NULL){
		Solution(n->parentNode);
	}
	print_to_screen(n);
	return n;

}

Node *AStarSearch(Node *current, priority_queue<Node> *explored, priority_queue<Node> *frontier){
	frontier = new priority_queue<Node>;
	explored = new priority_queue<Node>;

	frontier->push(*current);

	/* Search until no nodes are visible from current state */
	Node *curr = current;

	while(!frontier->empty()){
		curr = new Node(frontier->top());

		/* If current state == goal state, return */
		if (GoalCheck(curr)){
			cout << "You have reached the goal." << endl;
			//exit(EXIT_FAILURE);
			return curr;	
		} 

		frontier->pop();
		
		/* For each direction child of current state */
		string actions[] = {"up","down","left","right"};
		Node *child;

		for (int i=0; i<4; i++){
			child = ChildNode(curr, actions[i]);
			if (child != NULL){
				child->path_cost = child->cost2go + child->parentNode->path_cost + 1;
				if ((InQueue(child, frontier) == NULL) || (InQueue(child, explored) == NULL)){
					frontier->push(*child);
				}
				else if ((InQueue(child, frontier)->path_cost) > (child->path_cost)){
					Swap(child, frontier);
				}
				print_to_screen(child);
			}
		}
		
		curr = new Node(frontier->top());		
		child = NULL;	
	}

	return NULL;
}

void Swap(Node *swapState, priority_queue<Node> *q){
	priority_queue<Node> *temp;
	temp = new priority_queue<Node>;	

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

bool CompareStates(Node *cmp1, Node *cmp2){
	int *cmpTiles1 = cmp1->state;
	int *cmpTiles2 = cmp2->state;
	bool tileMatch = true;

	for(int i=0; i<9; i++){
		if (cmpTiles1[i] != cmpTiles2[i]){
			tileMatch = false;
		}
	}

	return tileMatch;
}

/* TODO Test*/
Node *InQueue(Node *compState, priority_queue<Node> *q){
	bool isIn = false;	
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

	for (int j=0; j < qSize; j++){
		Node *curr = new Node(temp->top());
		temp->pop();
		q->push(*curr);
	}

	return match;
}

int main(){

	// Initialize beginning state
	int initState[] = {5,0,4,2,1,3,6,7,8};	
	Node *startNode = construct_node(initState, NULL, "None", 0, 0, 0);

	// Define explored and frontier priority queues
	priority_queue<Node> *explored;
	priority_queue<Node> *frontier;
	

 	Node *l = AStarSearch(l, explored, frontier);

	if(l != NULL){Solution(l);}

	delete l;
	delete startNode;
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

