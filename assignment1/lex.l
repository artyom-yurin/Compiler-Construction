%%
alignas|continue|friend|register|true|alignof|decltype|goto {printf("keyword %s\n", yytext);}
reinterpret_cast|try|asm|default|if|return|typedef|auto|delete {printf("keyword %s\n", yytext);}
inline|short|typeid|bool|do|int|signed|typename|break|double {printf("keyword %s\n", yytext);}
long|sizeof|union|case|dynamic_cast|mutable|static|unsigned|catch {printf("keyword %s\n", yytext);}
else|namespace|static_assert|using|char|enum|new|static_cast|virtual {printf("keyword %s\n", yytext);}
char16_t|explicit|noexcept|struct|void|char32_t|export|nullptr|switch|volatile|class {printf("keyword %s\n", yytext);}
extern|operator|template|wchar_t|const|false|private|this|while|constexpr|float|protected|thread_local|const_cast|for|public|throw {printf("keyword %s\n", yytext);}

((([0-9]+)((e|E)(\+|-)?[0-9]+))|(([0-9]*\.[0-9]+)|([0-9]+\.))((e|E)(\+|-)?[0-9]+)?)(f|F|l|L)? {printf("float-literal %s\n", yytext);}

(([1-9][0-9]*)|(0[0-7]*)|(((0x)|(0X))[0-9a-fA-F]+)|((((0b)|(0B))[0-1]+)))(((u|U)((ll)|(LL)|(l)|(L))?)|(((ll)|(LL)|(l)|(L))(u|U)?))? {printf("integer-literal %s\n", yytext);}

([A-Za-z_]([A-Za-z_]|[0-9])*)|(override)|(final) {printf("identifier %s\n", yytext);}
%%

int main( int argc , char** argv)
{
    yylex();
    return 0;
}