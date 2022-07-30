#include <iostream>
#include <random>
#include <vector>

#include <CL/sycl.hpp>

namespace {
  constexpr size_t n = 1024;
};

class optimized;

int main()
{
    std::vector<float> A, res(1);

    A.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 0.25f);

    float value = dis(gen);
    A.push_back(value);

    for(size_t i = 1; i < n; ++i) {
	    A.push_back(A[0] + static_cast<float>(i)/static_cast<float>(n));
    }

    // please complete

    sycl::queue q;
    
    {
      sycl::buffer A_b {A};
      sycl::buffer res_b {res};

      q.submit([&] (sycl::handler &h) {
        sycl::accessor A_a(A_b, h, sycl::read_only);
        sycl::accessor res_a(res_b, h, sycl::write_only);
        h.single_task<class optimized>([=]() {
          float mul = 1.0f;
          float mul_copies[5] = {1.0f,1.0f,1.0f};
          for (size_t i = 0; i < n; ++i) {
            auto cur  = mul_copies[4] * A_a[i];

            #pragma unroll
            for (size_t j = 4; j > 0; --j) {
              mul_copies[j] = mul_copies[j-1];
            }

            mul_copies[0] = cur;
          }

          #pragma unroll
          for (int i = 0; i < 5; ++i) {
            mul *= mul_copies[i];
          }
          
          res_a[0] = mul;
        });
      });
    }

    std::cout << "res[0] = " << res[0] << std::endl;
}
