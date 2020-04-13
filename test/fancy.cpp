#include "test.h"

#include <string>
#include <algorithm>

#include <magi/type.hpp>
#include <magi/visit.hpp>

using tp1 = magi::type<class A,
                       class B>;
using tp2 = magi::type<class C,
                       class D,
                       class E>;
using tp3 = magi::type<class F,
                       class G>;

class A : public magi::with_type<A,tp1> {};
class B : public magi::with_type<B,tp1> {};
class C : public magi::with_type<C,tp2> {};
class D : public magi::with_type<D,tp2> {};
class E : public magi::with_type<E,tp2> {};
class F : public magi::with_type<F,tp3> {};
class G : public magi::with_type<G,tp3> {};

int main(int argc, char** argv) {
  tp1* t1 = nullptr;
  tp2* t2 = nullptr;
  tp3* t3 = nullptr;

  A a;
  E e;
  G g;
  if (argc >= 0) {
    t1 = &a;
    t2 = &e;
    t3 = &g;
  }

  TEST("aeg" == magi::visit(magi::make_visitor{
      [](A&, E&, G&) { return "aeg"; },
      [](B&, C&, F&) { return "bcf"; },
      [](auto&, auto&, auto&) { return "other"; }
  }, *t1, *t2, *t3) );

  return 0;
}

