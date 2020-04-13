#ifndef MAGI_VISIT_H_
#define MAGI_VISIT_H_

#include <type_traits>
#include <utility>

#include "type.hpp"

namespace magi {
  namespace detail {
    template<typename... Ts>
    struct compute_stride {
      static constexpr int get(int fibre, int current = 1) {
        return 0;
      }
    };

    template<typename Head, typename... Tail>
    struct compute_stride<Head,Tail...> {
      static constexpr int get(int fibre, int current = 1) {
        return fibre == 0
          ? current
          : compute_stride<Tail...>::get(
              fibre-1,
              current * std::decay_t<Head>::get_num_types()
              );
      }
    };

    constexpr int flatten() {
      return 0;
    }
    template<typename Head, typename... Tail>
    constexpr int flatten(Head&& head, Tail&&... tail) {
      return head.get_type_id() + flatten(tail...) * std::decay_t<Head>::get_num_types();
    }

    template<int index, int stride, typename T>
    constexpr auto& unflatten(T& t) {
      return detail::cast_type<(index / stride) % std::decay_t<T>::get_num_types()>(t);
    }

    template<int index, typename Visitor, typename... Ts, int... Is>
    constexpr auto dispatch(Visitor&& visitor, Ts&... ts, std::integer_sequence<int, Is...>) {
      return visitor(unflatten<index,compute_stride<Ts...>::get(Is),Ts>(ts)...);
    }

    // https://stackoverflow.com/questions/46278997/variadic-templates-and-switch-statement
    template<typename T, T... Is, typename F>
    auto compile_switch(T i, std::integer_sequence<T, Is...>, F f) {
      using return_type = std::common_type_t<decltype(f(std::integral_constant<T,Is>{}))...>;
      if constexpr (std::is_same_v<return_type,void>) {
        [[maybe_unused]] bool discard =
          ((i == Is ? f(std::integral_constant<T, Is>{}),false : false) || ...);
        return;
      } else {
        return_type ret;
        [[maybe_unused]] bool discard =
          ((i == Is ? (ret = f(std::integral_constant<T, Is>{})),false : false) || ...);
        return ret;
      }
    }
  }

  template<typename Visitor, typename... TypeIds>
  auto visit(Visitor&& visitor, TypeIds&&... ts) {
    constexpr int table_size = (std::decay_t<TypeIds>::get_num_types() * ...);
    return detail::compile_switch(
        detail::flatten(ts...),
        std::make_integer_sequence<int, table_size>{},
        [&] (auto i) -> auto {
        return detail::dispatch<i,Visitor,TypeIds...>(
            std::forward<Visitor>(visitor),
            std::forward<TypeIds>(ts)...,
            std::make_integer_sequence<int, sizeof...(TypeIds)>{}
            );
        }
        );
  }

  template<class... Ts> struct make_visitor : Ts... { using Ts::operator()...; };
  template<class... Ts> make_visitor(Ts...) -> make_visitor<Ts...>;
}

#endif // MAGI_VISIT_H_
