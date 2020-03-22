//
// Created by Jason Zheng on 2020/3/22.
//

#ifndef SANDRA_COMPILER_ATTR_H
#define SANDRA_COMPILER_ATTR_H

/*
 * GCC:
 * Function Attributes - https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 * Variable Attributes - https://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html
 * Type Attributes - https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html
 * Enumerator Attributes - https://gcc.gnu.org/onlinedocs/gcc/Enumerator-Attributes.html
 * Statement Attributes - https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html
 * Attribute Syntax - https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html
 *
 * You may optionally specify attribute names with ‘__’ preceding and following the name.
 * This allows you to use them in header files without being concerned about a possible macro
 * of the same name. For example, you may use the attribute name __noreturn__ instead of noreturn.
 *
 * Clang:
 * Attributes in Clang - https://clang.llvm.org/docs/AttributeReference.html
 */


/* =========== Common Function Attributes =========== */
#define sdr_attr_access(...) \
    __attribute__((__access__(__VA_ARGS__)))

#define sdr_attr_alias(target) \
    __attribute__((__alias__(target)))

#define sdr_attr_aligned(...) \
    __attribute__((__aligned__(__VA_ARGS__)))

#define sdr_attr_alloc_align(position) \
    __attribute__((__alloc_align__(position)))

#define sdr_attr_alloc_size(...) \
    __attribute__((__alloc_size__(__VA_ARGS__)))

#define sdr_attr_always_inline \
   __attribute__((__always_inline__))

#define sdr_attr_artificial \
   __attribute__((__artificial__))

#define sdr_attr_assume_aligned(...) \
     __attribute__((__assume_aligned__(__VA_ARGS__)))

#define sdr_attr_cold \
   __attribute__((__cold__))

#define sdr_attr_const \
   __attribute__((__const__))

#define sdr_attr_constructor(...) \
     __attribute__((__constructor__(__VA_ARGS__)))

#define sdr_attr_destructor(...) \
     __attribute__((__destructor__(__VA_ARGS__)))

#define sdr_attr_copy(...) \
     __attribute__((__copy__(__VA_ARGS__)))

#define sdr_attr_deprecated(...) \
     __attribute__((__deprecated__(__VA_ARGS__)))

#define sdr_attr_error(msg) \
     __attribute__((__error__(msg)))

#define sdr_attr_warning(msg) \
     __attribute__((__warning__(msg)))

#define sdr_attr_externally_visible \
   __attribute__((__externally_visible__))

#define sdr_attr_flatten \
   __attribute__((__flatten__))

#define sdr_attr_format(archetype, string_index, first_to_check) \
   __attribute__((__format__(archetype, string_index, first_to_check)))

/* shorthand */
#define sdr_attr_printf_like(string_index, first_to_check) \
    sdr_attr_format(__printf__, string_index, first_to_check)

#define sdr_attr_format_arg(string_index) \
   __attribute__((__format_arg__(string_index)))

#define sdr_attr_gnu_inline \
   __attribute__((__gnu_inline__))

#define sdr_attr_hot \
   __attribute__((__hot__))

#define sdr_attr_ifunc(resolver) \
   __attribute__((__ifunc__(resolver)))

#define sdr_attr_interrupt \
   __attribute__((__interrupt__))

#define sdr_attr_interrupt_handler \
   __attribute__((__interrupt_handler__))

#define sdr_attr_leaf \
   __attribute__((__leaf__))

#define sdr_attr_malloc \
   __attribute__((__malloc__))

#define sdr_attr_no_icf \
   __attribute__((__no_icf__))

#define sdr_attr_no_instrument_function \
   __attribute__((__no_instrument_function__))

#define sdr_attr_no_profile_instrument_function \
   __attribute__((__no_profile_instrument_function__))

#define sdr_attr_no_reorder \
   __attribute__((__no_reorder__))

#define sdr_attr_no_sanitize(sanitize_option) \
   __attribute__((__no_sanitize__(sanitize_option)))

#define sdr_attr_no_sanitize_address \
   __attribute__((__no_sanitize_address__))

#define sdr_attr_no_address_safety_analysis \
   __attribute__((__no_address_safety_analysis__))

#define sdr_attr_no_sanitize_thread \
   __attribute__((__no_sanitize_thread__))

#define sdr_attr_no_sanitize_undefined \
   __attribute__((__no_sanitize_undefined__))

#define sdr_attr_no_sanitize_undefined \
   __attribute__((__no_sanitize_undefined__))

#define sdr_attr_no_split_stack \
   __attribute__((__no_split_stack__))

#define sdr_attr_no_stack_limit \
   __attribute__((__no_stack_limit__))

#define sdr_attr_noclone \
   __attribute__((__noclone__))

#define sdr_attr_noinline \
   __attribute__((__noinline__))

#define sdr_attr_noipa \
   __attribute__((__noipa__))

#define sdr_attr_nonnull(...) \
     __attribute__((__nonnull__(__VA_ARGS__)))

#define sdr_attr_noplt \
     __attribute__((__noplt__))

#define sdr_attr_noreturn \
     __attribute__((__noreturn__))

#define sdr_attr_nothrow \
     __attribute__((__nothrow__))

#define sdr_attr_optimize(level, ...) \
     __attribute__((__optimize__(level, __VA_ARGS__)))

#define sdr_attr_patchable_function_entry \
     __attribute__((__patchable_function_entry__))

#define sdr_attr_pure \
     __attribute__((__pure__))

#define sdr_attr_returns_nonnull \
     __attribute__((__returns_nonnull__))

#define sdr_attr_returns_twice \
     __attribute__((__returns_twice__))

#define sdr_attr_section(section_name) \
     __attribute__((__section__(section_name)))

#define sdr_attr_sentinel(...) \
     __attribute__((__sentinel__(__VA_ARGS__)))

#define sdr_attr_simd(...) \
     __attribute__((__simd__(__VA_ARGS__)))

#define sdr_attr_stack_protect \
     __attribute__((__stack_protect__))

#define sdr_attr_target(string, ...) \
     __attribute__((__target__(string, __VA_ARGS__)))

#define sdr_attr_symver(ver) \
     __attribute__((__symver__(ver)))

#define sdr_attr_target_clones(options) \
     __attribute__((__target_clones__(options)))

#define sdr_attr_unused \
     __attribute__((__unused__))

#define sdr_attr_used \
     __attribute__((__used__))

#define sdr_attr_visibility(visibility_type) \
     __attribute__((__visibility__(visibility_type)))

#define sdr_attr_warn_unused_result \
     __attribute__((__warn_unused_result__))

#define sdr_attr_weak \
     __attribute__((__weak__))

#define sdr_attr_weakref(...) \
     __attribute__((__weakref__(__VA_ARGS__)))


/* =========== Common Variable Attributes =========== */
/* alias ("target") */

/*
 * aligned
 * aligned (alignment)
 */

#define sdr_attr_warn_if_not_aligned(alignment) \
    __attribute__((__warn_if_not_aligned__(alignment)))

/*
 * alloc_size (position)
 * alloc_size (position-1, position-2)
 */

#define sdr_attr_cleanup(cleanup_function) \
    __attribute__((__cleanup__(cleanup_function)))

#define sdr_attr_common \
    __attribute__((__common__))

#define sdr_attr_nocommon \
    __attribute__((__nocommon__))

/*
 * copy
 * copy (variable)
 */

/*
 * deprecated
 * deprecated (msg)
 */

#define sdr_attr_mode(mode) \
    __attribute__((__mode__(mode)))

#define sdr_attr_nonstring \
    __attribute__((__nonstring__))

#define sdr_attr_packed \
    __attribute__((__packed__))

#define sdr_attr_section(section_name) \
    __attribute__((__section__(section_name)))

#define sdr_attr_tls_model(tls_model) \
    __attribute__((__tls_model__(tls_model)))

/* unused */

/* used */

#define sdr_attr_vector_size(bytes) \
    __attribute__((__vector_size__(bytes)))

/* visibility ("visibility_type") */

/* weak */

#define sdr_attr_noinit \
    __attribute__((__noinit__))


/* ============= Common Type Attributes ============= */
/*
 * aligned
 * aligned (alignment)
 */

/* warn_if_not_aligned (alignment) */

/*
 * alloc_size (position)
 * alloc_size (position-1, position-2)
 */

/*
 * copy
 * copy (expression)
 */

/*
 * deprecated
 * deprecated (msg)
 */

#define sdr_attr_designated_init \
    __attribute__((__designated_init__))

#define sdr_attr_may_alias \
    __attribute__((__may_alias__))

/* mode (mode) */

/* packed */

#define sdr_attr_scalar_storage_order(endianness) \
    __attribute__((__scalar_storage_order__(endianness)))

#define sdr_attr_transparent_union \
    __attribute__((__transparent_union__))

/* unused */

/* vector_size (bytes) */

/* visibility */


/* ================ Label Attributes ================ */
/* unused */

/* hot */

/* cold */


/* ============= Enumerator Attributes ============== */
/* deprecated */


/* ============== Statement Attributes ============== */
#define sdr_attr_fallthrough \
    __attribute__((__fallthrough__))


#endif //SANDRA_COMPILER_ATTR_H
