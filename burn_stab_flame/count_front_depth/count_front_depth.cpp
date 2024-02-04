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

    const std::string depth_log = "depth.txt";
    const std::string par = "par.txt";

    const std::string temp_in_name = "C:/Users/gubar/VSProjects/burn_stab_flame/eval_no_log/temp-4000-0.781.txt";
    const std::string pos_in_name = "C:/Users/gubar/VSProjects/burn_stab_flame/eval_no_log/pos-4000-0.781.txt";
    const std::string par_in_name = "C:/Users/gubar/VSProjects/burn_stab_flame/eval_no_log/par-4000-0.781.txt";

    const my_type time_step = courant * std::pow(dim_step, 2);
    const size_t iters = size_t(time_simul / time_step);
    const size_t iter_log = iters / frames;
    const size_t points = size_t(width / dim_step);
    const size_t iter_progr = iters / 100 * verb;
    const my_type x_stab = x_flame_stab(m, sigma, N);
    const my_type temp_stab = temp_flame_stab(N, m);
    std::ofstream out_depth, out_par;

    std::vector<my_type> pos(points, 0);
    std::vector<my_type> temp(points, 0);

    std::array<std::vector<my_type>, 2> data = read_profile<my_type>(temp_in_name, pos_in_name, par_in_name);

    std::vector<my_type>& pos_last = data[0];
    std::vector<my_type>& temp_last = data[1];

    out_depth.open(depth_log);
    out_par.open(par);

    out_par << time_simul;
    out_par.close();

    my_type curr_time = 0;
    size_t curr_iter = 0;

    while (curr_time < time_simul) {
        ++curr_iter;
        curr_time += time_step;

        if (curr_iter % iter_log == 0) {
            const my_type depth = count_depth(pos);
            out_depth << depth << ' ';
        }

        evaluate_zero_der(pos_last, temp_last, pos, temp, m, N, sigma, lew, time_step, dim_step);

        copy(std::begin(pos), std::end(pos), std::begin(pos_last));
        copy(std::begin(temp), std::end(temp), std::begin(temp_last));

        if (curr_iter % iter_progr == 0) {
            const my_type progr = my_type(curr_iter) / iters * 100;
            std::cout << progr << " %" << '\n';
        }
    }

    out_depth.close();
    return 0;
}
