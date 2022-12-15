#include "tests.h"
#include "dc_collections/linked_list.h"
#include <dc_env/env.h>
#include <dc_error/error.h>


// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

static struct dc_env *env;
static struct dc_error *err;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
Describe(linked_list);
#pragma GCC diagnostic pop

BeforeEach(linked_list)
{
    err = dc_error_create(false);
    env = dc_env_create(err, false, NULL);
}

AfterEach(linked_list)
{
    free(env);
    dc_error_reset(err);
    free(err);
}

Ensure(linked_list, test)
{
    struct dc_linked_list *list;
    struct dc_linked_list_item item;

    list = dc_linked_list_create(env, err, dc_string_comparator);
    assert_true(dc_linked_list_is_empty(env, list));
    assert_that(dc_linked_list_size(env, list), is_equal_to(0));

    dc_linked_list_add_last(env, err, list, "Hello");
    assert_false(dc_error_has_error(err));
    assert_false(dc_linked_list_is_empty(env, list));
    assert_that(dc_linked_list_size(env, list), is_equal_to(1));
    item = dc_linked_list_get_at(env, list, 0);
    assert_that(item.index, is_equal_to(0));
    assert_that(item.data, is_equal_to("Hello"));
}

TestSuite *linked_list_tests(void)
{
    TestSuite *suite;

    suite = create_test_suite();
    add_test_with_context(suite, linked_list, test);

    return suite;
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
