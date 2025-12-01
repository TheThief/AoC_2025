#pragma once

import std;

int div_roundpos(int x, int y)
{
	return x / y + ((x % y) > 0);
}

int div_roundneg(int x, int y)
{
	return x / y - ((x % y) < 0);
}
