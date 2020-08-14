#ifndef ADDRESSING_H
#define ADDRESSING_H

#include <string>
#include "register.h"
#include "encoding.h"

using namespace std;

class Addressing {
protected:
    virtual char operandDescr() const = 0;
    static void addBytes(int value, int size, vector<char>& bytes);

public:
    virtual bool isImmed() const = 0;
    virtual int getPreferedSize() const = 0;
    virtual Encoding getEncoding(int size) const = 0;
};

class ImmedLiteral : public Addressing {
    int value;

protected:
    char operandDescr() const override { return (char)((0x0 << 5)); }

public:
    ImmedLiteral(int value) : value(value) {}
    bool isImmed() const override { return true; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
};

class ImmedSymbol : public Addressing {
    string symbol;

protected:
    char operandDescr() const override { return (char)((0x0 << 5)); }

public:
    ImmedSymbol(const string& symbol) : symbol(symbol) {}
    bool isImmed() const override { return true; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
};

class RegisterDirect : public Addressing { 
    Register reg;

protected:
    char operandDescr() const override { return (char)((0x1 << 5) + (reg.getEncoding() << 1) + reg.getLH()); }

public:
    RegisterDirect(Register reg) : reg(reg) {}
    bool isImmed() const override { return false; }
    int getPreferedSize() const override { return reg.getSize(); }
    Encoding getEncoding(int size) const override;
};

class RegisterIndirect : public Addressing { 
    Register reg;

protected:
    char operandDescr() const override { return (char)((0x2 << 5) + (reg.getEncoding() << 1)); }

public:
    RegisterIndirect(Register reg) : reg(reg) {}
    bool isImmed() const override { return false; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
};

class RegisterIndDispLiteral : public Addressing {
    Register reg;
    int value;

protected:
    char operandDescr() const override { return (char)((0x3 << 5) + (reg.getEncoding() << 1)); }

public:
    RegisterIndDispLiteral(Register reg, int value) : reg(reg), value(value) {}
    bool isImmed() const override { return false; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
}; 

class RegisterIndDispSymbol : public Addressing {
    Register reg;
    string symbol;

protected:
    char operandDescr() const override { return (char)((0x3 << 5) + (reg.getEncoding() << 1)); }

public:
    RegisterIndDispSymbol(Register reg, const string& symbol) : reg(reg), symbol(symbol) {}
    bool isImmed() const override { return false; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
};

class PCRelative : public Addressing {
    string symbol;

protected:
    char operandDescr() const override { return (char)((0x3 << 5) + (7 << 1)); }

public:
    PCRelative(const string& symbol) : symbol(symbol) {}
    bool isImmed() const override { return false; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
};

class MemDirLiteral : public Addressing {
    int value;

protected:
    char operandDescr() const override { return (char)((0x4 << 5)); }

public:
    MemDirLiteral(int value) : value(value) {}
    bool isImmed() const override { return false; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
};

class MemDirSymbol : public Addressing {
    string symbol;

protected:
    char operandDescr() const override { return (char)((0x4 << 5)); }

public:
    MemDirSymbol(const string& symbol) : symbol(symbol) {}
    bool isImmed() const override { return false; }
    int getPreferedSize() const override { return 0; }
    Encoding getEncoding(int size) const override;
};
#endif