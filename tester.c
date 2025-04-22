/* tests list_t.c functionality */

#include "list_t.h"

int main()
{
    list_t l = list_new(), l2;

    /* Test new and print */
    printf("\nInitial Test\n");
    printf("Make new and print list_t should print:\n");
    printf("[]\n");
    printf("Does print:\n");
    list_print(l);

    /* Test free */
    list_free(l);
    printf("Free ran without error.\n");

    /* Test append */
    printf("\n0. Append \n");
    printf("Append 2 to [1] and 4 to [3], should print:\n");
    printf("[1, 2]\n");
    printf("[3, 4]\n");
    printf("Does print:\n");
    l = list_new();
    list_append(l,(void *)1);
    list_append(l,(void *)2);
    l2 = list_new();
    list_append(l2,(void *)3);
    list_append(l2,(void *)4);
    list_print(l);
    list_print(l2);

    /* Test extend */
    printf("\n1. Extend \n");
    printf("Extend [1, 2] with [3, 4], should print:\n");
    printf("[1, 2, 3, 4]\n");
    printf("Does print:\n");
    list_extend(l,l2);
    list_print(l);

    /* Test insert */
    printf("\n2. Insert \n");
    printf("Insert 5 into list_t at position 2, should print:\n");
    printf("[1, 2, 5, 3, 4]\n");
    printf("Does print:\n");
    list_insert(l,2,(void *)5);
    list_print(l);

    /* Test remove */
    printf("\n3. Remove \n");
    printf("Remove 5 from list_t, should print:\n");
    printf("[1, 2, 3, 4]\n");
    printf("Does print:\n");
    list_remove(l,(void *)5);
    list_print(l);

    /* Test pop */
    printf("\n4. Pop \n");
    printf("Pop 1th element from list_t, should print:\n");
    printf("[1, 3, 4]\n");
    printf("Does print:\n");
    list_pop(l,1);
    list_print(l);

    /* Test pop */

    printf("\n5. Clear \n");
    printf("Clearing list, should print:\n");
    printf("[]\n");
    printf("Does print:\n");
    list_clear(l);
    list_print(l);

    /* Test index */
    printf("\n6. Index \n");
    printf("Index of 3 in [1,1,3,1,3] should be:\n");
    printf("2\n");
    printf("Is found to be:\n");
    list_append(l,(void *)1);
    list_append(l,(void *)1);
    list_append(l,(void *)3);
    list_append(l,(void *)1);
    list_append(l,(void *)3);
    printf("%lu\n",list_index(l, (void *)3));

    /* Test count */
    printf("\n7. Count \n");
    printf("Count of 1 in [1,1,3,1,3] should be:\n");
    printf("3\n");
    printf("Is found to be:\n");
    printf("%lu\n",list_count(l, (void *)1));

    /* Test reverse */
    printf("\n8. Reverse \n");
    printf("Reversing [1,1,3,1,3] should be:\n");
    printf("[3, 1, 3, 1, 1]\n");
    printf("Is found to be:\n");
    list_reverse(l);
    list_print(l);

    /* Test copy */
    printf("\n9. Copy \n");
    printf("Copying [3,1,3,1,1] then clearing, copy should be:\n");
    printf("[3, 1, 3, 1, 1]\n");
    printf("Is found to be:\n");
    l2 = list_copy(l);
    list_clear(l);
    list_print(l2);

    return 0;
}
