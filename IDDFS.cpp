#include <NodeFuncsLib.h>

// Override: Determine priority (in the priority queue)
bool operator<(const Node &a, const Node &b)
{
	int priorityA = a.depth;
	int priorityB = b.depth; 
	return priorityA < priorityB;
}

int main(){

	// Initialize beginning state
	int initState[] = {5,0,4,2,1,3,6,7,8};	
	Node *startNode = construct_node(initState, NULL, "None", 0, 0, 0);

	// Define explored and frontier priority queues
	priority_queue<Node> *explored;
	priority_queue<Node> *frontier;
	

 	Node *l = NULL;
  	int count = 0;
  	while (count <= 20 && l == NULL){
    		l = IDDFS(startNode, explored, frontier, count);
    		count++;
  	}

	if(l != NULL){Solution(l);}

	delete l;
	delete startNode;
 	
	return 0;
}

