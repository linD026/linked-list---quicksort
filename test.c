#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct __node {
  int value;
  struct __node *next;
} node_t;

static inline void list_add_node_t(node_t **list, node_t *node_t) {
  node_t->next = *list;
  *list = node_t;
}

static inline void list_concat(node_t **left, node_t *right) {
  while (*left)
    left = &((*left)->next);
  *left = right;
}

void quicksort(node_t **list) {
  if (!*list)
    return;

  node_t *pivot = *list;
  int value = pivot->value;
  node_t *p = pivot->next;
  pivot->next = NULL;

  node_t *left = NULL, *right = NULL;
  while (p) {
    node_t *n = p;
    p = p->next;
    list_add_node_t(n->value > value ? &right : &left, n);
  }

  quicksort(&left);
  quicksort(&right);

  node_t *result = NULL;
  list_concat(&result, left);
  list_concat(&result, pivot);
  list_concat(&result, right);
  *list = result;
}

static bool list_is_ordered(node_t *list) {
  bool first = true;
  int value;
  while (list) {
    if (first) {
      value = list->value;
      first = false;
    } else {
      if (list->value < value)
        return false;
      value = list->value;
    }
    list = list->next;
  }
  return true;
}

static void list_display(node_t *list) {
  printf("%s IN ORDER : ", list_is_ordered(list) ? "   " : "NOT");
  while (list) {
    printf("%d ", list->value);
    list = list->next;
  }
  printf("\n");
}

static node_t *list_make_node_t(node_t *list, int value) {
  node_t **indirect = &list;
  while (*indirect)
    indirect = &(*indirect)->next;
  *indirect = (node_t *)malloc(sizeof(node_t));
  (*indirect)->value = value;
  (*indirect)->next = NULL;
  return list;
}

static void list_free(node_t **list) {
  node_t *temp = NULL;
  while (*list) {
    temp = (*list);
    *list = (*list)->next;
    free(temp);
  }
}
// Lagged Fibonacci generators
// Sn ≡ Sn−j ⋆ Sn−k (modm), 0 < j < k
int self_random(int seed_f, int seed_s) {
  int sn_1 = 0;
  int sn_2 = 1;
  int sn;
  int i = seed_f;
  while (i > 0) {
    sn = (sn_2 & sn_1) % seed_s;
    sn_1 = sn_1 + 3;
    sn_2 = sn_2 + 7;
    i--;
  }
  return sn;
}

#define print_left                                                             \
  printf("left:");                                                             \
  list_display(left);

#define print_result                                                           \
  printf("top: %d\nresult:", top);                                             \
  list_display(result);

#define print_stack                                                            \
  printf("stack\n");                                                           \
  for (int i = top; i >= 0; i--) {                                             \
    printf("%d:", i);                                                          \
    list_display(stack[i]);                                                    \
  }

void quicksort_iterative(node_t **list) {
  if (!*list)
    return;

#define MAX 300
  node_t *stack[MAX] = {0};
  int top = -1;
  stack[++top] = *list;
  node_t *partition = NULL;
  node_t *result = NULL;

  while (top >= 0) {
    partition = stack[top--];
    if (partition && partition->next != NULL) {
      node_t *pivot = partition;
      int value = pivot->value;
      node_t *p = pivot->next;
      pivot->next = NULL;
      node_t *left = NULL, *right = NULL;
      while (p) {
        node_t *n = p;
        p = p->next;
        list_add_node_t(n->value >= value ? &right : &left, n);
      }
      if (left != NULL)
        list_concat(&left, pivot);
      else
        left = pivot;
      if (right)
        stack[++top] = right;
      if (left)
        stack[++top] = left;
    } else {
      top++;
      while (top >= 0 && stack[top]->next == NULL) {
        node_t *temp = stack[top--];
        list_concat(&result, temp);
      }
    }
  }
  *list = result;
}

int main(int argc, char **argv) {
  size_t count = 20;
  node_t *list = NULL;

  unsigned int seed = (uintptr_t)*argv;

  time_t current_time;
  srandom(seed & time(&current_time));
  struct tm *timeinfo;
  timeinfo = localtime(&current_time);
  printf("now: %s", asctime(timeinfo));

  while (count--) {
    list =
        list_make_node_t(list, self_random(random() % 1024, random() % 1024));
  }

  list_display(list);
  quicksort_iterative(&list);
  list_display(list);

  // forget list_free
  if (!list_is_ordered(list))
    return EXIT_FAILURE;

  list_free(&list);
  return EXIT_SUCCESS;
}