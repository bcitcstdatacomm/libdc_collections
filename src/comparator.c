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
#include <dc_c/dc_string.h>


int dc_string_comparator(const struct dc_env *env, const void *item_a, const void *item_b)
{
    const char *str_a = item_a;
    const char *str_b = item_b;

    return dc_strcmp(env, item_a, item_b);
}
