/*
 * Copyright 2022-2022 D'Arcy Smith.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "dc_collections/linked_list.h"
#include <dc_c/dc_stdlib.h>


struct node
{
    void *data;
    struct node *next;
    struct node *prev;
};

struct dc_linked_list
{
    size_t number_of_elements;
    dc_comparator comparator;
    struct node *head;
    struct node *tail;
};

static void check_list(const struct dc_env *env, struct dc_error *err, const struct dc_linked_list *list, size_t number_of_elements);
static struct node *get_node_at(const struct dc_env *env, const struct node *start, size_t index);
static struct node *get_node_with(const struct dc_env *env, const struct node *start, void *item, dc_comparator comparator);

// NOLINTBEGIN(readability-function-cognitive-complexity)
static void check_list(const struct dc_env *env, struct dc_error *err, const struct dc_linked_list *list, size_t number_of_elements)
{
    DC_TRACE(env);

    if(list->comparator == NULL)
    {
        DC_ERROR_RAISE_SYSTEM(err, "", 1);
        return;
    }

    if(list->number_of_elements != number_of_elements)
    {
        DC_ERROR_RAISE_SYSTEM(err, "", 2);
        return;
    }

    if(list->number_of_elements == 0)
    {
        if(list->head != NULL)
        {
            DC_ERROR_RAISE_SYSTEM(err, "", 3);
            return;
        }

        if(list->tail != NULL)
        {
            DC_ERROR_RAISE_SYSTEM(err, "", 4);
            return;
        }
    }
    else
    {
        if(list->head->prev != NULL)
        {
            DC_ERROR_RAISE_SYSTEM(err, "", 5);
            return;
        }

        if(list->tail->next != NULL)
        {
            DC_ERROR_RAISE_SYSTEM(err, "", 6);
            return;
        }

        if(list->number_of_elements == 1)
        {
            if(list->head == NULL)
            {
                DC_ERROR_RAISE_SYSTEM(err, "", 7);
                return;
            }

            if(list->head != list->tail)
            {
                DC_ERROR_RAISE_SYSTEM(err, "", 8);
                return;
            }
        }
        else
        {
            if(list->head == NULL)
            {
                DC_ERROR_RAISE_SYSTEM(err, "", 9);
                return;
            }

            if(list->tail == NULL)
            {
                DC_ERROR_RAISE_SYSTEM(err, "", 10);
                return;
            }

            if(list->head != list->tail)
            {
                DC_ERROR_RAISE_SYSTEM(err, "", 11);
                return;
            }

            if(list->head->next == NULL)
            {
                DC_ERROR_RAISE_SYSTEM(err, "", 12);
                return;
            }

            if(list->tail->prev == NULL)
            {
                DC_ERROR_RAISE_SYSTEM(err, "", 13);
                return;
            }
        }
    }
}
// NOLINTEND(readability-function-cognitive-complexity)

static struct node *get_node_at(const struct dc_env *env, const struct node *start, size_t index)
{
    // TODO: could optimize this, if closer to the end, start at tail and go backwards
    struct node *tmp;

    DC_TRACE(env);
    tmp = start;

    for(size_t current_index = 0; current_index < index; current_index++)
    {
        tmp = tmp->next;
    }

    return tmp;
}

static struct node *get_node_with(const struct dc_env *env, const struct node *start, void *item, dc_comparator comparator)
{
    struct node *tmp;

    DC_TRACE(env);
    tmp = start;

    while(tmp)
    {
        int comparison;

        comparison = comparator(env, item, tmp->data);

        if(comparison == 0)
        {
            break;
        }

        tmp = tmp->next;
    }

    return tmp;
}

struct dc_linked_list *dc_linked_list_create(const struct dc_env *env, struct dc_error *err, dc_comparator comparator)
{
    struct dc_linked_list *list;

    DC_TRACE(env);
    list = dc_calloc(env, err, 1, sizeof(struct dc_linked_list));

    if(dc_error_has_no_error(err))
    {
        list->comparator = comparator;
        check_list(env, err, list, 0);
    }

    return list;
}

void dc_linked_list_destroy(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list)
{
    DC_TRACE(env);
    dc_linked_list_clear(env, err, list);
    dc_free(env, list);
}

bool dc_linked_list_is_empty(const struct dc_env *env, const struct dc_linked_list *list)
{
    DC_TRACE(env);

    return list->number_of_elements == 0;
}

size_t dc_linked_list_size(const struct dc_env *env, const struct dc_linked_list *list)
{
    DC_TRACE(env);

    return list->number_of_elements;
}

void dc_linked_list_clear(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list)
{
    DC_TRACE(env);

    for(struct node *tmp = list->head; tmp;)
    {
        struct node *next;

        next = tmp->next;
        dc_free(env, tmp);
        tmp = next;
        list->number_of_elements--;
    }

    list->head = NULL;
    list->tail = NULL;
    check_list(env, err, list, 0);
}

bool dc_linked_list_add_first(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item)
{
    bool ret_val;

    DC_TRACE(env);
    ret_val = dc_linked_list_add_at(env, err, list, 0, item);

    return ret_val;
}

ssize_t dc_linked_list_add_last(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item)
{
    ssize_t ret_val;

    DC_TRACE(env);
    dc_linked_list_add_at(env, err, list, list->number_of_elements, item);

    if(dc_error_has_error(err))
    {
        ret_val = -1;
    }
    else
    {
        ret_val = (ssize_t)list->number_of_elements;
    }

    return ret_val;
}

bool dc_linked_list_add_at(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, size_t index, const void *item)
{
    bool ret_val;
    size_t number_of_elements;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;

    if(index > number_of_elements)
    {
        DC_ERROR_RAISE_SYSTEM(err, "", 1);
        ret_val = false;
    }
    else
    {
        struct node *new_node;

        new_node = dc_calloc(env, err, 1, sizeof(struct node));

        if(dc_error_has_no_error(err))
        {
            new_node->data = item;

            if(index == 0)
            {
                new_node->next = list->head;
                list->head = new_node;
                list->tail = new_node;
            }
            else if(index == number_of_elements)
            {
                new_node->prev = list->tail;
                list->tail->next = new_node;
                list->tail = new_node;
            }
            else
            {
                struct node *node;

                node = get_node_at(env, list->head, index - 1);
                // insert C between A and B
                // A<->B, NULL<-C->NULL

                // A->B, C<-B, NULL<-C->NULL
                node->next->prev = new_node;

                // A->B, C<-B, NULL<-C->B
                new_node->next = node->next;

                // A->C, C<-B, NULL<-C->B
                node->next = new_node;

                // A->B, C<-B, A<-C->B
                new_node->prev = node;
            }

            list->number_of_elements++;
            ret_val = true;
        }
        else
        {
            ret_val = true;
        }
    }

    check_list(env, err, list, number_of_elements + 1);

    if(dc_error_has_error(err))
    {
        ret_val = false;
    }

    return ret_val;
}

void *dc_linked_list_set(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, size_t index, const void *item)
{
    size_t number_of_elements;
    void *old_data;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;
    old_data = NULL;

    if(index > number_of_elements)
    {
        DC_ERROR_RAISE_SYSTEM(err, "", 1);
    }
    else
    {
        struct node *node;

        node = get_node_at(env, list->head, index);

        if(node)
        {
            old_data = node->data;
            node->data = item;
        }
        else
        {
            DC_ERROR_RAISE_SYSTEM(err, "", 1);
        }
    }

    check_list(env, err, list, number_of_elements);

    return old_data;
}

struct dc_linked_list_item dc_linked_list_get_first(const struct dc_env *env, const struct dc_linked_list *list)
{
    struct dc_linked_list_item item;

    DC_TRACE(env);

    if(list->head == NULL)
    {
        item.index = -1;
        item.data = NULL;
    }
    else
    {
        item.index = 0;
        item.data = list->head->data;
    }

    return item;
}

struct dc_linked_list_item dc_linked_list_get_last(const struct dc_env *env, const struct dc_linked_list *list)
{
    DC_TRACE(env);
    struct dc_linked_list_item item;

    DC_TRACE(env);

    if(list->head == NULL)
    {
        item.index = -1;
        item.data = NULL;
    }
    else
    {
        item.index = ((ssize_t)list->number_of_elements) - 1;
        item.data = list->tail->data;
    }

    return item;
}

struct dc_linked_list_item dc_linked_list_get_at(const struct dc_env *env, const struct dc_linked_list *list, size_t index)
{
    struct node *node;
    struct dc_linked_list_item item;

    node = get_node_at(env, list->head, index);
    item.index = (ssize_t)index;
    item.data = node->data;

    DC_TRACE(env);

    return item;
}

struct dc_linked_list_item dc_linked_list_get_first_occurrence(const struct dc_env *env, const struct dc_linked_list *list, const void *item)
{
    DC_TRACE(env);
}

struct dc_linked_list_item dc_linked_list_get_last_occurrence(const struct dc_env *env, const struct dc_linked_list *list, const void *item)
{
    DC_TRACE(env);
}

bool dc_linked_list_contains(const struct dc_env *env, const struct dc_linked_list *list, const void *item)
{
    DC_TRACE(env);
}

struct dc_linked_list_item dc_linked_list_remove_first(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list)
{
    size_t number_of_elements;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;
    check_list(env, err, list, list->number_of_elements - 1);
}

struct dc_linked_list_item dc_linked_list_remove_last(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list)
{
    size_t number_of_elements;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;
    check_list(env, err, list, list->number_of_elements - 1);
}

struct dc_linked_list_item dc_linked_list_remove_at(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, size_t index)
{
    size_t number_of_elements;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;
    check_list(env, err, list, list->number_of_elements - 1);
}

struct dc_linked_list_item dc_linked_list_remove_first_occurrence(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item)
{
    size_t number_of_elements;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;
    check_list(env, err, list, list->number_of_elements - 1);
}

struct dc_linked_list_item dc_linked_list_remove_last_occurrence(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item)
{
    size_t number_of_elements;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;
    check_list(env, err, list, list->number_of_elements - 1);
}

size_t dc_linked_list_remove_all_occurrences(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item)
{
    size_t number_of_elements;
    size_t count;

    DC_TRACE(env);
    number_of_elements = list->number_of_elements;
    check_list(env, err, list, list->number_of_elements - count);
}

ssize_t dc_linked_list_index_of(const struct dc_env *env, const struct dc_linked_list *list, const void *item)
{
    DC_TRACE(env);
}

ssize_t dc_linked_list_last_index_of(const struct dc_env *env, const struct dc_linked_list *list, const void *item)
{
    DC_TRACE(env);
}

void dc_linked_list_to_array(const struct dc_env *env, const struct dc_linked_list *list, void *array, size_t count)
{
    DC_TRACE(env);
}

void dc_linked_list_visit(const struct dc_env *env, struct dc_error *err, const struct dc_linked_list *list, dc_visitor visitor, void *state)
{
    struct node *tmp;

    DC_TRACE(env);
    tmp = list->head;

    while(tmp)
    {
        visitor(env, err, tmp->data, state);
        tmp = tmp->next;
    }
}
