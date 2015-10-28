/* File: SymbolTable.h
* ---------------------
* Implementation of symbol table and symbol table entries. Symbol table
* is implemented as a "spaghetti stack", as it opens and closes symbol tables 
* representing block scopes (exactly as a compiler does).
*/

#ifndef _H_SYMTABLE
#define _H_SYMTABLE

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "TType.h"
#include "hashtable.h"
#include "list.h"

namespace llvm {

class TType;
class SymbolTable;

class Symbol {
    private:
        std::string key;
        TType* ttype;
        int scope;
        SymbolTable* gamma;

    public:
        Symbol(std::string k, TType* t, int scope);
        Symbol(std::string k, TType* t, int scope, SymbolTable *e);

        std::string getKey();
        void setKey(std::string k);

        TType* getTType();
        void setTType(TType* t);

        int getScope();
        void setScope(int s);

        SymbolTable *getGamma();
        void setGamma(SymbolTable *g);

        void print();
};

//#########################################################################

class SymbolTable {
    private:
        SymbolTable *_prev;       // Pointer to SymbolTable of parent scope
        SymbolTable *_super;      // Pointer to SymbolTable of parent class
        SymbolTable *_this;       // Scope of curr class, NULL if no class

        Hashtable<Symbol*> *_table;   // Pointer to hash table for scope
        List<SymbolTable*> *_blocks;  // For printing purposes only

    public:
        SymbolTable();

        void setParent(SymbolTable *p);
        SymbolTable *getParent();

        void setSuper(SymbolTable *s);
        SymbolTable *getSuper();

        void setThis(SymbolTable *t);
        SymbolTable *getThis();

        void setTable(SymbolTable *t);
        SymbolTable *getTable();

        Symbol* addSymbol(std::string key,
                            TType* ttype,
                            int scope,
                            SymbolTable* gamma);

        SymbolTable *addWithScope(std::string key,
                                 TType* ttype,
                                 int scope);

        SymbolTable *newScope();

        Symbol *findSuper(std::string key);
        Symbol *findSuper(std::string key, TType* ttype);

        Symbol *findLocal(std::string key);
        Symbol *findLocal(std::string key, TType* ttype);

        Symbol *findInClass(std::string key);
        Symbol *findInClass(std::string key, TType* ttype);

        Symbol *find(std::string key);
        Symbol *find(std::string key, TType* ttype);

        Symbol* findInAll(SymbolTable* gamma, std::string key, int typeID);
        Symbol* findInAll(SymbolTable* gamma, std::string key);

        Symbol* findInGlobal(SymbolTable* globalEnv, std::string key);

        Symbol *findUp(std::string key);
        Symbol *findUp(std::string key, TType* ttype);

        Symbol *findClassField(std::string className,
                                std::string fieldName);
        Symbol *findClassField(std::string className,
                                std::string fieldName,
                                TType* ttype);

        List<SymbolTable*>* getBlocks();
        void print();
        int getSize();
};
}
#endif
