#include <arrayfire.h>
#include <openacc.h>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace af;

array generate_signals(const int& N);
array max_sidelobes(const array& akfs);

int main(int argc, char* argv[]) {
    try {
        const int mn = 3;
        const int mx = 22;

        array signals, akfs, sidelobes, min_val, 
            min_idx, optimal_signal, optimal_akf;
        
        std::ofstream result("exec_time_libs_no_long.txt");

        for (int signal_length = mn; signal_length <= mx; ++signal_length) {
            std::cout << "Processing signal length: " << signal_length << std::endl;

            int best_sidelobe = INT16_MAX;

            const auto start{std::chrono::steady_clock::now()};
            signals = generate_signals(signal_length);

            akfs = fftConvolve1(signals, flip(signals, 0));

            sidelobes = max_sidelobes(akfs);
            min_val = min(sidelobes).as(s32);
            min_idx = where(sidelobes == tile(min_val, sidelobes.dims())).as(s32);

            optimal_signal = signals(span, min_idx(0).scalar<int>());
            optimal_akf = akfs(span, min_idx(0).scalar<int>());
            best_sidelobe = min_val.scalar<int>();

            af_print(optimal_akf);
            af_print(optimal_signal);
            std::cout << "Best sidelobe for length " << signal_length << ": " << best_sidelobe << std::endl;

            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> elapsed_seconds{end - start};
            std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n\n";

            result << signal_length << " " << elapsed_seconds.count() << std::endl;
        }
    } 
    catch (af::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }

    return 0;
}

array generate_signals(const int &N) {
    std::vector<int> flat_signals;

    std::vector<int> current_signal(N, -1);
    bool all_ones = false;

    #pragma acc data copy(flat_signals)
    {
        while (!all_ones) {
            for (int j = 0; j < N; ++j) {
                flat_signals.push_back(current_signal[j]);
            }

            int i = N - 1;
            while (i >= 0 && current_signal[i] == 1) {
                current_signal[i] = -1;
                --i;
            }
            if (i >= 0) {
                current_signal[i] = 1;
            }

            all_ones = (i < 0);
        }
    }

    return array(N, flat_signals.size() / N, flat_signals.data());
}

array max_sidelobes(const array& akfs) {
    int center = akfs.dims(0) / 2;

    array sidelobes = akfs(seq(0, center - 1), span);
    sidelobes = join(0, sidelobes, akfs(seq(center + 1, end), span));

    return max(abs(sidelobes), 0);
}