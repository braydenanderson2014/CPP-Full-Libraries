#ifndef TYPETRAITS_H
#define TYPETRAITS_H

#include <string>
#include <cstddef>

template <typename T>
struct TypeTraits {
    static const bool isPointer = false;
    static const bool isReference = false;
};

template <typename T>
struct TypeTraits<T*> {
    static const bool isPointer = true;
    static const bool isReference = false;
};

template <typename T>
struct TypeTraits<T&> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T*&> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T&&> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T*&&> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T> {
    static const bool isPointer = false;
    static const bool isReference = false;
};

template <typename T>
struct TypeTraits<const T*> {
    static const bool isPointer = true;
    static const bool isReference = false;
};

template <typename T>
struct TypeTraits<const T&> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T*&> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T&&> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T*&&> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T* const> {
    static const bool isPointer = true;
    static const bool isReference = false;
};

template <typename T>
struct TypeTraits<T& const> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T* const&> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T&& const> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T*&& const> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T* const> {
    static const bool isPointer = true;
    static const bool isReference = false;
};

template <typename T>
struct TypeTraits<const T& const> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T* const&> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T&& const> {
    static const bool isPointer = false;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<const T* const&&> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct TypeTraits<T* const& const> {
    static const bool isPointer = true;
    static const bool isReference = true;
};

template <typename T>
struct is_arithmetic {
    static const bool value = false;
};

template <>
struct is_arithmetic<int> {
    static const bool value = true;
};

template <>
struct is_arithmetic<float> {
    static const bool value = true;
};

template <>
struct is_arithmetic<double> {
    static const bool value = true;
};

template <>
struct is_arithmetic<long> {
    static const bool value = true;
};

template <>
struct is_arithmetic<std::byte> {
    static const bool value = true;
};

template <typename T>
struct is_Char {
    static const bool value = false;
};

template <>
struct is_Char<char> {
    static const bool value = true;
};

template <typename T>
struct is_String {
    static const bool value = false;
};

template <>
struct is_String<std::string> {
    static const bool value = true;
};

template <typename T>
struct is_Integral {
    static const bool value = false;
};

template <>
struct is_Integral<int> {
    static const bool value = true;
};

template <>
struct is_Integral<long> {
    static const bool value = true;
};

template <>
struct is_Integral<char> {
    static const bool value = true;
};

template <>
struct is_Integral<bool> {
    static const bool value = true;
};

template <typename T>
struct is_floating_point {
    static const bool value = false;
};

template <>
struct is_floating_point<float> {
    static const bool value = true;
};

template <>
struct is_floating_point<double> {
    static const bool value = true;
};

template <typename T>
struct is_Array {
    static const bool value = false;
};

template <typename T>
struct is_Array<T[]> {
    static const bool value = true;
};

template <typename T>
struct is_Array<T[1]> {
    static const bool value = true;
};

template <typename T>
struct is_pointer {
    static const bool value = false;
};

template <typename T>
struct is_pointer<T*> {
    static const bool value = true;
};

template <typename T>
struct is_pointer<T* const> {
    static const bool value = true;
};

template <typename T>
struct is_reference {
    static const bool value = false;
};

template <typename T>
struct is_reference<T&> {
    static const bool value = true;
};

template <typename T>
struct is_const {
    static const bool value = false;
};

template <typename T>
struct is_const<const T> {
    static const bool value = true;
};

template <typename T>
struct is_volatile {
    static const bool value = false;
};

template <typename T>
struct is_volatile<volatile T> {
    static const bool value = true;
};

template <typename T, typename U>
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static const bool value = true;
};

template <typename T>
struct is_void {
    static const bool value = false;
};

// Check if a type is an enum
template<typename T>
struct is_enum {
    static const bool value = __is_enum(T);
};

// Check if a type is a union
template<typename T>
struct is_union {
    static const bool value = __is_union(T);
};

// Check if a type is a class or struct
template<typename T>
struct is_class {
    static const bool value = __is_class(T);
};

// Check if a type is abstract
template<typename T>
struct is_abstract {
    static const bool value = __is_abstract(T);
};

// Check if a type is empty (has no non-static data members)
template<typename T>
struct is_empty {
    static const bool value = __is_empty(T);
};

// Check if a type is polymorphic (has virtual functions)
template<typename T>
struct is_polymorphic {
    static const bool value = __is_polymorphic(T);
};

// Check if a type is final (cannot be inherited from)
template<typename T>
struct is_final {
    static const bool value = __is_final(T);
};

// Check if a type has a virtual destructor
template<typename T>
struct has_virtual_destructor {
    static const bool value = __has_virtual_destructor(T);
};

// Check if a type is signed
template<typename T>
struct is_signed {
    static const bool value = T(-1) < T(0);
};

// Check if a type is unsigned
template<typename T>
struct is_unsigned {
    static const bool value = T(0) < T(-1);
};

// Check if a type is a function
template<typename T>
struct is_function {
    static const bool value = false;
};

template<typename R, typename... Args>
struct is_function<R(Args...)> {
    static const bool value = true;
};

// Check if a type is a member function pointer
template<typename T>
struct is_member_function_pointer {
    static const bool value = false;
};

template<typename R, typename C, typename... Args>
struct is_member_function_pointer<R(C::*)(Args...)> {
    static const bool value = true;
};

// Check if a type is convertible to another type
template<typename From, typename To>
struct is_convertible {
private:
    static void helper(To);
    template<typename F>
    static char test(decltype(helper(std::declval<F>()))*);
    template<typename>
    static long test(...);
public:
    static const bool value = sizeof(test<From>(0)) == 1;
};

// Remove const qualifier
template<typename T>
struct remove_const {
    typedef T type;
};

template<typename T>
struct remove_const<const T> {
    typedef T type;
};

// Remove reference
template<typename T>
struct remove_reference {
    typedef T type;
};

template<typename T>
struct remove_reference<T&> {
    typedef T type;
};

template<typename T>
struct remove_reference<T&&> {
    typedef T type;
};

// Remove pointer
template<typename T>
struct remove_pointer {
    typedef T type;
};

template<typename T>
struct remove_pointer<T*> {
    typedef T type;
};

// Add const
template<typename T>
struct add_const {
    typedef const T type;
};

// Add lvalue reference
template<typename T>
struct add_lvalue_reference {
    typedef T& type;
};

// Add rvalue reference
template<typename T>
struct add_rvalue_reference {
    typedef T&& type;
};

// Conditional type selection
template<bool B, class T, class F>
struct conditional {
    typedef T type;
};

template<class T, class F>
struct conditional<false, T, F> {
    typedef F type;
};

// Enable if (for SFINAE)
template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> {
    typedef T type;
};

// Decay (remove reference and cv-qualifiers, and convert array/function to pointer)
template<typename T>
struct decay {
private:
    typedef typename remove_reference<T>::type U;
public:
    typedef typename conditional<
        is_array<U>::value,
        typename remove_extent<U>::type*,
        typename conditional<
            is_function<U>::value,
            typename add_pointer<U>::type,
            typename remove_cv<U>::type
        >::type
    >::type type;
};

// Alignment of type
template<typename T>
struct alignment_of {
    static const size_t value = alignof(T);
};

// Check if types are the same, ignoring const/volatile qualifiers
template<typename T, typename U>
struct is_same_ignoring_cv {
    static const bool value = is_same<
        typename remove_cv<T>::type,
        typename remove_cv<U>::type
    >::value;
};

// Type identity (useful for template metaprogramming)
template<typename T>
struct type_identity {
    using type = T;
};

// Integral constant (useful for compile-time constants)
template<typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

// Commonly used integral constants
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// Logical operations on type traits
template<typename... Ts>
struct conjunction : true_type {};

template<typename T, typename... Ts>
struct conjunction<T, Ts...> : conditional<T::value, conjunction<Ts...>, false_type>::type {};

template<typename... Ts>
struct disjunction : false_type {};

template<typename T, typename... Ts>
struct disjunction<T, Ts...> : conditional<T::value, true_type, disjunction<Ts...>>::type {};

template<typename T>
struct negation : integral_constant<bool, !T::value> {};

// Helper to detect if a type has a specific member function
#define DEFINE_HAS_MEMBER_FUNCTION(func_name)                           \
template<typename, typename T>                                          \
struct has_##func_name {                                                \
    static_assert(std::integral_constant<T, false>::value,              \
                  "Second template parameter needs to be of function type."); \
};                                                                      \
                                                                        \
template<typename C, typename Ret, typename... Args>                    \
struct has_##func_name<C, Ret(Args...)> {                               \
private:                                                                \
    template<typename T>                                                \
    static constexpr auto check(T*)                                     \
    -> typename std::is_same<                                           \
        decltype( std::declval<T>().func_name( std::declval<Args>()... ) ), \
        Ret    /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \
                 * attempt to call it and see if the return type is correct */ \
    >::type;                                                            \
    template<typename>                                                  \
    static constexpr std::false_type check(...);                        \
    typedef decltype(check<C>(0)) type;                                 \
public:                                                                 \
    static constexpr bool value = type::value;                          \
};

// Example usage of DEFINE_HAS_MEMBER_FUNCTION:
// DEFINE_HAS_MEMBER_FUNCTION(size)
// This would define has_size<T, Ret(Args...)> which checks if T has a size() member function
// with the specified return type and arguments.


#endif // TYPETRAITS_H