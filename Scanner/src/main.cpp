#include "platform/ProcessEnumeratorWindows.h"
#include <iostream>

int main()
{
	auto i = core::ProcessEnumeratorWindows();
	i.list();

	int x;
	std::cin >> x;
}