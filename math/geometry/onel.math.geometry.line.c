
#include "onel.math.geometry.line.h"



float get_y_2points_line(float x0, float x1, float y0, float y1, float x)
{
	if (x1-x0 == 0)
		return 0.0;
	float m = (y1 - y0)/(x1 - x0);
	float y = m * (x - x0) + y0;
	return y;
}
