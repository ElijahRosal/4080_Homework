/*
 * Doubly-linked list of heap-allocated strings.
 * Functions: insert, find, delete. Includes tests in main().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	char *data; // heap-allocated
	struct Node *prev;
	struct Node *next;
} Node;

// Create a node with a heap-allocated copy of str
Node *create_node(const char *str) {
	if (!str) return NULL;
	Node *n = (Node *)malloc(sizeof(Node));
	if (!n) return NULL;
	size_t len = strlen(str) + 1;
	n->data = (char *)malloc(len);
	if (!n->data) {
		free(n);
		return NULL;
	}
	memcpy(n->data, str, len);
	n->prev = n->next = NULL;
	return n;
}

// Insert at head
void insert_front(Node **head, const char *str) {
	if (!head) return;
	Node *n = create_node(str);
	if (!n) return;
	n->next = *head;
	if (*head)
		(*head)->prev = n;
	*head = n;
}

// Insert at tail
void insert_back(Node **head, const char *str) {
	if (!head) return;
	Node *n = create_node(str);
	if (!n) return;
	if (!*head) {
		*head = n;
		return;
	}
	Node *cur = *head;
	while (cur->next) cur = cur->next;
	cur->next = n;
	n->prev = cur;
}

// Find first node whose data equals str. Returns pointer or NULL.
Node *find(Node *head, const char *str) {
	Node *cur = head;
	while (cur) {
		if (strcmp(cur->data, str) == 0) return cur;
		cur = cur->next;
	}
	return NULL;
}

// Delete a node given its pointer. Updates head if needed.
void delete_node(Node **head, Node *n) {
	if (!head || !*head || !n) return;
	if (n->prev)
		n->prev->next = n->next;
	else
		*head = n->next; // n was head
	if (n->next)
		n->next->prev = n->prev;
	free(n->data);
	free(n);
}

// Delete first occurrence of str. Returns 1 if deleted, 0 not found.
int delete_by_value(Node **head, const char *str) {
	Node *n = find(*head, str);
	if (!n) return 0;
	delete_node(head, n);
	return 1;
}

// Print list forward
void print_forward(Node *head) {
	printf("List forward:");
	for (Node *cur = head; cur; cur = cur->next) {
		printf(" [%s]", cur->data);
	}
	printf("\n");
}

// Print list backward
void print_backward(Node *head) {
	if (!head) {
		printf("List backward: (empty)\n");
		return;
	}
	Node *cur = head;
	while (cur->next) cur = cur->next;
	printf("List backward:");
	for (; cur; cur = cur->prev) {
		printf(" [%s]", cur->data);
	}
	printf("\n");
}

// Free the whole list
void free_list(Node **head) {
	if (!head) return;
	Node *cur = *head;
	while (cur) {
		Node *next = cur->next;
		free(cur->data);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

// Tests
int main(void) {
	Node *head = NULL;

	printf("Inserting: apple, banana, cherry at back\n");
	insert_back(&head, "apple");
	insert_back(&head, "banana");
	insert_back(&head, "cherry");
	print_forward(head);
	print_backward(head);

	printf("Inserting at front: date\n");
	insert_front(&head, "date");
	print_forward(head);

	printf("Finding 'banana' and 'fig'...\n");
	Node *f = find(head, "banana");
	printf("find banana: %s\n", f ? "found" : "not found");
	f = find(head, "fig");
	printf("find fig: %s\n", f ? "found" : "not found");

	printf("Deleting 'banana' (middle)\n");
	if (delete_by_value(&head, "banana")) print_forward(head); else printf("banana not found\n");

	printf("Deleting head ('date')\n");
	if (delete_by_value(&head, "date")) print_forward(head); else printf("date not found\n");

	printf("Deleting tail ('cherry')\n");
	if (delete_by_value(&head, "cherry")) print_forward(head); else printf("cherry not found\n");

	printf("Attempting to delete non-existent 'grape'\n");
	if (!delete_by_value(&head, "grape")) printf("grape not found (as expected)\n");

	printf("Final list and cleanup:\n");
	print_forward(head);
	free_list(&head);
	print_forward(head);

	return 0;
}
