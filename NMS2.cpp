#include "NMS2.h"

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif
#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif

void NMS2::Initialize() {
	std::vector<std::vector<float>> NewMap;
	for (int x = 0; x < NMS2_DIVISOR; x++) {
		std::vector<float> Row;
		for (int y = 0; y < NMS2_DIVISOR; y++) {
			Row.push_back(0);
		}
		NewMap.push_back(Row);
	}
	NMS2Buffer = NewMap;
}
void NMS2::Tick() {
	for (int x = 0; x < NMS2_DIVISOR; x++) {
		for (int y = 0; y < NMS2_DIVISOR; y++) {
			NMS2Buffer[x][y] = MAX(0, NMS2Buffer[x][y] - NMS2_WEIGHT_NEGATIVE);
		}
	}
}
float Clamp(float in, float min, float max) {
	return (in > max) ? max : ((in < min) ? min : in);
}
void NMS2::AddRect(float x1, float y1, float x2, float y2) {
	x1 = round(Clamp(x1 * NMS2_DIVISOR, 0, NMS2_DIVISOR));
	y1 = round(Clamp(y1 * NMS2_DIVISOR, 0, NMS2_DIVISOR));
	x2 = round(Clamp(x2 * NMS2_DIVISOR, 0, NMS2_DIVISOR));
	y2 = round(Clamp(y2 * NMS2_DIVISOR, 0, NMS2_DIVISOR));
	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			NMS2Buffer[x][y] = MIN(1, NMS2Buffer[x][y] + NMS2_WEIGHT_POSITIVE);
		}
	}
}
bool NMS2::Query(float x, float y) {
	return (NMS2Buffer[(int)(x * NMS2_DIVISOR)][(int)(y * NMS2_DIVISOR)]) > NMS_QUERY_THRESHOLD;
}