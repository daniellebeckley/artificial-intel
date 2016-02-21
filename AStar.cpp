#include <NodeFuncsLib.h>

// Override: Determine priority (in the priority queue)
bool operator<(const Node &a, const Node &b)
{
	int priorityA = a.path_cost;
	int priorityB = b.path_cost; 
	return priorityA > priorityB;
}

int main(){

	// Initialize beginning state
	int initState[] = {5,0,4,2,1,3,6,7,8};	
	Node *startNode = construct_node(initState, NULL, "None", 0, 0, 0);

	// Define explored and frontier priority queues
	priority_queue<Node> *explored;
	priority_queue<Node> *frontier;
	

 	Node *l = AStarSearch(startNode, explored, frontier);

	if(l != NULL){Solution(l);}

	delete l;
	delete startNode;

 	return 0;
}


