#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <fstream>
#include <vector>

template <typename type>
type ch(const type x) {
	const type res = (std::exp(x) + std::exp(-x)) / 2;
	return res;
}

template <typename type>
type sh(const type x) {
	const type res = (std::exp(x) - std::exp(-x)) / 2;
	return res;
}

template <typename type>
type cth(const type x) {
	const type res = ch(x) / sh(x);
	return res;
}

template <typename type>
void dump(std::ofstream& out, const std::vector<type>& data) {

    for (const type value : data) {
        out << value << ' ';
    }

    out << '\n';
}

template <typename type>
void fill_sin(std::vector<type>& target, const type base, const type ampl, const type freq, const type dim_step) {
    const size_t points = std::size(target);
    for (size_t i = 0; i < points; ++i) {
        target[i] = base + ampl * std::sin(2 * 3.14 * type(i) / points * freq);
    }
}

template <typename type>
std::array<std::vector<type>, 2> read_profile(const std::string& temp_name, const std::string& pos_name,
    const std::string& par_name) {
    type width = 0;
    size_t points = 0;
    std::ifstream temp_in, pos_in, par_in;

    temp_in.open(temp_name);
    pos_in.open(pos_name);
    par_in.open(par_name);

    par_in >> width >> points;

    std::vector<type> temp(points, 0);
    std::vector<type> pos(points, 0);

    type temp_value = 0, pos_value = 0;

    for (size_t point = 0; point < points; ++point) {
        temp_in >> temp_value;
        pos_in >> pos_value;

        temp[point] = temp_value;
        pos[point] = pos_value;
    }

    std::array<std::vector<type>, 2> profiles{ pos, temp };

    temp_in.close();
    pos_in.close();
    par_in.close();

    return profiles;
}

#endif