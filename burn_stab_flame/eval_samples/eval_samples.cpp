#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

#include "../headers/funct.h"
#include "../headers/par.h"
#include "../headers/ut.h"

typedef double my_type;

int main()
{
    const size_t frames = 1000;
    const size_t verb = 10;
    const my_type time_simul = 1000;
    const size_t precis_name = 2;

    const std::string path_out = "C:/Users/gubar/source/repos/burn_stab_flame/eval/";
    const std::string path_in = "C:/Users/gubar/source/repos/burn_stab_flame/eval_no_log/";

    const std::string temp_in_name = path_in + "temp.txt";
    const std::string pos_in_name = path_in + "pos.txt";
    const std::string par_in_name = path_in + "par.txt";

    const std::vector<my_type> m_s{ 0.78, 0.79 };

    const size_t suffix_length = 2 + precis_name;
    const my_type time_step = courant * std::pow(dim_step, 2);
    const size_t iters = size_t(time_simul / time_step);
    const size_t iter_log = iters / frames;
    const size_t points = size_t(width / dim_step);
    const size_t iter_progr = iters / 100 * verb;
    const my_type x_stab = x_flame_stab(m, sigma, N);
    const my_type temp_stab = temp_flame_stab(N, m);
    std::ofstream out_pos, out_par, out_temp;

    std::vector<my_type> pos(points, 0);
    std::vector<my_type> temp(points, 0);

    for (const my_type m : m_s) {

        std::cout << "m: " << m << '\n';
        const std::string suffix = std::to_string(m).substr(0, suffix_length);

        const std::string pos_log = path_out + "pos-0-" + suffix + ".txt";
        const std::string par = path_out + "par-0-" + suffix + ".txt";
        const std::string temp_log = path_out + "temp-0-" + suffix + ".txt";

        std::array<std::vector<my_type>, 2> data = read_profile<my_type>(temp_in_name, pos_in_name, par_in_name);

        std::vector<my_type>& pos_last = data[0];
        std::vector<my_type>& temp_last = data[1];

        out_par.open(par);
        out_pos.open(pos_log);
        out_temp.open(temp_log);

        out_par << width << ' ' << time_simul << ' ' << iter_log << ' ' << time_step
            << ' ' << dim_step << ' ' << points << ' ' << x_stab << ' ' << temp_stab;

        out_par.close();

        my_type curr_time = 0;
        size_t curr_iter = 0;

        while (curr_time < time_simul) {
            ++curr_iter;
            curr_time += time_step;

            if (curr_iter % iter_log == 0) {
                dump<my_type>(out_pos, pos_last);
                dump<my_type>(out_temp, temp_last);
            }

            evaluate_zero_der(pos_last, temp_last, pos, temp, m, N, sigma, lew, time_step, dim_step);

            copy(std::begin(pos), std::end(pos), std::begin(pos_last));
            copy(std::begin(temp), std::end(temp), std::begin(temp_last));

            if (curr_iter % iter_progr == 0) {
                const my_type progr = my_type(curr_iter) / iters * 100;
                std::cout << progr << " %" << '\n';
            }
        }

        out_pos.close();
        out_temp.close();
    }

    return 0;
}
