# FReader
This is a simple project for Theory of Programming Language.  
## Basic Rules
The object of this project is to transform a given formula into another new formula according to the given rules.  
I suppose that the variables are in the form of "[a-z][0-9]*" (i.e. a12 and b are valid variables, but abc and A are not). The operators are {"=>", "&", "|", "~"}.  
Example:  <pre>((a&b)|((~a)&c))=>(b=>c)
((b&a)=>((~d)|c))
(a=>c&b&d)
b&(c|a)</pre>
I provide seven rules here. In front of " : " is the original formula, after ":" is the new formula.  
<pre>(1) a=>b:(~a)|b
(2) (a|(b&c)):((a|b)&(a|c))
(3) (a|b):(b|a)
(4) (a&b):(b&a)
(5) (~(a&b)):((~a)|(~b))
(6) (~(a|b)):((~a)&(~b))
(7) (~(~a)):a</pre>
The input file are the list of original formulas "formula.txt" and the list of rules "regle.txt". There's no need to use all of the rules. You can choose what you like. The output file is the list of new formulas "result.txt".
## Lexical Analysis
I used regex to match the variables ([a-z][0-9]*) and used DFA to match parentheses and operators.
## Syntactic Analysis
I designed my CFG to do the syntactic analysis.  
<pre>grammar antlr;
NUMBER : [0-9];
CHAR: [a-z];
OP: '&' | '|' | '=>';
variable: CHAR NUMBER*;
term: facteur | '~' facteur | facteur OP facteur;
facteur: variable | '(' term ')';</pre>
## Apply rules
The following work is to build trees for the given formula by using resursion algorithm. Search the node available for applying rules by DFS. When find the node, change the structure of the tree according to the rules.  
To find all the possible results, I store the tree of each generated formula in the stack. The program won't stop until the stack is empty.
