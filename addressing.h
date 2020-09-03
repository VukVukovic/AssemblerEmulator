#ifndef ADDRESSING_H
#define ADDRESSING_H

#include <string>
#include "register.h"
#include "encoding.h"

using namespace std;

enum AddressingType { IMMED, REGDIR, REGIND, PCREL, MEMDIR};

class Addressing {
protected:
    virtual char operandDescr() const = 0;

public:
    virtual AddressingType getType() const = 0;
    virtual int getPreferedSize() const = 0;
    virtual int getBytesSize(int operSize) const = 0;
    virtual Encoding getEncoding(int size) const = 0;
    virtual void setPCRelOffset(int pcRelOffset) {}
    virtual ~Addressing() {}
    Addressing() = default;
    Addressing(const Addressing&) = delete;
};

class ImmedLiteral : public Addressing {
    int value;

protected:
    char operandDescr() const override { return (char)((0x0 << 5)); }

public:
    ImmedLiteral(int value) : value(value) {}
    AddressingType getType() const override { return IMMED; }
    int getPreferedSize() const override { return 0; }
    int getBytesSize(int operSize) const override { return 1+operSize; }
		Encoding getEncoding(int size) const override;
};

class ImmedSymbol : public Addressing {
    string symbol;

protected:
    char operandDescr() const override { return (char)((0x0 << 5)); }

public:
    ImmedSymbol(const string& symbol) : symbol(symbol) {}
    AddressingType getType() const override { return IMMED; }
    int getPreferedSize() const override { return 0; }
    int getBytesSize(int operSize) const override { return 1+operSize; }
		Encoding getEncoding(int size) const override;
};

class RegisterDirect : public Addressing {
    Register reg;

protected:
    char operandDescr() const override { return (char)((0x1 << 5) + (reg.getEncoding() << 1) + reg.getLH()); }

public:
    RegisterDirect(Register reg) : reg(reg) {}
    AddressingType getType() const override { return REGDIR; }
    int getPreferedSize() const override { return reg.getSize(); }
    int getBytesSize(int operSize) const override { return 1; }
		Encoding getEncoding(int size) const override;
};

class RegisterIndirect : public Addressing {
    Register reg;

protected:
    char operandDescr() const override { return (char)((0x2 << 5) + (reg.getEncoding() << 1)); }

public:
    RegisterIndirect(Register reg) : reg(reg) {}
    AddressingType getType() const override { return REGIND; }
    int getPreferedSize() const override { return 0; }
    int getBytesSize(int operSize) const override { return 1; }
		Encoding getEncoding(int size) const override;
};

class RegisterIndDispLiteral : public Addressing {
    Register reg;
    int value;

protected:
    char operandDescr() const override { return (char)((0x3 << 5) + (reg.getEncoding() << 1)); }

public:
    RegisterIndDispLiteral(Register reg, int value) : reg(reg), value(value) {}
    AddressingType getType() const override { return REGIND; }
    int getPreferedSize() const override { return 2; }
    int getBytesSize(int operSize) const override { return 1+2; }
		Encoding getEncoding(int size) const override;
};

class RegisterIndDispSymbol : public Addressing {
    Register reg;
    string symbol;

protected:
    char operandDescr() const override { return (char)((0x3 << 5) + (reg.getEncoding() << 1)); }

public:
    RegisterIndDispSymbol(Register reg, const string& symbol) : reg(reg), symbol(symbol) {}
    AddressingType getType() const override { return REGIND; }
    int getPreferedSize() const override { return 2; }
    int getBytesSize(int operSize) const override { return 1+2; }
		Encoding getEncoding(int size) const override;
};

class PCRelative : public Addressing {
    string symbol;
    int pcRelOffset;

protected:
    char operandDescr() const override { return (char)((0x3 << 5) + (7 << 1)); }

public:
    PCRelative(const string& symbol) : symbol(symbol) {}
    AddressingType getType() const override { return PCREL; }
    int getPreferedSize() const override { return 0; }
    int getBytesSize(int operSize) const override { return 1+2; }
    void setPCRelOffset(int pcRelOffset) override { this->pcRelOffset = pcRelOffset; }
		Encoding getEncoding(int size) const override;
};

class MemDirLiteral : public Addressing {
    int value;

protected:
    char operandDescr() const override { return (char)((0x4 << 5)); }

public:
    MemDirLiteral(int value) : value(value) {}
    AddressingType getType() const override { return MEMDIR; }
    int getPreferedSize() const override { return 0; }
    int getBytesSize(int operSize) const override { return 1+2; }
		Encoding getEncoding(int size) const override;
};

class MemDirSymbol : public Addressing {
    string symbol;

protected:
    char operandDescr() const override { return (char)((0x4 << 5)); }

public:
    MemDirSymbol(const string& symbol) : symbol(symbol) {}
    AddressingType getType() const override { return MEMDIR; }
    int getPreferedSize() const override { return 0; }
    int getBytesSize(int operSize) const override { return 1+2; }
		Encoding getEncoding(int size) const override;
};
#endif
