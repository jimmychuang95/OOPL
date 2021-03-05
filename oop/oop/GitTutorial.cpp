#include "stdafx.h"
#include <string>
#include <icstream>


using namespace  System;

bool Guess(int number) {
}

int main(array<System::string^>^args)

{
	int guess;
	do {
		std::cout << "Choose a number between 1 ~ 100 :";
		std::cin >> guess;
	} while (!Guess(guess));
	return 0;
}
