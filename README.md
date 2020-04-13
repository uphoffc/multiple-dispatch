# Multiple dispatch

C++17 brought us support for [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch)
with [std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit).
std::visit is used on type-safe unions ([std::variant](https://en.cppreference.com/w/cpp/utility/variant)).
However, unions might not always be desirable (e.g. if sizeof(A) much larger than sizeof(B)).

This project brings md::visit which works like std::visit but does not require std::variant.
Instead, the only limitation is that visitable classes have a vtable.

## Usage

Make types visitable by inheriting from md::type

```cxx
using tp1 = md::type<class A,
                       class B>;
using tp2 = md::type<class C,
                       class D,
                       class E>;

class A : public md::with_type<A,tp1> {};
class B : public md::with_type<B,tp1> {};
class C : public md::with_type<C,tp2> {};
class D : public md::with_type<D,tp2> {};
class E : public md::with_type<E,D> {}; // Inherits from D
```

Call md::visit

```cxx
  A a; E e;
  tp1* t1 = &a;
  tp2* t2 = &e;
  
  // return ae
  md::visit(md::make_visitor{
      [](A&, E&) { return "ae"; },
      [](B&, C&) { return "bc"; },
      [](auto&, auto&) { return "other"; }
  }, *t1, *t2) );
```
