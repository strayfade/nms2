# nms2
A better algorithm for eliminating false object detections in OpenCV (specifically for more reliable inference of videos)

> **nms2** was previously used in closed-source software!

### Why does this exist?
The default function used by OpenCV (C++) for consolidating multiple object detection results into a single bounding box is `cv2.dnn.NMSBoxes`

Although this function works for the most part, false detections can become a problem when performing subsequent passes, especially on media such as video files. Even if there are no false positives for a few frames, some will likely occur eventually. 

It was clear that there had to be a better way to eliminate false positives in video-based inference.

### The solution
The best way to solve this problem would be to use an algorithm that can track how an object detection propogates over time inside of a buffer. This is the purpose of this repository, **nms2**.

### Usage

```C++
#include "NMS2.h"

// Placed at the beginning of a source file, before all inference
NMS2::Initialize();

// Placed before every inference frame ([network].forward)
NMS2::Tick();

// Placed inside of a loop iterating through all object detection results
// Arguments are the x1/y1/x2/y2 bounding box of the current detection (with values between 0 and 1 for each)
NMS2::AddRect(x1, y1, x2, y2);

// Placed after NMS2::AddRect
// Arguments are the x/y point (to test if that point is part of a valid detected area)
if (NMS2::Query((x2 - x1) / 2 + x1, (y2 - y1) / 2 + y1) {
    // Assume detection bounding box is valid and perform actions
}
```

### How it works
Using `NMS2::Initialize()` creates a two-dimensional buffer that will contain values representing how likely any particular point is to be a valid detection.

`NMS2::Tick()` subtracts from all of the values in the buffer each frame, meaning that values add up over time and diminish over time.

> This type of algorithm works best in high-framerate sessions (more than 100 inferences per second) because `NMS2::Tick()` must deteriorate the buffer by a certain amount each frame.

`NMS2::AddRect(x1, y1, x2, y2)` adds a set of values to the buffer to represent a detection that *could* be valid. Notably, the values added to the buffer are minimal, meaning that a valid detection will only appear after multiple frames have been inferred.

`NMS2::Query(x, y)` checks a point in the buffer to see if it has had multiple detections added to it in the same general area. This algorithm works because most false positives are one-off detections, on only a few frames per second, in seemingly random areas of the buffer. When calling `AddRect`, a small value is added to that area of the buffer. If multiple frames have a detection in roughly the same area, then points inside that area will be returned as a valid detection by `Query`.

### Advanced configuration (from `NMS2.h`)
```C++
// The width of the detection buffer (should be the resolution of your input video divided by 2, 4, 8, etc.)
#define NMS2_DIVISOR 33

// The threshold for a value in the buffer to count as a valid detection
#define NMS_QUERY_THRESHOLD 0.5
```

The following options are used to control how fast a detection propogates and dissipates.
```C++
// The amount to add to the buffer for each presumed valid detection
#define NMS2_WEIGHT_POSITIVE 0.08f

// The amount to deteriorate the buffer every frame (should be increased if running at low framerates)
#define NMS2_WEIGHT_NEGATIVE 0.05f
```