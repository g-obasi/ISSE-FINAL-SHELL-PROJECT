/*
 * clist.c
 *
 * Linked list implementation
 *
 * Author: Greenfield Obasi
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "clist.h"

#define DEBUG


struct _cl_node {
    CListElementType element;
    struct _cl_node *next;
};

struct _clist {
    struct _cl_node *head;
    int length;
};



/*
 * Create (malloc) a new _cl_node and populate it with the supplied
 * values
 *
 * Parameters:
 *   element, next  the values for the node to be created
 *
 * Returns: The newly-malloc'd node, or NULL in case of error
 */
static struct _cl_node*
_CL_new_node(CListElementType element, struct _cl_node *next)
{
    struct _cl_node* new = (struct _cl_node*) malloc(sizeof(struct _cl_node));

    assert(new);

    new->element = element;
    new->next = next;

    return new;
}


// Documented in .h file
CList CL_new()
{
    CList list = (CList) malloc(sizeof(struct _clist));
    assert(list);

    list->head = NULL;
    list->length = 0;

    return list;
}


void CL_free(CList list)
{
//    assert(list);

    if (list == NULL) {
        // Handle the case of an empty list
        return;
    }

    // Traverse the list and free each node
    struct _cl_node *current = list->head;
    while (current != NULL) {
        struct _cl_node *next = current->next;  // Store the next node
        free(current);  // Free the current node
        current = next; // Move to the next node
    }

    // Reset list properties
    list->head = NULL;
    list->length = 0;

    // Finally, free the list structure itself
    free(list);
}


// Documented in .h file
int CL_length(CList list)
{

        if (list == NULL || list->head == NULL) {
            // Handle the case of an empty list
            return 0;
        }
#ifdef DEBUG
    // In production code, we simply return the stored value for
    // length. However, as a defensive programming method to prevent
    // bugs in our code, in DEBUG mode we walk the list and ensure the
    // number of elements on the list is equal to the stored length.

    int len = 0;
    for (struct _cl_node *node = list->head; node != NULL; node = node->next)
        len++;

    assert(len == list->length);
#endif // DEBUG

    return list->length;
}


 //Documented in .h file
void CL_push(CList list, CListElementType element)
{
    assert(list);
    list->head = _CL_new_node(element, list->head);
    list->length++;
}


// Documented in .h file
CListElementType CL_pop(CList list)
{
    assert(list);
    struct _cl_node *popped_node = list->head;

    if (popped_node == NULL)
        return INVALID_RETURN;

    CListElementType ret = popped_node->element;

    // unlink previous head node, then free it
    list->head = popped_node->next;
    free(popped_node);
    // we cannot refer to popped node any longer

    list->length--;
    return ret;
}


// Documented in .h file
void CL_append(CList list, CListElementType element) {
    struct _cl_node *new_node = malloc(sizeof(struct _cl_node));
    if (new_node == NULL) {
        // Handle memory allocation error
        return;
    }

    new_node->element = element;
    new_node->next = NULL; // Initialize the next pointer to NULL for a singly linked list

    if (list->head == NULL) {
        // If the list is empty, make the new node the head
        list->head = new_node;
    } else {
        // If the list is not empty, add the new node to the end
        struct _cl_node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
//    free(new_node);

    list->length++;
}


CListElementType CL_nth(CList list, int pos) {
    // Ensure the input list is not NULL.
    if (list == NULL)
        return INVALID_RETURN;

    // Check if the position is out of range.
    if (pos < -list->length || pos >= list->length){
        fprintf(stderr, "Error Enter a valid range (%d) is not valid\n", pos);
//        exit (0);
        return INVALID_RETURN;
    }

    // Convert negative position to positive if needed.
    if (pos < 0)
        pos += list->length;

    // Iterate through the list to the specified position.
    struct _cl_node *nth_node = list->head;
    for (int count = 0; count < pos; count++) {
        nth_node = nth_node->next;
    }

    // Return the element found at the specified position in the list.
    return nth_node->element;
}


// Documented in .h file
CListElementType CL_remove(CList list, int pos) {
    assert(list);
    if ( list->length == 0 || pos < -list->length || pos >= list->length) {
        return INVALID_RETURN; // Return invalid value if list is empty or pos is out of range
    }

    if (pos < 0) {
        pos += list->length; // Convert negative index to equivalent positive index
    }

  struct _cl_node *current = list->head;
    struct _cl_node  *prev = NULL;

    // Traverse the list to find the node at the specified position
    for (int i = 0; i < pos; i++) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        // Removing the head node
        list->head = current->next;
    } else {
        prev->next = current->next;
    }

    CListElementType removed_element = current->element;
    free(current); // Free the memory of the removed node
    list->length--;

    return removed_element;
}


bool CL_insert(CList list, CListElementType element, int pos)
{
    assert(list);
    if ( pos < -list->length - 1 || pos > list->length)
    { // Invalid list or out-of-range position
         return 0; }

        // Create a new node for the element
        struct _cl_node *new_node = (struct _cl_node *)malloc(sizeof(struct _cl_node));
        if (!new_node) {
            // Memory allocation failed
            return 0;
        }
        new_node->element = element;

        // Handle the case where pos is negative (counting from the end)
        if (pos < 0) {
            pos = list->length + pos + 1;
        }

        // Handle the case where pos is 0 (insert at the beginning)
        if (pos == 0) {
            new_node->next = list->head;
            list->head = new_node;
        } else {
            struct _cl_node *prev_node = list->head;
            int i = 0;
            // Iterate through the list until the specified position is reached
            while (i < pos - 1 && prev_node != NULL) {
                prev_node = prev_node->next;
                i++;
            }

            // Check if prev_node is valid
            if (prev_node == NULL) {
                // Invalid position or empty list
                return 0;
            }

            // Store the next node of the new node
            struct _cl_node *next_node = prev_node->next;

            // Insert the new node between prev_node and next_node
            prev_node->next = new_node;
            new_node->next = next_node;
        }

        // Do not free the new node, since it is still part of the list
        // free(new_node);
        list->length++;
        return true;
    }


// Documented in .h file
CList CL_copy(CList list)
{
    assert(list);
//    struct _cl_node *node = list->head;    // used to iterate over the original list
//    CList newList = (CList) malloc(sizeof(struct _clist));    // head of the new list
//    struct _cl_node *tail = NULL;       // point to the last node in a new list


    // Create a new empty list
    CList new_list = CL_new();


    // Iterate through the original list and copy each element to the new list
    struct _cl_node *current = list->head;
    for (int i = 0; i < CL_length(list); i++) {
        if (!CL_insert(new_list, current->element, i)) {
            // Handle memory allocation failure for copying elements
            CL_free(new_list); // Free the memory for the new list
            return NULL;
        }
        current = current->next;
    }

    return new_list;
}


void CL_join(CList list1, CList list2) {
    if (list1 == NULL || list2 == NULL) {
        // Handle the case of invalid lists
        return;
    }

    while (list2->length > 0) {
        CL_append(list1, CL_pop(list2));
    }

}


void CL_reverse(CList list) {
    assert(list);

    if ( list->head == NULL || list->head->next == NULL) {
        // Empty list or list with only one element, nothing to reverse
        return;
    }

    struct _cl_node *prev = NULL;
    struct _cl_node *current = list->head;
    struct _cl_node *next = NULL;

    while (current != NULL) {
        // Save the next node
        next = current->next;

        // Reverse the next pointer of the current node
        current->next = prev;

        // Move to the next node
        prev = current;
        current = next;
    }

    // Update the head of the list to point to the new first element (previously the last element)
    list->head = prev;
}


// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data) {
    assert(list);

    // Function to iterate through the list and call the callback function for each element
    if ( callback == NULL) {
        // Handle invalid input
        return;
    }

    int position = 0;
    struct _cl_node *current = list->head;

    while (current != NULL) {
        // Call the callback function with position, element, and cb_data
        callback(position, current->element, cb_data);

        // Move to the next node
        current = current->next;
        position++;
    }
}


bool CL_is_empty(CList list) {
    return list == NULL || list->length == 0;
}


Token CL_next(CList tokens) {
    Token emptyToken;
    emptyToken.type = TOK_END;

    if (CL_is_empty(tokens)) {
        return emptyToken;
    } else {
            return CL_nth(tokens, 0);
    }
}