#ifndef MAGI_TYPE_H_
#define MAGI_TYPE_H_

#include <type_traits>
#include <limits>

namespace md {
  template<typename... Ts>
  class type;

  namespace detail {
    template<int start, typename For, typename... In> struct get_type_id {
      static constexpr int result = -1;
    };

    template<int start, typename For, typename Head, typename... Tail>
    struct get_type_id<start,For,Head,Tail...> {
      static constexpr int result = std::is_same_v<For,Head>
        ? start
        : get_type_id<start+1,For,Tail...>::result;
    };

    template<int I, typename... Ts>
    struct get_type;

    template<int I, typename Head, typename... Tail>
    struct get_type<I,Head,Tail...> {
      using type = typename get_type<I-1, Tail...>::type;
    };

    template<typename Head, typename... Tail>
    struct get_type<0,Head,Tail...> {
      using type = Head;
    };

    template<int I, typename... Ts>
    using get_type_t = typename get_type<I,Ts...>::type;

    template<int I, typename... Ts>
    constexpr get_type_t<I, Ts...>& cast_type(type<Ts...>& t) {
      return static_cast<get_type_t<I, Ts...>&>(t);
    }

    template<int I, typename... Ts>
    constexpr get_type_t<I, Ts...> const& cast_type(type<Ts...> const& t) {
      return static_cast<get_type_t<I, Ts...> const&>(t);
    }
  }

  template<typename... Ts>
  class type {
  public:
    template<typename T>
    static constexpr int compute_type_id(T const*) {
      constexpr int theType = detail::get_type_id<0,T,Ts...>::result;
      static_assert(sizeof...(Ts) < std::numeric_limits<int>::max(), "too many types in typelist");
      static_assert(theType >= 0, "type not in typelist");
      return theType;
    }

    virtual int get_type_id() const noexcept = 0;

    static constexpr auto get_num_types() { return sizeof...(Ts); }
  };

  template<typename T, typename... Ts>
  bool is_type(type<Ts...> const* obj) {
    return obj->get_type_id() == type<Ts...>::compute_type_id(static_cast<T const*>(obj));
  }

  template<typename Derived, typename Base>
  class with_type : public Base {
  public:
    using Base::Base;

    int get_type_id() const noexcept override {
      return Base::compute_type_id(static_cast<Derived const*>(this));
    }
  };
}




#endif // MAGI_TYPE_H_
