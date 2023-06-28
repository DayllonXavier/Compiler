# SLR(0) Table Generator  
How to use:  
Write Grammar description in a text file with the format:  
```
1 S' -> S  
2 S -> ( L )  
3 S -> x  
4 L -> S  
5 L -> L , S  
```
```bash
./generator grammar.txt
```  
Output: multiples files used in the parser:  
  
action.txt  
goto.txt  
follow.txt  
terminalList.txt  
naoTerminalList.txt  
qtd.txt  
rule_size.txt  

Issues:  
    Currently has no support for ε (empty string)  
