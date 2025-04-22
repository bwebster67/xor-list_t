#include "list_t.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define XOR(a,b) ((node_t)((uintptr_t)(a) ^ (uintptr_t)(b)))

/* clear; gcc tester.c list_t.c -std=c89 -Wall -Wextra -Werror -Wpedantic -O2 -g; valgrind --leak-check=yes ./a.out */


typedef struct node {
	void *value;
	void *npx;
} *node_t;

struct list_t {
	/* linked-list style implementation */
	node_t head;
	node_t tail;
};


node_t node_new() {
	node_t ptr = (node_t)malloc(sizeof(struct node));
    if (ptr == 0) { 
       fprintf(stderr, "Malloc failed.\n"); 
       exit(-1); 
    }
	ptr->value = NULL;
	ptr->npx = NULL; /* XOR */
	return ptr;
}


/*
 _: New (a la Python __init__)
 inputs: none
 outputs: a new list_t containing no values
 side effects: none
 */
list_t list_new() {
	list_t ptr = (list_t)malloc(sizeof(struct list_t));
    if (ptr == 0) { 
       fprintf(stderr, "Malloc failed.\n"); 
       exit(-1); 
    }
	ptr->head = NULL;
	ptr->tail = NULL; /* XOR */
	return ptr;
 }

/*
 _: Free (no Python equivalent)
 inputs: a list_t l
 outputs: nothing
 side effects: frees all memory associated with l
 */
void list_free(list_t l) {
	node_t prev = NULL;
	node_t next = NULL;
	node_t curr = l->head;

	/* If list is empty */
	if (curr == NULL) {return;}
	
	/* Else If list has one node */
	else {
		if (l->head->npx == NULL) {
			free(curr);
			curr = NULL;
		}
	/* Else if list has more than one value */
		else {
			while (curr != NULL) {

				next = XOR(prev, curr->npx);
				if (prev != NULL) {
					free(prev);
					prev = NULL;
				}
				
				prev = curr;
				curr = next;
				
			}
			if (prev != NULL) {
				free(prev);
				prev = NULL;
			}
		}
	}
	free(l);
	l = NULL;
	return;
}

/*
 _: Print (a la Python __str__)
 _: Print (a la Python __str__)
 inputs: a list_t l
 outputs: nothing
 side effects: the elements of l are printed as in python
 example:
 list_print(list_new());
 - should print "[]"
 Note: Prints void *'s as uint64_t's. (use %lu)
 */

void list_print(list_t l) {
	void* prev = 0;
	void* next = 0;
	node_t curr;
	
	/* If list is NULL */
	if (l == NULL) {return;}

	/* If list is empty */
	if (l->head == NULL) {fprintf(stdout, "[]\n"); return;}
	
	/* Else if list has one value */
	if (l->head->npx == NULL) {fprintf(stdout, "[%lu]", (long unsigned)l->head->value); return;}
	
	/* Else if list has more than one value */
	curr = l->head;
	fprintf(stdout, "[");
	while (curr != NULL) {
		next = XOR(prev, curr->npx); 
		if (next == NULL) break;
		fprintf(stdout, "%lu, ", (unsigned long)curr->value);
		
		prev = curr;
		curr = next;
	}
	fprintf(stdout, "%lu]\n", (unsigned long)curr->value);
	return;	
}

/*
 0: Append
 inputs: a list_t l, a pointer to an memory object of any type x
 outputs: nothing
 side effects: x is added to the end of l
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 list_print(l);
 - should print "[1]"
 */
void list_append(list_t l, void *x) {
	node_t prev = 0;
	node_t new = node_new();
	new->value = x;

	/* list is empty */
	if (l->head == NULL) {
		l->head = new;
		l->tail = new;
		return;
	}
		
	/* list has at least one element */
	else {
		new->npx = XOR(l->tail, NULL);
		prev = XOR(l->tail->npx, NULL);
		l->tail->npx = XOR(prev, new);
		l->tail = new;
		return;
	}
}

/*
 1: Extend
 inputs: two list_ts, l1 and l2
 outputs: nothing
 side effects: all elements of l2 are appended to l1
 example:
 list_t l1 = list_new();
 uint64_t *val = 1;
 list_append(l1, val);
 list_t l2 = list_new();
 *val = 2;
 list_append(l2, val);
 list_print(l1);
 - should print "[1, 2]"
 */
void list_extend(list_t l1, list_t l2) {
	node_t curr = l2->head;
	node_t prev = 0;
	node_t next = 0;
	
	while (curr != NULL) {
		list_append(l1, curr->value);	
		next = XOR(prev, curr->npx); 
		prev = curr;
		curr = next;
	}
	return;
}

/*
 2: Insert
 inputs: a list_t l, size_t list index i, and a pointer to an memory object of any type x
 outputs: nothing
 side effects: x is added to l with index i and all elements are preserved in l
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 *val = 2;
 list_insert(l, 0, val);
 list_print(l);
 - should print "[2, 1]"
 */
void list_insert(list_t l, size_t i, void *x) {
	size_t curr_index = 0;
	node_t curr_node = l->head;
	node_t prev = 0;
	node_t next = 0;
	node_t new = node_new();
	new->value = x;

	/* If insert is at index 0 */
	if (i == 0) {
		/* list is empty */
		if (l->head == NULL) {
			l->head = new;
			l->tail = new;
		}
		else {
			l->head->npx = XOR(new, l->head->npx);
			new->npx = l->head;
			l->head = new;
		}
		return;
	}

	/* Else if insert is at another index */
	/* make sure to update tail if its being added at the end */
	while (curr_node != NULL) {
		
		curr_index++;
		next = XOR(prev, curr_node->npx);

		/* If current node is at the given index */
		if (i == curr_index) {

			/* If index is the last thing in the list */
			if (next == l->tail) {
				l->tail->npx = XOR(l->tail->npx, new);
				new->npx = l->tail;
				l->tail = new;
			}
			else {
				new->npx = XOR(curr_node, next);
				curr_node->npx = XOR(prev, new);
				next->npx = XOR(new, XOR(next->npx, curr_node)); 
			}
			return;
		}
		prev = curr_node;
		curr_node = next;
	}
	free(new);
	return;
}

/*
 3: Remove
 inputs: a list_t l, and a pointer to an memory object of any type x
 outputs: TRUE if an instance of x is removed, FALSE otherwise
 side effects: the first instance of x is removed from l
 example:
 list_t l = list_new();
 uint64_t *val = 2;
 list_append(l, val);
 list_append(l, val);
 *val = 1;
 list_print(l1);
 list_remove(l, 1, val);
 list_print(l);
 - should return True
 - should print "[2, 1, 2]" then "[1, 2]"
 */
bool list_remove(list_t l, void *x) {
	node_t curr = l->head;
	node_t prev = 0;
	node_t next = 0;
	
	/* If head */
	if (l->head->value == x) {
		/* If only one value */
		if (l->head->npx == NULL) {
			free(l->head);
			l->head = NULL;
			return TRUE;
		}
		next = l->head->npx; 
		next->npx = XOR(next->npx, l->head);
		free(l->head);
		l->head = next;

		return TRUE;
	}

	while (curr != NULL) {

		next = XOR(prev, curr->npx); 
		if (curr->value == x) {
			/* If node is tail */
			if (curr == l->tail) {	

				/* Tail remove logic*/
				prev->npx = XOR(prev->npx, curr);
				l->tail = prev;
				free(curr);
				return TRUE;
			}
			prev->npx = XOR(next, XOR(prev->npx, curr));
			next->npx = XOR(prev, XOR(next->npx, curr));
			free(curr);
			return TRUE;
		}
		prev = curr;
		curr = next;
	}
	return FALSE;
}

/*
 4: Pop
 inputs: a list_t l, and a size_t list index i
 outputs: The element at index i, or exit(1) if i is out of range
 side effects: remove the element at index i
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 *val = 2;
 list_insert(l, 0, val);
 list_print(l);
 printf("%d\n", pop(l, 1));
 list_print(l);
 - should print "[1, 2]" then "2" then "[1]"
 */
void *list_pop(list_t l, size_t i) {
	size_t curr_index = 0;
	node_t curr_node = l->head;
	node_t prev = 0;
	node_t next = 0;

	/* If pop is at index 0 */
	if (i == 0) {
		/* list is empty */
		if (l->head == NULL) {
			exit(1);
		}
		/* list is not empty */
		else {
			next = l->head->npx; 
			next->npx = XOR(next->npx, l->head);
			l->head = next;
		}
		return curr_node;
	}

	/* Else if pop is at another index */
	while (curr_node != NULL) {
		
		next = XOR(prev, curr_node->npx);

		/* If current node is at the given index */
		if (i == curr_index) {

			/* If index is the last thing in the list */
			if (curr_node == l->tail) {
				/* POP TAIL LOGIC */	
				prev->npx = XOR(prev->npx, curr_node);
				l->tail = prev;
				return curr_node;
			}

			else {
				/* GENERAL POP LOGIC */
				prev->npx = XOR(next, XOR(prev->npx, curr_node));
				next->npx = XOR(prev, XOR(next->npx, curr_node));
				return next;
			}
		}
		prev = curr_node;
		curr_node = next;
		curr_index++;
	}
	exit(1);
}

/*
 5: Clear
 inputs: a list_t l
 outputs: nothing
 side effects: l contains no elements
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 clear(l)
 - should print "[]"
 */
void list_clear(list_t l) {
	node_t prev = NULL;
	node_t next = NULL;
	node_t curr = l->head;

	/* If list is empty */
	if (curr == NULL) {return;}
	
	/* Else If list has one node */
	else {
		if (l->head->npx == NULL) {
			free(curr);
			curr = NULL;
		}
	/* Else if list has more than one value */
		else {
			while (curr != NULL) {
				next = XOR(prev, curr->npx);
				if (prev != NULL) {
					free(prev);
					prev = NULL;
				}
				
				prev = curr;
				curr = next;
				
			}
			if (prev != NULL) {
				free(prev);
				prev = NULL;
			}
		}
	}
	l->head = NULL;
	l->tail = NULL;
}

/*
 6: Index
 inputs: a list_t l, and a pointer to an memory object of any type x
 outputs: a size_t i giving the index of x in l, or exit(1) if x is not in l.
 side effects: none
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 printf("%d\n", index(l, val)));
 - should print "0"
 */
size_t list_index(list_t l, void *x) {
	node_t curr = l->head;
	node_t prev = 0;
	node_t next = 0;
	size_t i = 0;

	while (curr != NULL) {

		next = XOR(prev, curr->npx); 
		/*fprintf(stdout, "	value: %lu,   ptr: %p,   npx: %p\n", (unsigned long)curr->value, (void*)curr, (void*)curr->npx);*/
		if (curr->value == x) {

			return i;
		}

		prev = curr;
		curr = next;
		i++;
	}
	exit(1);
}

/*
 7: Count
 inputs: a list_t l, and a pointer to an memory object of any type x
 outputs: The number of times x occurs in l
 side effects: none
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 list_append(l, val);
 list_count(l, val);
 - should return 2
 */
uint64_t list_count(list_t l, void *x) {
	node_t curr = l->head;
	node_t prev = 0;
	node_t next = 0;
	size_t count = 0;
	
	while (curr != NULL) {

		if (curr->value == x) {count++;}

		next = XOR(prev, curr->npx); 
		prev = curr;
		curr = next;
	}
	return count;
}

/*
 8: Reverse
 inputs: a list_t l
 outputs: nothing
 side effects: the elements of l are reversed
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 *val = 2;
 list_append(l, val);
 list_print(l);
 list_reverse(l)
 list_print(l);
 - should print "[1, 2]" then "[2, 1]"
 */
void list_reverse(list_t l) {
	node_t tmp = l->head;
	l->head = l->tail;
	l->tail = tmp;
}

/*
 9: Copy
 inputs: a list_t l
 outputs: a list_t r containing all the elements of l
 side effects: none
 example:
 list_t l = list_new();
 uint64_t *val = 1;
 list_append(l, val);
 list_t r = copy(l);
 list_append(l, val);
 list_print(l);
 list_print(r);
 - should print "[1, 1]" then "[1]"
 */
list_t list_copy(list_t l) {
	list_t copy = list_new();
	node_t curr = l->head;
	node_t prev = 0;
	node_t next = 0;
	
	while (curr != NULL) {
		list_append(copy, (void *)curr->value);
		next = XOR(prev, curr->npx); 

		prev = curr;
		curr = next;
	}

	return copy;
}
