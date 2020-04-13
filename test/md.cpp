#include <cstdio>
#include <cstdlib>
#define TO_STRING(x) #x
#define TEST(x) if (!(x)) { printf("Assertion failed: (%s) in %s@%d.\n", TO_STRING(x), __FILE__, __LINE__); abort(); }


#include <magi/type.hpp>
#include <magi/visit.hpp>

using tp = magi::type<class A,
                      class B,
                      class C,
                      class D>;

class A : public magi::with_type<A,tp> {
};
class B : public magi::with_type<B,A> {
public:
  double b = 1.;
};
class C : public magi::with_type<C,B> {
public:
  double c = 2.;
};
class D : public magi::with_type<D,tp> {
public:
  double d = 3.;
};

struct Vis {
    double operator()(B& b, C& c) { return b.b / c.c; }
    double operator()(C& c, B& b) { return c.c / b.b; }
    double operator()(D& d, C& c) { return d.d / c.c; }
    double operator()(C& c, D& d) { return c.c / d.d; }
    template<typename T, typename U>
    double operator()(T&, U&) { return 0.0; }
};

int main(int argc, char** argv) {
  B b;
  C c;
  D d;

  tp* t0 = &b;
  tp* t1 = &c;
  tp* t2 = &d;

  if (argc >= 0) {
    t1 = &d;
    t2 = &c;
  }

  TEST(1.0/2.0 == magi::visit(Vis(), b, *t2));
  TEST(2.0/1.0 == magi::visit(Vis(), *t2, b));
  TEST(3.0/2.0 == magi::visit(Vis(), *t1, *t2));
  TEST(2.0/3.0 == magi::visit(Vis(), *t2, *t1));

  return 0;
}

