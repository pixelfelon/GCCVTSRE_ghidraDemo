#include <iostream>

using namespace std;

class Base {
  public:
    virtual void foo (void);
    virtual void bar (void);
    unsigned int b;
};

void Base::foo(void) {
    cout << "foo" << endl;
}

void Base::bar(void) {
    cout << "bar" << endl;
}

class Quirk {
  public:
    virtual void quirk (void);
    void * Q;
};

void Quirk::quirk(void) {
    cout << "quirk" << endl;
}

class Derived : public Quirk, public Base {
  public:
    virtual void baz (void);
    void bar (void);
    unsigned char d;
};

void Derived::bar(void) {
    Base::bar();
    cout << "Derived" << endl;
}

void Derived::baz(void) {
    quirk();
    cout << "baz" << endl;
}


void do_bar(Base& object) {
    object.bar();
}


int main()
{
    Base * obj = new Base();
    obj->bar();

    // Upcasting
    Derived * obj2 = new Derived();
    Base * obj2b = static_cast<Base*>(obj2);
    obj2->foo();
    obj2b->foo();
    do_bar(*obj2);
    do_bar(*obj);

    return 0;
}
