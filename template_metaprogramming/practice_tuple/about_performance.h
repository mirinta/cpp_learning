#pragma once
namespace example {
/// class template
template <typename... Ts>
struct list
{
};

/// alias
template <typename... Ts>
using my_list = list<Ts...>;

/// function template
template <typename... Ts>
void func(Ts...)
{
}
} // namespace example

/** performance of operations (from fast to slow):
 * 1. Looking up a memorized type (LIGHTNING FAST)
 * - When list<int> is first called, the type will be stored in a hash table.
 *   The next time it encounters this type, it'll just look it up in the hash table.
 *
 * 2. Adding a parameter to an alias call
 * - Example: my_list<int> vs. my_list<int, bool>
 *
 * 3. Adding a parameter to a type
 * - Example: list<int> (first use) vs. list<int, bool> (first use)
 *
 * 4. Calling an alias
 * - Example: my_list<int,...>
 *
 * ---------------------------------------------------------------------------
 * Try to avoid operations below this line,
 * especially for intermediate results of recursive computations.
 * ---------------------------------------------------------------------------
 *
 * 5. Instantiating a type
 * - Example: first use of list<int,...>, need allocation
 *
 * 6. Instantiating a function template
 * - Example: first use of func(1), need allocation and other operations
 *
 * 7. SFINAE (SUPER SLOW)
 * - The compiler must go through the entire overload stack instantiating everything in it.
 * Even if it finds a matching overload, it needs to check everything to make sure there is no
 * ambiguity.
 */

////////////////////////////////////////
////////////////// IF //////////////////
////////////////////////////////////////
namespace slow_impl {
template <bool CONDITION, typename THEN, typename ELSE>
struct if_
{
    using type = THEN;
};

template <typename THEN, typename ELSE>
struct if_<false, THEN, ELSE>
{
    using type = ELSE;
};

/**
 * struct A{}; struct B{};
 * 1. if_<true, A, B>, first call -> instantiation
 * 2. if_<false, A, B>, different values -> new type
 * 3. if_<false, B, A>, different order -> new type, but it should be equivalent to line 1
 * 4. if_<true, A, B>, lookup
 */
} // namespace slow_impl

namespace faster_impl {
template <bool CONDITION>
struct if_
{
    template <typename THEN, typename ELSE>
    using f = THEN;
};

template <>
struct if_<false>
{
    template <typename THEN, typename ELSE>
    using f = ELSE;
};

/**
 * struct A{}; struct B{};
 * 1. if_<true>::template f<A, B>, instantiation
 * 2. if_<false>::template f<A, B>, instantiation
 * 3. if_<false>::template f<B, A>, lookup
 * 4. if_<true>::template f<A, B>, lookup
 */
} // namespace faster_impl
