# Symbol Table

This is an implementation of a symbol table data structure. <br>
It is designed over the 'Spaghetti stack' technique. <br>


### Spaghetti Stack

A spaghetti stack is an N-ary tree data structure in which child nodes <br>
have pointers to the parent nodes (but not vice-versa)

Spaghetti stack structure is used in situations when records are dynamically <br>
pushed and popped onto a stack as execution progresses, but references to the <br>
popped records remain in use. Following are some applications of Spaghetti Stack. <br>

Compilers, for languages such as C, create a spaghetti stack as it opens and closes symbol tables<br>
representing block scopes. When a new block scope is opened, a symbol table is pushed onto a stack. <br>
When the closing curly brace is encountered, the scope is closed and the symbol table is popped. <br>
But that symbol table is remembered, rather than destroyed. <br>
And of course it remembers its higher level “parent” symbol table and so on.
