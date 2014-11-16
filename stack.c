#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*************************************************************************************************
 Stack Node
*************************************************************************************************/

StackNode* create_stacknode(int new_data)
{
	StackNode* new_node = malloc(sizeof(StackNode));

	new_node->data = new_data;
	new_node->below = NULL;

	return new_node;
}

void delete_stacknode(StackNode* deleted_node)
{
	if(deleted_node->below != NULL)
	{
		delete_stacknode(deleted_node->below);
	}

	free(deleted_node);
	return;
}


/*************************************************************************************************
 Stack
*************************************************************************************************/

Stack* create_stack()
{
    Stack* new_stack = malloc(sizeof(Stack));
    new_stack->top = NULL;
    return new_stack;
}

Stack* copy_stack(Stack* original_stack)
{
    Stack* new_stack = create_stack();
	
	if(original_stack->top != NULL)
	{
	    copy_stacknode(original_stack->top, new_stack);
	}
	
	return new_stack;
}

void copy_stacknode(StackNode* current_node, Stack* new_stack)
{
    if(current_node->below != NULL)
    {
        copy_stacknode(current_node->below, new_stack);
    }
    push_stack(new_stack,current_node->data);
    return;
}

void delete_stack(Stack* stack)
{
	if(stack->top != NULL)
	{
		delete_stacknode(stack->top);
	}
	free(stack);
	return;
}

void push_stack(Stack* stack, int pushed_data)
{
	StackNode* new_node = create_stacknode(pushed_data);

	if(stack->top == NULL)
	{
		stack->top = new_node;
	}
	else
	{
		new_node->below = stack->top;
		stack->top = new_node;
	}

	return;
}

int pop_stack(Stack* stack)
{
	if(stack->top != NULL)
	{
		int popped_data = stack->top->data;
		StackNode* next_node = stack->top->below;

		free(stack->top);
		stack->top = next_node;

		return popped_data;
	}
	else
	{
		printf("STACK: No elements to pop!\n");
		exit(1);
	}
}

void print_stack(Stack* stack)
{
	if(stack->top != NULL)
	{
		StackNode* current_node = stack->top;

		while(current_node != NULL)
		{
			printf("%d ", current_node->data);
			current_node = current_node->below;
		}
		putchar('\n');
	}
	else
	{
		printf("STACK IS EMPTY!\n");
	}
	return;
}

int isempty_stack(Stack* stack)
{
    if(stack->top == NULL)
        return 1;
    else
        return 0;
}
