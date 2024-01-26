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
    const size_t verb = 1;
    const my_type pos_start_marg = -1;
    const my_type temp_start_marg = -0.1;
    const my_type freq = 0.5;
    const my_type ampl = 1;
    const my_type time_simul = 4000;

    const std::string pos_log = "pos.txt";
    const std::string temp_log = "temp.txt";
    const std::string par = "par.txt";

    const my_type time_step = courant * std::pow(dim_step, 2);
    const size_t iters = size_t(time_simul / time_step);
    const size_t points = size_t(width / dim_step);
    const size_t iter_progr = iters / 100 * verb;
    std::ofstream out_pos, out_par, out_temp;

    const my_type temp_stab = temp_flame_stab(N, m);
    const my_type pos_stab = x_flame_stab(m, sigma, N);

    const my_type pos_start = pos_stab + pos_start_marg;
    const my_type temp_start = temp_stab + temp_start_marg;

    std::vector<my_type> pos_last(points, 0);
    std::vector<my_type> temp_last(points, temp_start);

    fill_sin(pos_last, pos_start, ampl, freq, dim_step);

    std::vector<my_type> pos(points, 0);
    std::vector<my_type> temp(points, 0);

    out_par.open(par);
    out_pos.open(pos_log);
    out_temp.open(temp_log);

    out_par  << width << ' ' << points;
    out_par.close();

    my_type curr_time = 0;
    size_t curr_iter = 0;

    while (curr_time < time_simul) {
        ++curr_iter;
        curr_time += time_step;

        evaluate_zero_der(pos_last, temp_last, pos, temp, m, N, sigma, lew, time_step, dim_step);

        copy(std::begin(pos), std::end(pos), std::begin(pos_last));
        copy(std::begin(temp), std::end(temp), std::begin(temp_last));

        if (curr_iter % iter_progr == 0) {
            const my_type progr = my_type(curr_iter) / iters * 100;
            std::cout << progr << " %" << '\n';
        }
    }

    dump<my_type>(out_pos, pos_last);
    dump<my_type>(out_temp, temp_last);

    out_pos.close();
    out_temp.close();

    return 0;
}