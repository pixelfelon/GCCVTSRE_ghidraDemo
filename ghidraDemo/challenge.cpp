// SPOILERS!!!!
// Don't read this source code until you've tried to reverse engineer the binary!
// (Unless you're no fun.)












































































#include <iostream>
#include <signal.h>
#include <stdint.h>

using namespace std;


const int uibits = sizeof(uint32_t) * 8;

static inline uint32_t
rotl (uint32_t n, uint32_t c)
{
  return (n << c) | (n >> (32 - c));
}

static inline uint32_t
rotr (uint32_t n, uint32_t c)
{
  return (n << (uibits - c)) | (n >> c);
}


class CrypticBase {
  public:
    void load(const void * in_d, unsigned int in_sz)
    {
        this->in_d = in_d;
        this->in_sz = in_sz;
        accumulator = 0;
        crunch();
    }

    uint32_t result()
    {
        return accumulator;
    }

    virtual ~CrypticBase() {}

  protected:
    const void * in_d = nullptr;
    unsigned int in_sz = 0;
    uint32_t accumulator = 0;

    virtual unsigned int rounds() = 0;
    virtual void round() = 0;
    virtual void step(uint32_t d) = 0;

    virtual void crunch()
    {
        for (unsigned int r = 0; r < rounds(); r++)
        {
            round();
        }
    }
};


class CrypticHash : public CrypticBase {
  protected:
    virtual void round_update();

    virtual void round()
    {
        for (unsigned int i = 0; i < in_sz; i++)
        {
            step(*((char *)in_d + i));
        }
        round_update();
    }

    virtual void step(uint32_t d)
    {
        accumulator = rotr(accumulator, 4);
        accumulator ^= d;
    }
};

class CrypticHash702 : public CrypticHash {
  protected:
    virtual unsigned int rounds()
    {
        return 8;
    }

    virtual void round_update()
    {
        accumulator = (
            rotl(accumulator % 0xAD2514E7, 7)
            ^ rotr(accumulator % 0x035F4A39, 10)
        );
    }
};


class ObjSysObj {
  public:
      ObjSysObj() {}
      virtual ~ObjSysObj() {}

      virtual void * parent() = 0;
      virtual const char * name() = 0;

  protected:
      const char * what_is_this = "Not Qt";
};

class CryptoDelegate {
  public:
    virtual ~CryptoDelegate()
    {
        if (nullptr != crypt)
        {
            delete crypt;
        }
    }

    virtual uint32_t
    cryptify_string(string& str)
    {
        get_crypt().load(str.c_str(), str.size());
        return get_crypt().result();
    }

  protected:
    CrypticBase * crypt = nullptr;

    virtual void init() = 0;
    CrypticBase& get_crypt()
    {
        if (nullptr == crypt)
        {
            init();
        }
        return *crypt;
    }
};

class CryptoDelegateImpl : public CryptoDelegate, public ObjSysObj {
  public:
    virtual void init()
    {
        crypt = new CrypticHash702();
    }

    virtual void * parent() { return nullptr; }
    virtual const char * name() { return what_is_this; }
};

class Controller {
  public:
    Controller(CryptoDelegate * cd) : cryptoDelegate(cd) {}

    void prompt()
    {
        string input;
        cout << "Enter some text:" << endl;
        getline(cin, input);

        cout << "Here is a number:" << endl;
        cout << cryptoDelegate->cryptify_string(input) << endl;
    }

  protected:
    CryptoDelegate * cryptoDelegate;
};


static bool run = true;

void sigint_handler(int s)
{
    run = false;
}

int main()
{
    CryptoDelegate * crypt = new CryptoDelegateImpl();
    Controller controller = Controller(crypt);
    signal(SIGINT, sigint_handler);

    while (run)
    {
        controller.prompt();
    }

    delete crypt;
    return 0;
}
