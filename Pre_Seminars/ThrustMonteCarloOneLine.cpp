const int N = 1000000; // Number of Monte-Carlo simulations.

struct random_point {
private:
	thrust::default_random_engine rng;
public:
	__device__ __host__
	float2 operator()(int index) {
		rng.discard(2*index);
		return make_float2(
			(float)rng() / thrust::default_random_engine::max,
			(float)rng() / thrust::default_random_engine::max);
	}
};

struct inside_circle {
private:
	__device__ __host__
	unsigned int inside(float2 p) const {
		return (((p.x-0.5)*(p.x-0.5)+(p.y-0.5)*(p.y-0.5))<0.25) ? 1 : 0;
	}
public:
	// Used for-on-the fly.
	__device__ __host__
	unsigned int operator()(int index) const {
		// Generate a random point.
		random_point point;
		return inside(point(index));
	}
};

int main()
{
// DEVICE:
thrust::counting_iterator<int> index(0);
size_t total = thrust::count_if(index, index+N, inside_circle());

// HOST: Print estimate of PI.
std::cout << "PI: " << 4.0*(float)total/(float)N << std::endl;

return 0;
}