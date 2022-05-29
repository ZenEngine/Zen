#pragma once

#if ZEN_DEBUG
#include <cassert>
#define ZEN_EXPECTS(expression) assert(expression)
#define ZEN_EXPECTS_MSG(expression, message) assert(expression && message)

#define ZEN_ENSURES(expression) assert(expression)
#define ZEN_ENSURES_MSG(expression, message) assert(expression && message)

#define ZEN_ASSERT(expression) assert(expression)

#define ZEN_ASSERT_MSG(expression, message) ZEN_ASSERT(expression && message)

#define ZEN_ASSERT_SLOW(x) ZEN_ASSERT(x)

#define ZEN_ASSERT_NO_ENTRY() ZEN_ASSERT(false)

#define ZEN_ASSERT_NO_ENTRY_MSG(message) ZEN_ASSERT_MSG(false, message)

#define ZEN_VERIFY(expression) assert(expression)

#else
#define ZEN_EXPECTS(expression) ((void)0)
#define ZEN_EXPECTS_MSG(expression, message) ((void)0)
#define ZEN_ENSURES(expression) ((void)0)
#define ZEN_ENSURES_MSG(expression, message) ((void)0)
#define ZEN_ASSERT(expression) ((void)0)
#define ZEN_ASSERT_MSG(expression, message) ((void)0)
#define ZEN_ASSERT_SLOW(x) ((void)0)
#define ZEN_ASSERT_NO_ENTRY() ((void)0)
#define ZEN_ASSERT_NO_ENTRY_MSG(message) ((void)0)
#define ZEN_VERIFY(expression) (expression)
#endif