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
int Cost2Go(Node *current);
Node *InQueue(Node *compState, priority_queue<Node> *q);
bool CompareStates(Node *cmp1, Node *cmp2);
void Swap(Node *swapState, priority_queue<Node> *q);
void print_to_screen(Node *state);

// Override: Determine priority (in the priority queue)
bool operator<(const Node &a, const Node &b)
{
	/* TODO fix priority */
	int priorityA = a.path_cost + a.cost2go;
	int priorityB = b.path_cost + b.cost2go; 

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

	// TODO How will we do these? dummy values for now
	int path_cost = currNode->path_cost;
	int depth = currNode->depth;
	int cost2go = currNode->cost2go;
	path_cost++;
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
	cout << "Step: " << state->depth << endl;				
	for(int i = 0; i<9; ){
		for(int j = 0; j<3; j++){		
			cout << state->state[i] << " ";
			i++;
	   	}
		cout << endl;
	}
}

Node *Solution(Node *n){	
	
	if(n->parentNode != NULL){
		Solution(n->parentNode);
	}
	print_to_screen(n);
	return n;

}


Node *AStarSearch(Node *current, priority_queue<Node> *explored, priority_queue<Node> *frontier){
	/* Start with initial state */
	frontier = new priority_queue<Node>;
	explored = new priority_queue<Node>;
	frontier->push(*current);

	/* Search until no nodes are visible from current state */
	while (!frontier->empty()){
		Node *curr = new Node(frontier->top());	

		/* If current state == goal state, return */
		if (GoalCheck(curr)){
			return Solution(curr);	
		} 

		frontier->pop();
		explored->push(*curr);
		
		/* For each direction child of current state */
		Node *up = ChildNode(curr, "up");
		Node *down = ChildNode(curr, "down");
		Node *left = ChildNode(curr, "left");
		Node *right = ChildNode(curr, "right");
		Node *oldChild;		

		/* If child isn't NULL */
		if (up != NULL){			
			/* If child isn't in explored queue, update child path_cost */
			if (InQueue(up, explored) == NULL){
				up->path_cost = (up->cost2go) + (up->path_cost);
			}
			/* If child is in explored queue and cheaper, replace */
			else if ((InQueue(up, explored)->path_cost) > (up->path_cost)){		
				up->path_cost = (up->cost2go) + (up->path_cost);
				Swap(up, explored);			
			}
		}	
		if (down != NULL){
			if (InQueue(down, explored) == NULL){
				down->path_cost = (down->cost2go) + (down->path_cost);
			}
			else if ((InQueue(down, explored)->path_cost) > (down->path_cost)){
				down->path_cost = (down->cost2go) + (down->path_cost);
				Swap(down, explored);			
			}
		}
		if (left != NULL){
			if (InQueue(left, explored) == NULL){
				left->path_cost = (left->cost2go) + (left->path_cost);
			}
			else if ((InQueue(left, explored)->path_cost) > (left->path_cost)){
				left->path_cost = (left->cost2go) + (left->path_cost);
				Swap(left, explored);			
			}
		}
		if (right != NULL){
			if (InQueue(right, explored) == NULL){
				right->path_cost = (right->cost2go) + (right->path_cost);
			}
			else if ((InQueue(right, explored)->path_cost) > (right->path_cost)){
				right->path_cost = (right->cost2go) + (right->path_cost);
				Swap(right, explored);			
			}
		}

		/* If there is child, find smallest path_cost */
		int bestPath = INT_MAX;
		Node *bestState = NULL;	
		if (up != NULL){
			if (up->path_cost < bestPath){
				bestPath = up->path_cost;
				bestState = up;
			}
		}
		if (down != NULL){
			if (down->path_cost < bestPath){
				bestPath = down->path_cost;
				bestState = down;
			}
		}
		if (left != NULL){
			if (left->path_cost < bestPath){
				bestPath = left->path_cost;
				bestState = left;
			}
		}
		if (right != NULL){
			if (right->path_cost < bestPath){
				bestPath = right->path_cost;
				bestState = right;
			}
		}
		
		/* If there is a child, add smallest path_cost to frontier */
		if (bestState != NULL){
			/* TODO remove test */
			print_to_screen(bestState);

			frontier->push(*bestState);
		}

		for (int x=0; x < frontier->size(); x++){
			Node *test = new Node(frontier->top());			

			// TODO remove test			
			//cout << "--PRINT Q--" << endl;
			//print_to_screen(test);
			//delete test;
		}

		//delete up;
		//delete down;
		//delete left;
		//delete right;
		//delete bestState;
		//delete oldChild;
		//delete curr;
	}
	//delete curr;	

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
	
	/* For each state in priority queue */
	for (int i = 0; i < q->size() && !isIn; i++){	
		/* Check if state is same as 'compState' */
		Node *curr = new Node(q->top());
		bool tileMatch = CompareStates(compState, curr);
		
		/* State is the same */
		if (tileMatch){
			isIn = true;
			match = curr;
		}
	}

	return match;
}

int main(){

	// Initialize beginning state
	int initState[] = {5,0,4,2,1,3,6,7,8};	
	Node *startNode = construct_node(initState, NULL, "", 0, 0, 0);
	
	// TODO remove test
	//print_to_screen(startNode);

	// Define explored and frontier priority queues
	priority_queue<Node> *explored;
	priority_queue<Node> *frontier;

	// Generate child nodes
	Node *child = ChildNode(startNode, "down");
	
	//
	Node *test = AStarSearch(startNode, explored, frontier);

	//cout<<"Start: "<<endl;
	//print_to_screen(startNode);
	//cout<<"Child: "<<endl;
	//print_to_screen(child);

	cout <<"Solution:" << endl;
	/**while(!GoalCheck(child)){
		//do algorithm
	}
	*/
	Solution(child);		//this will print the solution

	// Deallocation of memory
	//TODO:; some sort of while loop to delete;
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

