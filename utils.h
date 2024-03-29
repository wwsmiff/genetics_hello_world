#include <array>
#include <random>

std::random_device device;
std::mt19937 generator(device());

namespace utils
{
	inline int32_t generate_random(int32_t start, int32_t stop)
	{
		std::uniform_int_distribution<std::mt19937::result_type> distribution(start, stop);

		return distribution(generator);
	}
};
