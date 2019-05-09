gcc sample.c -o sample
yacc -v -t -d test.y
lex test.l
gcc y.tab.c lex.yy.c network.c -lm -ll -o network 
gcc client.c -o client
gcc y.tab.c lex.yy.c driver.c -lm -ll -o driver 
