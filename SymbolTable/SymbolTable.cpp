/*
* SymbolTable.cpp
* -----------------
* Symbol table implementation
*/

#include "SymbolTable.h"

SymbolTable::SymbolTable() 
{
    _table = new Hashtable<Symbol*>;
    _blocks = new List<SymbolTable*>;
    _this = NULL;
    _super = NULL;
    _prev = NULL;
}

void 
SymbolTable::setParent(SymbolTable *p) 
{
    _prev = p;
}

SymbolTable *
SymbolTable::getParent() 
{
    return _prev;
}

void 
SymbolTable::setSuper(SymbolTable *s) 
{
    _super = s;
}

SymbolTable *
SymbolTable::getSuper() 
{
    return _super;
}

void 
SymbolTable::setThis(SymbolTable *t) 
{
    _this = t;
}

SymbolTable *
SymbolTable::getThis() 
{
    return _this;
}

Symbol* 
SymbolTable::addSymbol(std::string key, TType* ttype, int scope, SymbolTable* gamma) 
{
    assert(ttype && gamma);
    Symbol *sym = new Symbol(key, ttype, scope, gamma);
    assert(sym && "None symbol created!");
    assert(_table && "Empty table!");
    _table->enter(key, sym, false);
    return sym;
}

SymbolTable *
SymbolTable::addWithScope(std::string key, TType* ttype, int scope) 
{
    SymbolTable *newGamma = new SymbolTable;
    Symbol *sym;
    newGamma->setParent(this);

    if (_this) 
    {
        newGamma->setThis(_this);
    }

    sym = new Symbol(key, ttype, scope, newGamma);
    assert(sym && "No symbol to enter to ST");
    _table->enter(key, sym, false);
    _blocks->append(newGamma);

    return newGamma;
}

SymbolTable *
SymbolTable::newScope() 
{
    SymbolTable *child = new SymbolTable;
    child->setParent(this);

    if (_this) 
    {
        child->setThis(_this);
    }
    _blocks->append(child);

    return child;
}


Symbol *
SymbolTable::findSuper(std::string key) 
{
    SymbolTable *current = _super;

    if (!_super) 
    {
        return NULL;
    }

    for ( ; current != NULL; current = current->getSuper()) 
    {
        if (Symbol* sym = current->findLocal(key)) 
        {
            return sym;
        }
    }
    return NULL;
}

Symbol *
SymbolTable::findSuper(std::string key, TType* ttype) 
{
    Symbol *s = findSuper(key);
    return (s && s->getTType() == ttype) ? s : NULL;
}

Symbol *
SymbolTable::findInClass(std::string key) 
{
    Symbol *sym = NULL;

    if ((sym = findLocal(key)) != NULL) 
    {
        return sym;
    }

    if ((sym = findSuper(key)) != NULL) 
    {
        return sym;
    }
    return NULL;
}

Symbol *
SymbolTable::findInClass(std::string key, TType* type) 
{
    Symbol *s = findInClass(key);
    return (s && s->getTType() == type) ? s : NULL;
}

Symbol *
SymbolTable::findLocal(std::string key) 
{
    Symbol *sym;

    if ((sym = _table->lookup(key)) != NULL) 
    {
        return sym;
    }

    if (_super) 
    {
        if ((sym = findSuper(key)) != NULL) 
        {
            return sym;
        }
    }
    return NULL;
}

Symbol *
SymbolTable::findLocal(std::string key, TType* type) 
{
    Symbol *s = findLocal(key);
    return (s && s->getTType() == type) ? s : NULL;
}

Symbol *
SymbolTable::find(std::string key) 
{
    SymbolTable *current = this;
    Symbol *sym;

    for ( ; current != NULL; current = current->getParent()) 
    {
        if ((sym = current->findLocal(key)) != NULL) 
        {
            return sym;
        }
    }

    if (_super) 
    {
        if ((sym = findSuper(key)) != NULL) 
        {
            return sym;
        }
    }
    return NULL;
}

Symbol* 
SymbolTable::findInAll(SymbolTable* gamma, std::string key, int typeID) 
{
    assert(gamma && "No gamma!");
    SymbolTable* current = gamma;
    List<SymbolTable*>* blocks = current->getBlocks();

    for (int i = 0; i < blocks->getNumElements(); i++) 
    {
        SymbolTable* tmpST = blocks->getNth(i);
        Symbol* sym = tmpST->findLocal(key);
        if ( (sym != NULL) && (sym->getTType()->getTTypeID() == typeID) ) 
        {
            return sym;
        }
    }
    return NULL;
}

Symbol* 
SymbolTable::findInAll(SymbolTable* gamma, std::string key) 
{
    assert(gamma && "No gamma!");
    SymbolTable* current = gamma;
    List<SymbolTable*>* blocks = current->getBlocks();

    for (int i = 0; i < blocks->getNumElements(); i++) 
    {
        SymbolTable* tmpST = blocks->getNth(i);
        Symbol* sym = tmpST->findLocal(key);
        if (sym != NULL) 
        {
            return sym;
        }
    }
    return NULL;
}

Symbol* 
SymbolTable::findInGlobal(SymbolTable* globalEnv, std::string key)
{
    assert(globalEnv && "No gamma!");
    Symbol* sym;

    if ( (sym = globalEnv->findLocal(key)) != NULL ) 
    {
        return sym;
    }
    return NULL;
}

Symbol *
SymbolTable::find(std::string key, TType* type) 
{
    Symbol *s = find(key);
    return (s && s->getTType() == type) ? s : NULL;
}

Symbol *
SymbolTable::findUp(std::string key) 
{
    SymbolTable *current = _prev;

    for ( ; current != NULL; current = current->getParent())
    {
        if (Symbol* sym = current->findLocal(key)) 
        {
            return sym;
        }
    }
    return NULL;
}

Symbol *
SymbolTable::findUp(std::string key, TType* type) 
{
    Symbol *s = this->findUp(key);
    return (s && s->getTType() == type) ? s : NULL;
}

Symbol *
SymbolTable::findClassField(std::string className, std::string fieldName) 
{
    Symbol *classSym = SymbolTable::find(className);
    if (classSym == NULL) 
    {
        return NULL;
    }
    return classSym->getGamma()->find(fieldName);
}

Symbol *
SymbolTable::findClassField(std::string className, std::string fieldName, TType* type) 
{
    Symbol *s = this->findClassField(className, fieldName);
    return (s && s->getTType() == type) ? s : NULL;
}

int
SymbolTable::getSize() 
{
    return _table->getNumEntries();
}

List<SymbolTable*>* 
SymbolTable::getBlocks() 
{
    return this->_blocks;
}

void 
SymbolTable::print() 
{
    int indentLevel = 0;
    const char numSpaces = 3;
    printf("\n");
    printf("%*s%s [Entries:%d] [ref this %p]\n", numSpaces * indentLevel,
                "",
                ">Scope Table",
                _table->getNumEntries(),
                _this);

    _table->printall();
}

//###########################################################################

/* Class: Symbol
* -------------
* Implementation for symbol class
*/
Symbol::Symbol(std::string k, TType* t, int s) 
{
    key = k;
    ttype = t;
    scope = s;
    gamma = NULL;
}

Symbol::Symbol(std::string k, TType* t, int s, SymbolTable *e) 
{
    key = k;
    ttype = t;
    scope = s;
    gamma = e;
}

std::string 
Symbol::getKey() 
{
    return key;
}

void 
Symbol::setKey(std::string  k)
{
    key = k;
}

TType* 
Symbol::getTType() 
{
    return ttype;
}

void 
Symbol::setTType(TType* t) 
{
    ttype = t;
}

int 
Symbol::getScope() 
{
    return scope;
}

void 
Symbol::setScope(int s) 
{
    scope = s;
}

SymbolTable *
Symbol::getGamma() 
{
    return gamma;
}

void 
Symbol::setGamma(SymbolTable *g) 
{
    gamma = g;
}
