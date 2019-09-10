%%
([A-Za-z_]([A-Za-z_]|[0-9])*)|(override)|(final) {printf("identifier %s\n", yytext);}
%%

int main( int argc , char** argv)
{
    yylex();
    return 0;
}