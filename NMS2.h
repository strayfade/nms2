// Non-maximum suppression filter by Strayfade <3

#include <iostream>
#include <vector>

// See README.md for configuration options
#define NMS2_DIVISOR 33
#define NMS_QUERY_THRESHOLD 0.5
#define NMS2_ADD_AMOUNT 0.08f
#define NMS2_SUBTRACT_AMOUNT 0.05f

namespace NMS2 {
	inline std::vector<std::vector<float>> NMS2Buffer;

	void Initialize();
	void Tick();

	// 0-1 Values
	void AddRect(float x1, float y1, float x2, float y2);
	bool Query(float x, float y);
}