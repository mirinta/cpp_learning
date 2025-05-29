#pragma once

/**
 * @brief The List class
 */
template <typename... Ts>
struct List
{
    static constexpr unsigned long long size = sizeof...(Ts);
};

/**
 * @brief The function_pointer class
 */
template <typename Func>
struct function_pointer;

template <typename Ret, typename... Args>
struct function_pointer<Ret (*)(Args...)>
{
    using return_type = Ret;
    using args_list = List<Args...>;
};

/**
 * @brief The is_convertible class
 */
template <typename From, typename To>
struct is_convertible
{
    static int check(To);
    static char check(...);
    static From get();
    static constexpr bool value = sizeof(int) == sizeof(check(get()));
};

template <typename T>
struct is_convertible<T, void>
{
    static constexpr bool value = true;
};

template <>
struct is_convertible<void, void>
{
    static constexpr bool value = true;
};

template <typename T1, typename T2>
inline constexpr bool is_convertible_v = is_convertible<T1, T2>::value;

/**
 * @brief The is_list_compatible class
 */
template <typename FromList, typename ToList>
struct is_list_compatible
{
    static constexpr bool value = false;
};

template <typename List1, typename List2>
inline constexpr bool is_list_compatible_v = is_list_compatible<List1, List2>::value;

template <typename From, typename To, typename... Froms, typename... Tos>
struct is_list_compatible<List<From, Froms...>, List<To, Tos...>>
{
    static constexpr bool value =
        is_convertible_v<From, To> && is_list_compatible_v<List<Froms...>, List<Tos...>>;
};

template <typename... Ts>
struct is_list_compatible<List<Ts...>, List<>>
{
    static constexpr bool value = true;
};

/**
 * @brief The is_connectable class
 */
template <typename SignalFunc, typename SlotFunc>
struct is_connectable
{
    using signal = function_pointer<SignalFunc>;
    using slot = function_pointer<SlotFunc>;
    static constexpr bool value =
        signal::args_list::size >= slot::args_list::size &&
        is_list_compatible_v<typename signal::args_list, typename slot::args_list> &&
        is_convertible_v<typename slot::return_type, typename signal::return_type>;
};

template <typename SignalFunc, typename SlotFunc>
inline constexpr bool is_connectable_v = is_connectable<SignalFunc, SlotFunc>::value;

void signal(int, char);
void slot(double);
static_assert(is_connectable_v<decltype(&signal), decltype(&slot)>);
