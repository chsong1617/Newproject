#include <iostream>
#include <stdlib.h>

class LetDebug {
public:
	void printNum() {
		int* value = new int;
		*value = 10;
		std::cout << *value << std::endl;
	}
};
int main(void) {
	LetDebug* ld = new LetDebug;
	ld->printNum();
}
