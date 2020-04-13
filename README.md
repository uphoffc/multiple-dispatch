# Multiple dispatch

C++17 brought us support for [multiple dispatch](https://en.wikipedia.org/wiki/Multiple_dispatch)
with [std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit).
std::visit is used on type-safe unions ([std::variant](https://en.cppreference.com/w/cpp/utility/variant)).
However, unions might not always be desirable (e.g. if sizeof(A) much larger than sizeof(B)).

This project brings magi::visit which works like std::visit but does not require std::variant.
Instead, the only limitation is that visitable classes have a vtable.

## Usage

Make types visitable by inheriting from magi::type

```cxx
using tp1 = magi::type<class A,
                       class B>;
using tp2 = magi::type<class C,
                       class D,
                       class E>;

class A : public magi::with_type<A,tp1> {};
class B : public magi::with_type<B,tp1> {};
class C : public magi::with_type<C,tp2> {};
class D : public magi::with_type<D,tp2> {};
class E : public magi::with_type<E,D> {}; // Inherits from D
```

Call magi::visit

```cxx
  A a; E e;
  tp1* t1 = &a;
  tp1* t2 = &e;
  
  // return ae
  magi::visit(magi::make_visitor{
      [](A&, E&) { return "ae"; },
      [](B&, C&) { return "bc"; },
      [](auto&, auto&) { return "other"; }
  }, *t1, *t2, *t3) );
```
