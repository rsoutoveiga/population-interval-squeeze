/*
 *-----------------------------------------------------------------------------
 * This file is part of Interval Squeeze model.
 *
 * Interval Squeeze model is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Interval Squeez model is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Interval Squeeze model.  If not, see
 * <https://www.gnu.org/licenses/>.
 *-----------------------------------------------------------------------------
 */

#include "parameters.h"

// ----------------------------------------------------------------------------

std::string Params::name_sim_file = "data/in/proba.txt";
std::string Params::folder_climate = "data/in/climate_scn/";
std::string Params::output_prefix = "default";  // Prefix for the output file

std::string Params::sim_id = "0";

bool Params::is_seed_dynamics = true;
bool Params::is_persistence = true;

int Params::run_time = 100;

bool Params::is_climate = true;
double Params::long_term_rain = 453.848;
std::string Params::climate_scenario = "baseline";

bool Params::is_fire = false;
FireScenario Params::fire_scenario = FireScenario::kDeterministic;
int Params::fire_interval_min = 17;
int Params::fire_interval_max = 17;
int Params::fire_interval_step = 0;
int Params::fire_lower_limit = 12;

SpeciesId Params::species_id = SpeciesId::kBanksiaHookeriana;
FlowerScenario Params::flower_scenario = FlowerScenario::kBaseline;

// Population Initialization
int Params::carrying_capacity = 2500;

// Cohort Initialization
int Params::init_individuals = 863;
double Params::init_seedbank = 630;

int Params::plant_longevity = 999;
int Params::plant_age_maturity = 5;

double Params::recruit_postfire_min = 0.95;
double Params::recruit_postfire_max = 0.987;
double Params::recruit_postfire_mean = 0.969;

int Params::plant_senescence_age = 25;
double Params::plant_senescence_increase = 0.01;

double Params::recruit_interfire = 5;  // percentage of post-fire recruitment

double Params::plant_mort_age_a = 0.0553;
double Params::plant_mort_age_b = 0.2645;
double Params::plant_mort_age_c = 0.179;

double Params::plant_mort_weather_a = -0.00061;
double Params::plant_mort_weather_b = 0.26873;

int Params::cone_cycle = 1;
int Params::seed_longevity = 12;

double Params::flower_max = 16.5;

double Params::flower_age_a = 9.17976;
double Params::flower_age_b = 8.71027;
double Params::flower_age_c = 0.62059;

double Params::flower_weather_a = 0.053176;
double Params::flower_weather_b = -15.212370;
double Params::flower_weather_c = 0.021990;
double Params::flower_weather_d = 0.023144;
double Params::flower_weather_e = -33.383165;

// baseline = 0.90; current = 0.65
double Params::pollination_success_min = 90.0;
double Params::pollination_success_max = 90.0;
double Params::pollination_success_step = 0.0;

double Params::num_follicles = 9.97;  // baseline = 9.97; current = 7.32
double Params::num_seeds = 2;
double Params::firm_seeds = 0.83;
double Params::viable_seeds = 0.744;

double Params::insect_a = 0.02;
double Params::insect_b = 0.18;

double Params::decay_a = 0.34;
double Params::decay_b = -5.95;

double Params::open_follicles_a = 0.3896;
double Params::open_follicles_b = 6.6873;
double Params::open_follicles_c = 0.9588;
