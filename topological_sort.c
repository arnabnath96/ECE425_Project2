#include "topological_sort.h"
#include "graph.h"
#include "stack.h"

//Control function that calls the recursive depth-first traversal function responsible for
//topological sort algorithm with nodes that have no predecessors.
Stack* topological_sort(NODE* graph, int max)
{
    Stack* sorted = create_stack();
	char visited[Mnod];	memset(visited,'n',Mnod);

	int id = 0;
	for(id = max; id >= 0; id--)
	{
		if(graph[id].Type == INPT)
		{
			depthfirst_traversal(graph, visited, sorted, id);
		}
	}

	if(isempty_stack(sorted))
        goto empty_stack;

	return sorted;

empty_stack:
    printf("ERROR: In topological_sort(...): sorted is empty!\n");
    exit(1);

}

//Determines the topological sort of a graph based on the post-order time algorithm presented in
//the book; Uses a stack to store sorted nodes instead of a list of times.
void depthfirst_traversal(NODE* graph, char* visited, Stack* sorted, int current_id)
{
    int number_of_fanout = graph[current_id].Nfo;
	LIST* current_fanout = graph[current_id].Fot;
	int current_fanout_id = -1;

	int i = 0;
	for(i = 0; i <= (number_of_fanout-1); i++)
	{
	    current_fanout_id = current_fanout->id;

        if(visited[current_fanout_id] == 'n')
        {
            depthfirst_traversal(graph, visited, sorted, current_fanout_id);
            visited[current_fanout_id] = 'y';
        }

		current_fanout = current_fanout->next;
	}

	push_stack(sorted, current_id);
	return;
}
