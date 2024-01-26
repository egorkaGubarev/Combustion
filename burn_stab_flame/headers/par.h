#ifndef PARAMS_H
#define PARAMS_H

#include <string>

typedef double my_type;

const my_type N = 10;
const my_type width = 100;
const my_type m = 0.773;
const my_type sigma = 0.15;
const my_type lew = 1.5;

const my_type dim_step = my_type(1.0 / 15);
const my_type courant = 0.46;

const std::string temp_in_name = "C:/Users/gubar/VSProjects/burn_stab_flame/prep_front/temp.txt";
const std::string pos_in_name = "C:/Users/gubar/VSProjects/burn_stab_flame/prep_front/pos.txt";
const std::string par_in_name = "C:/Users/gubar/VSProjects/burn_stab_flame/prep_front/par.txt";

#endif