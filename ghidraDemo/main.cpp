#include <iostream>

using namespace std;

class Base {
  public:
    Base(unsigned int b) : b(b) {}
    virtual void foo (void);
    virtual void bar (void);
    unsigned int b;
};

void Base::foo(void) {
    cout << "foo" << endl;
}

void Base::bar(void) {
    cout << "bar " << b << endl;
}

class Quirk {
  public:
    virtual void quirk (void);
    void * Q = nullptr;
};

void Quirk::quirk(void) {
    cout << "quirk " << Q << endl;
}

class Derived : public Quirk, public Base {
  public:
    Derived(unsigned int b, unsigned char d) : Base(b), d(d) {}
    virtual void baz (void);
    void bar (void);
    unsigned char d;
};

void Derived::bar(void) {
    cout << "Derived ";
    Base::bar();
}

void Derived::baz(void) {
    quirk();
    cout << "baz " << d << endl;
}


void do_bar(Base& object) {
    object.bar();
}


int main()
{
    Base * objBase = new Base(1);
    objBase->foo();
    objBase->bar();
    cout << endl;

    Quirk * objQuirk = new Quirk();
    objQuirk->Q = objQuirk;
    objQuirk->quirk();
    cout << endl;

    Derived * objDerived = new Derived(2, '3');
    objDerived->Q = objDerived;
    objDerived->foo();
    objDerived->bar();
    objDerived->baz();
    objDerived->quirk();
    cout << endl;

    // Upcasting
    Base * derivedAsBase = static_cast<Base*>(objDerived);
    objDerived->foo();
    derivedAsBase->foo();
    do_bar(*objDerived);
    do_bar(*derivedAsBase);

    return 0;
}
