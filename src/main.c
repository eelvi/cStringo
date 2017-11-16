#include "cstringo.h"

int main(){

	char str[50] = "best library on the planet!.\n";
	puts(str);
	str_repl(str, "best", "worst");
	puts(str);

}
