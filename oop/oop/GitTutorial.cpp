﻿#include "stdafx.h"
#include <string>
#include <icstream>


using namespace  System;

bool Guess(int number) {
	static int target = -1;

	if (target == -1) {
		Random r;
		target = r.Next() % 100 + 1;
	}


	if (number > target) {
		std::_Count_pr << "Smaller" << std::end;
		return flase;
	}
	else if (number < target) {
		std::_Count_pr << "Bigger" << std::endl;
		return false;
	}
	return true;

}

int main(array<System::string^>^args)

{
	int guess;
	do {
		std::cin >> guess;
	} while (!Guess(guess));
	return 0;
}
