%{
#include "utile.h"
#include "parser.h"

%}
D[0-9]
L[a-zA-Z_]
%%
"+"      { return PLUS;   }
"-"      { return MOINS;  }
"*"      { return STAR;   }
"&"      { return REF;    }
"/"      { return DIV;    }
"("      { return PO;     }
")"      { return PF;     }
"{"      { return AO;     }
"}"      { return AF;     }
"="      { return EQ;     }
"if"     { return IF;     }
"else"   { return ELSE;   }
","      { return VIR;    }
"<"      { return INF;    }
">"      { return SUP;    }
"=="     { return EQUAL;  }
"!="     { return DIFF;   }
"&&"     { return AND;    }
"||"     { return OR;     }
"!"      { return NOT;    }
";"      { return PV;     }
"do"     { return DO;     }
"while"  { return WHILE;  }
"until"  { return UNTIL;  }
"return" { return RETURN; }
"int"    { yylval.t_type = INT_T; return INT;     }
"float"  { yylval.t_type = FLOAT_T; return FLOAT; }
"void"   { yylval.t_type = VOID_T; return VOID;   }

[$]?{L}({L}|{D})*    { yylval.id=strdup(yytext); return(ID);  }
{D}+    	           { yylval.n=atoi(yytext); return(CONSTANTI); }
{D}+"."{D}*          { yylval.f=atof(yytext); return(CONSTANTF); }
\n                   { ; }
[ \t]                { /* on avale sans rendre la main */;       }
