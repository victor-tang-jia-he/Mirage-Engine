#include "pch.h"
#include <quaternion.h>
using namespace MirageEngine::Core::Math;
// TEST TEMPLATEx
//TEST(/*Name*/, /*Name*/)
//{
//	/*TYPE*/ m1(/*....*/), m2(/*....*/);
//	auto start = std::chrono::high_resolution_clock::now();
//	for (int i = 0; i < 10000; ++i) {
//		/*Function need to be tested*/
//	}
//	auto end = std::chrono::high_resolution_clock::now();
//	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//	std::cout << duration.count() << std::endl;
//	EXPECT_LT(duration.count(), /*Time in 祍*/); // 100000 祍 = 100 ms
//}

// -------------------------------------------------------------------------------- //	
// All Comparator & Define value
// -------------------------------------------------------------------------------- //

inline static bool operator==(const Quaternion& _q1, const glm::quat& _q2)
{
	return std::abs(_q1.X - _q2.x) < EPSILON &&
		std::abs(_q1.Y - _q2.y) < EPSILON &&
		std::abs(_q1.Z - _q2.z) < EPSILON &&
		std::abs(_q1.W - _q2.w) < EPSILON;
}


inline static bool operator==(const glm::quat& _q1, const Quaternion& _q2)
{
	return _q2 == _q1;
}