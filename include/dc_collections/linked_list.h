#ifndef LIBDC_COLLECTIONS_LINKED_LIST_H
#define LIBDC_COLLECTIONS_LINKED_LIST_H


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


#include "dc_collections/comparator.h"
#include "dc_collections/visitor.h"
#include <dc_env/env.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>


struct dc_linked_list;


struct dc_linked_list_item
{
    ssize_t index;
    void *data;
};


#ifdef __cplusplus
extern "C" {
#endif


struct dc_linked_list *dc_linked_list_create(const struct dc_env *env, struct dc_error *err, dc_comparator comparator);
void dc_linked_list_destroy(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list);
bool dc_linked_list_is_empty(const struct dc_env *env, const struct dc_linked_list *list);
size_t dc_linked_list_size(const struct dc_env *env, const struct dc_linked_list *list);
void dc_linked_list_clear(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list);
bool dc_linked_list_add_first(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item);
ssize_t dc_linked_list_add_last(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item);
bool dc_linked_list_add_at(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, size_t index, const void *item);
void *dc_linked_list_set(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, size_t index, const void *item);
struct dc_linked_list_item dc_linked_list_get_first(const struct dc_env *env, const struct dc_linked_list *list);
struct dc_linked_list_item dc_linked_list_get_last(const struct dc_env *env, const struct dc_linked_list *list);
struct dc_linked_list_item dc_linked_list_get_at(const struct dc_env *env, const struct dc_linked_list *list, size_t index);
struct dc_linked_list_item dc_linked_list_get_first_occurrence(const struct dc_env *env, const struct dc_linked_list *list, const void *item);
struct dc_linked_list_item dc_linked_list_get_last_occurrence(const struct dc_env *env, const struct dc_linked_list *list, const void *item);
bool dc_linked_list_contains(const struct dc_env *env, const struct dc_linked_list *list, const void *item);
struct dc_linked_list_item dc_linked_list_remove_first(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list);
struct dc_linked_list_item dc_linked_list_remove_last(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list);
struct dc_linked_list_item dc_linked_list_remove_at(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, size_t index);
struct dc_linked_list_item dc_linked_list_remove_first_occurrence(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item);
struct dc_linked_list_item dc_linked_list_remove_last_occurrence(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item);
size_t dc_linked_list_remove_all_occurrences(const struct dc_env *env, struct dc_error *err, struct dc_linked_list *list, const void *item);
ssize_t dc_linked_list_index_of(const struct dc_env *env, const struct dc_linked_list *list, const void *item);
ssize_t dc_linked_list_last_index_of(const struct dc_env *env, const struct dc_linked_list *list, const void *item);
void dc_linked_list_to_array(const struct dc_env *env, const struct dc_linked_list *list, void *array, size_t count);
void dc_linked_list_visit(const struct dc_env *env, struct dc_error *err, const struct dc_linked_list *list, dc_visitor visitor, void *state);


/*
bool addAll(Collection<? extends E> c)
boolean addAll(int index, Collection<? extends E> c)
bool addAll(int index, Collection<? extends E> c)
bool containsAll(Collection<?> c)
bool removeAll(Collection<?> c)
bool retainAll(Collection<?> c)
Object clone()
Iterator<E> descendingIterator()
Iterator<E> iterator()
ListIterator<E> listIterator()
ListIterator<E> listIterator(int index)
Spliterator<E> spliterator()
boolean equals(Object o)
int hashCode()
List<E> subList(int fromIndex, int toIndex)
String toString()
*/


#ifdef __cplusplus
}
#endif


#endif //LIBDC_COLLECTIONS_LINKED_LIST_H
