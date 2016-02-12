#include <string>
#include <iostream>
#include <queue>


using namespace std;


struct Node{					
	int state;
	Node* parentNode;
	string actions[4];
	int path_cost;
	int depth;
	int cost2go;

};

//TODO::
//Node ChildNode(Node n, action[goalstate]){
//	return NULL;	
//}

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
			return false;
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
}

int main(){

	int state[] = {5,0,4,2,1,3,6,7,8};
	Node stateNode[9];
	priority_queue<int> explored;
	priority_queue<int> frontier;
	string act[] = {"up", "down", "left", "right"};
 	
 	/**
 	* Instantiating new nodes in Array
	*/
 	for(int i = 0; i < 9; i++){
 		stateNode[i].state = state[i];
 		stateNode[i].parentNode = NULL;
 		stateNode[i].path_cost = 1;
 	}

 	print_to_screen(stateNode);

 	return 0;
 }
