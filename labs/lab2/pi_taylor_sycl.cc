#include <CL/sycl.hpp>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <cmath>
#include <numeric>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using my_float = float;
using tid_time = std::pair<std::thread::id, double>;

template<typename F, typename... Args>
tid_time
time_ms(F f, Args&&... args)
{
    auto start = std::chrono::steady_clock::now();
    f(std::forward<Args>(args)...);
    auto stop = std::chrono::steady_clock::now();

    auto tid = std::this_thread::get_id();
    double ex_time = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count();
    return std::make_pair(tid, ex_time);
}

int
main(int argc, const char *argv[])
{
    // read the number of steps from the command line
    if (argc != 2) {
        std::cerr << "Invalid syntax: pi_taylor <steps>" << std::endl;
        exit(1);
    }

    auto steps = std::stoll(argv[1]);


    sycl::default_selector device_selector;
    sycl::queue q(device_selector);

    auto work_group_size = q.get_device().get_info<cl::sycl::info::device::max_work_group_size>();

    if (steps < work_group_size ) {
        std::cerr << "The number of steps should be larger than " << work_group_size << std::endl;
        exit(1);

    }

    // please complete
    my_float pi = 0.0;
    std::vector<my_float> v_pi;
    v_pi.reserve(steps);

    for (int i = 1; i < steps; i++) {
 		v_pi.push_back(0);
	}

    {
		sycl::buffer bufpi_b {v_pi};
		q.submit([&] (sycl::handler &h) {
			auto v_pi = bufpi_b.get_access(h, sycl::write_only);
            h.parallel_for(steps, [=](auto i){
                int sign = i & 0x1 ? -1 : 1;
				v_pi[i] =sign/static_cast<my_float>(2*i+1);
			});
		});
	}
    for (int i = 0; i < v_pi.size(); i++) pi += v_pi[i];
    pi *= 4;


    std::cout << "For " << steps << " steps, pi value: "
        << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
        << pi << std::endl;
}

