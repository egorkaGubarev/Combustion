#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>
#include <vector>

#include "ut.h"

template <typename type>
type ad_vel(const type temp, const type N) {
    const type vel = std::exp(N / 2 * (1 - 1.0 / temp));
    return vel;
}

template <typename type>
type temp_flame_stab(const type N, const type m) {
    const type res = N / (N - 2 * std::log(m));
    return res;
}

template <typename type>
type x_flame_stab(const type m, const type sigma, const type N) {
    const type temp_stab = temp_flame_stab(N, m);
    const type res = std::log((1 - sigma) / (1 - temp_stab)) / m;
    return res;
}

template <typename type>
type khi(const type m, const type pos) {
    const type arg = m * pos / 2;
    const type res = cth(arg) - arg / std::pow(sh(arg), 2);
    return res;
}

template <typename type>
type phi(const type pos, const type temp, const type m, const type sigma, const type N) {
    const type vel = ad_vel(temp, N);
    const type res = (1 - sigma) * m * vel - std::pow(m, 2) / 2 * (1 + cth(m * pos / 2)) * (temp - sigma);
    return res;
}

template <typename type>
type psi_1(const type pos, const type temp, const type m, const type lew, const type N) {
    const type vel = ad_vel(temp, N);
    const type res = (m - vel) / (1 - std::exp(-m * lew * pos));
    return res;
}

template <typename type>
type psi_2(const type pos, const type temp, const type m, const type lew, const type N, const type sigma) {
    const type phi_val = phi(pos, temp, m, sigma, N);
    const type khi_val = khi(m, pos);
    const type psi_1_val = psi_1(pos, temp, m, lew, N);
    const type res = (phi_val + m * (1 - sigma) * psi_1_val * khi_val) / (1 + khi_val);
    return res;
}

template <typename type>
void evaluate_per(const std::vector<type>& pos_last, const std::vector<type>& temp_last, std::vector<type>& pos, std::vector<type>& temp,
    const type m, const type N, const type sigma, const type lew, const type time_step, const type dim_step) {

    const size_t points = std::size(temp_last);

    for (size_t point = 0; point < points; ++point) {
        const type temp_cent = temp_last[point];
        const type temp_left = temp_last[point - 1];
        const type temp_right = temp_last[point + 1];

        const type pos_cent = pos_last[point];
        const type pos_left = pos_last[point - 1];
        const type pos_right = pos_last[point + 1];

        const type khi_val = khi(m, pos_cent);
        const type a = m / (2 * (1 - std::exp(-m * lew * pos_cent))) - m;
        const type b = m * (1 - sigma) * (1 - lew) * khi_val / (lew * (khi_val + 1));

        const type c = (1 - sigma) * m * (a * khi_val - m) / (khi_val + 1) +
            std::pow((1 - sigma) * m, 2) * khi_val / ((khi_val + 1) * (temp_cent - sigma));

        const type d = -(1 - sigma) * m * khi_val / ((khi_val + 1) * (temp_cent - sigma));

        type der_2_temp = 0;
        type der_1_temp = 0;
        type der_2_pos = 0;
        type der_1_pos = 0;

        if (point == 0) {
            der_2_temp = type((temp_right - 2 * temp_cent + temp_last[points - 1]) / std::pow(dim_step, 2));
            der_1_temp = type((temp_right - temp_last[points - 1]) / (2 * dim_step));
            der_2_pos = type((pos_right - 2 * pos_cent + pos_last[points - 1]) / std::pow(dim_step, 2));
            der_1_pos = type((pos_right - pos_last[points - 1]) / (2 * dim_step));
        }
        else {
            if (point == points - 1) {
                der_2_temp = type((temp_last[0] - 2 * temp_cent + temp_left) / std::pow(dim_step, 2));
                der_1_temp = type((temp_last[0] - temp_left) / (2 * dim_step));
                der_2_pos = type((pos_last[0] - 2 * pos_cent + pos_left) / std::pow(dim_step, 2));
                der_1_pos = type((pos_last[0] - pos_left) / (2 * dim_step));
            }
            else {
                der_2_temp = type((temp_right - 2 * temp_cent + temp_left) / std::pow(dim_step, 2));
                der_1_temp = type((temp_right - temp_left) / (2 * dim_step));
                der_2_pos = type((pos_right - 2 * pos_cent + pos_left) / std::pow(dim_step, 2));
                der_1_pos = type((pos_right - pos_left) / (2 * dim_step));
            }
        }

        const type temp_new = type(temp_cent + time_step * (der_2_temp + b * der_2_pos + c * std::pow(der_1_pos, 2) +
            d * der_1_pos * der_1_temp + psi_2(pos_cent, temp_cent, m, lew, N, sigma)));

        const type pos_new = type(pos_cent + time_step * (der_2_pos / lew + a * std::pow(der_1_pos, 2) +
            psi_1(pos_cent, temp_cent, m, lew, N)));

        temp[point] = temp_new;
        pos[point] = pos_new;
    }
}

template <typename type>
void evaluate_zero_der(const std::vector<type>& pos_last, const std::vector<type>& temp_last, std::vector<type>& pos, std::vector<type>& temp,
    const type m, const type N, const type sigma, const type lew, const type time_step, const type dim_step) {

    const size_t points = std::size(temp_last);

    const size_t left_ev_bord = 1;
    const size_t right_ev_bord = points - 1;

    for (size_t point = left_ev_bord; point < right_ev_bord; ++point) {
        const type temp_cent = temp_last[point];
        const type temp_left = temp_last[point - 1];
        const type temp_right = temp_last[point + 1];

        const type pos_cent = pos_last[point];
        const type pos_left = pos_last[point - 1];
        const type pos_right = pos_last[point + 1];

        const type khi_val = khi(m, pos_cent);
        const type a = m / (2 * (1 - std::exp(-m * lew * pos_cent))) - m;
        const type b = m * (1 - sigma) * (1 - lew) * khi_val / (lew * (khi_val + 1));

        const type c = (1 - sigma) * m * (a * khi_val - m) / (khi_val + 1) +
            std::pow((1 - sigma) * m, 2) * khi_val / ((khi_val + 1) * (temp_cent - sigma));

        const type d = -(1 - sigma) * m * khi_val / ((khi_val + 1) * (temp_cent - sigma));

        type der_2_temp = 0;
        type der_1_temp = 0;
        type der_2_pos = 0;
        type der_1_pos = 0;

        der_2_temp = type((temp_right - 2 * temp_cent + temp_left) / std::pow(dim_step, 2));
        der_1_temp = type((temp_right - temp_left) / (2 * dim_step));
        der_2_pos = type((pos_right - 2 * pos_cent + pos_left) / std::pow(dim_step, 2));
        der_1_pos = type((pos_right - pos_left) / (2 * dim_step));

        const type temp_new = type(temp_cent + time_step * (der_2_temp + b * der_2_pos + c * std::pow(der_1_pos, 2) +
            d * der_1_pos * der_1_temp + psi_2(pos_cent, temp_cent, m, lew, N, sigma)));

        const type pos_new = type(pos_cent + time_step * (der_2_pos / lew + a * std::pow(der_1_pos, 2) +
            psi_1(pos_cent, temp_cent, m, lew, N)));

        temp[point] = temp_new;
        pos[point] = pos_new;
    }

    temp[0] = (4 * temp[1] - temp[2]) / 3;
    pos[0] = (4 * pos[1] - pos[2]) / 3;
    temp[points - 1] = (4 * temp[points - 2] - temp[points - 3]) / 3;
    pos[points - 1] = (4 * pos[points - 2] - pos[points - 3]) / 3;
}

#endif