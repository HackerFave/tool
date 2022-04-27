#ifndef FRAMEWORK_COMMON_MACROS_H_
#define FRAMEWORK_COMMON_MACROS_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <new>
#include <type_traits>
#include <utility>
#include "log.h"

#define DEFINE_TYPE_TRAIT(name, func)                      \
  template <typename T>                                    \
  struct name {                                            \
    template <typename Class>                              \
    static constexpr bool Test(decltype(&Class::func) *) { \
      return true;                                         \
    }                                                      \
    template <typename>                                    \
    static constexpr bool Test(...) {                      \
      return false;                                        \
    }                                                      \
                                                           \
    static constexpr bool value = Test<T>(nullptr);        \
  };                                                       \
                                                           \
  template <typename T>                                    \
  constexpr bool name<T>::value;

DEFINE_TYPE_TRAIT(HasShutdown, Shutdown)

template <typename T>
typename std::enable_if<HasShutdown<T>::value>::type CallShutdown(T *instance) {
  instance->Shutdown();
}

template <typename T>
typename std::enable_if<!HasShutdown<T>::value>::type CallShutdown(
    T *instance) {
  (void)instance;
}

// There must be many copy-paste versions of these macros which are same
// things, undefine them to avoid conflict.
#undef UNUSED
#undef DISALLOW_COPY_AND_ASSIGN

#define UNUSED(param) (void)param

#define DISALLOW_COPY_AND_ASSIGN(classname) \
  classname(const classname &) = delete;    \
  classname &operator=(const classname &) = delete;

#define DECLARE_SINGLETON(classname)                                      \
 public:                                                                  \
  static classname *Instance(bool create_if_needed = true) {              \
    static classname *instance = nullptr;                                 \
    if (!instance && create_if_needed) {                                  \
      static std::once_flag flag;                                         \
      std::call_once(flag,                                                \
                     [&] { instance = new (std::nothrow) classname(); }); \
    }                                                                     \
    return instance;                                                      \
  }                                                                       \
                                                                          \
  static void CleanUp() {                                                 \
    auto instance = Instance(false);                                      \
    if (instance != nullptr) {                                            \
      CallShutdown(instance);                                             \
    }                                                                     \
  }                                                                       \
                                                                          \
 private:                                                                 \
  classname();                                                            \
  DISALLOW_COPY_AND_ASSIGN(classname)

#if !defined(RETURN_IF_NULL)
#define RETURN_IF_NULL(ptr)                 \
  if (ptr == nullptr)                       \
    do {                                    \
      LOG_Warn() << #ptr << " is nullptr."; \
      return;                               \
    } while(0)
#endif

#if !defined(RETURN_VAL_IF_NULL)
#define RETURN_VAL_IF_NULL(ptr, val)        \
  if (ptr == nullptr)                       \
    do {                                    \
      LOG_Warn() << #ptr << " is nullptr."; \
      return val;                           \
    } while(0)
#endif

#if !defined(RETURN_IF)
#define RETURN_IF(condition)                  \
  if (condition)                              \
    do {                                      \
      LOG_Warn() << #condition << " is met."; \
      return;                                 \
    } while(0)
#endif

#if !defined(RETURN_VAL_IF)
#define RETURN_VAL_IF(condition, val)         \
  if (condition)                              \
    do {                                      \
      LOG_Warn() << #condition << " is met."; \
      return val;                             \
    } while(0)
#endif

/**
* We can create a template function that simplifies our declarations of aligned
* shared pointers. Alignment and size are passed through to aligned malloc, and
* aligned free is always used as the deleter.  We then generate the correct pointer
* type based on the templated call
*
* Notice here that the shared pointer doesn't need a special type due to the deleter
* The deleter type is only required for the unique pointer.
*/
#ifndef _WIN32
template<class T>
std::shared_ptr<T> aligned_sptr(size_t align, size_t size)
{
	return std::shared_ptr<T>(static_cast<T*>(aligned_alloc(align, size)), &free);
}

template <class T>
std::shared_ptr<T> alloc_sptr(size_t size) {
  return std::shared_ptr<T>(static_cast<T *>(malloc(size)), &free);
}
#endif

#endif  // DATAX_COMMON_MACROS_H_
