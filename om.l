%{
    #include "om.h"
    #include "om.tab.h++"

    void yyerror(const char *,...);

    extern int linenum;
    extern YYSTYPE yylval;
    extern void debug(const char *, ...);

    using namespace std;
%}

%% 

"+"         { L_DEBUG("token: ADD"); return ADD; }
"-"         { L_DEBUG("token: SUB"); return SUB; }
"*"         { L_DEBUG("token: MUL"); return MUL; }
"/"         { L_DEBUG("token: DIV"); return DIV; }
"%"         { L_DEBUG("token: MOD"); return MOD; }
"=="        { L_DEBUG("token: EQ"); return EQ; }
"!="        { L_DEBUG("token: NE"); return NE; }
"<="        { L_DEBUG("token: LE"); return LE; }
"<"         { L_DEBUG("token: LT"); return LT; }
">="        { L_DEBUG("token: GE"); return GE; }
">"         { L_DEBUG("token: GT"); return GT; }
"&&"        { L_DEBUG("token: AND"); return AND; }
"||"        { L_DEBUG("token: LOR"); return LOR; }
"!"	    { L_DEBUG("token: NOT"); return NOT; }
"("         { L_DEBUG("token: LP"); return LP; }
")"         { L_DEBUG("token: RP"); return RP; }
"="         { L_DEBUG("token: SET"); return SET; }
"."         { L_DEBUG("token: DOT"); return DOT; }
","         { L_DEBUG("token: COM"); return COM; }
";"         { L_DEBUG("token: COM"); return COM; }
":"         { L_DEBUG("token: COL"); return COL; }
"|"	    { L_DEBUG("token: CAT"); return CAT; }
"prt"       { L_DEBUG("token: PRT"); return PRT; }
"for"       { L_DEBUG("token: FOR"); return FOR; }
"fin"       { L_DEBUG("token: FIN"); return FIN; }
"brk"       { L_DEBUG("token: BRK"); return BRK; }
"con"       { L_DEBUG("token: CON"); return CON; }
"if"        { L_DEBUG("token: IF"); return IF;  }
"else"      { L_DEBUG("token: ELSE"); return ELSE; }
"fi"        { L_DEBUG("token: FI"); return FI;  }
"ret"       { L_DEBUG("token: RET"); return RET; }
"end"       { L_DEBUG("token: END"); return END; }


\r\n { /* newlines are equal to commas in om (dos) */
    linenum++;
    L_DEBUG("token: COM");
    return COM;
}

\n { /* newlines are equal to commas in om */
    linenum++; 
    L_DEBUG("token: COM");
    return COM; 
}


[0-9]+ { /* integer constant */
    yylval.s = strMalloc(yytext, yyleng);
    L_DEBUG("token: INT");
    return INT; 
}


[0-9]*\.[0-9]+ { /* floating point constant */
    yylval.s = strMalloc(yytext, yyleng);
    L_DEBUG("token: FLO");
    return FLO;
}


\"[^\"]*\" { /* string constant */
    yylval.s = strMalloc(yytext, yyleng);
    L_DEBUG("token: STR");
    return STR;
}

[a-z][a-zA-Z0-9_]* {  /* identifier */
    yylval.s = strMalloc(yytext, yyleng);
    L_DEBUG("token: ID");
    return ID;
}


[ ] { ; } /* space */
\t { ; } /* tab   */
\r { ; } /* ignore */
[ \t]*#.*$ { ; } /* comment line */

. { 
    yyerror("illegal token:%s line:%d", yytext, linenum); 
    exit(1);
}

%%

#ifndef yywrap
int yywrap(void) { return 1; }
#endif
