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

/**
 * @file parameters.h
 *
 * @brief Static parameters for each simulation run
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <string>


enum class FlowerScenario { kBaseline, kCurrent, kCurrentNB };

enum class SpeciesId { kBanksiaHookeriana };

enum class FireScenario { kDeterministic, kStochastic };

struct Params
{
  static std::string name_sim_file;  // Filename of Simulation-File
  static std::string folder_climate;
  static std::string output_prefix;  // Prefix for the output file

  static std::string sim_id;

  static bool is_seed_dynamics;  // simulate seed dynamics
  static bool is_persistence;    // simulate persistence

  static int run_time;

  static bool is_climate;
  static double long_term_rain;
  static std::string climate_scenario;

  static bool is_fire;  // true = fire occurs, false = fire does not occur
  static FireScenario fire_scenario;  // 0 = deterministic, 1 = stochastic
  static int fire_interval_min;
  static int fire_interval_max;
  static int fire_interval_step;
  static int fire_lower_limit;  // lower bound for stochastic fires

  static SpeciesId species_id;  // 0 = Banksia hookeriana
  static FlowerScenario flower_scenario;  // 0 = baseline, 1 = current_lm, 2 = current_nb

  // Population Initialization
  static int carrying_capacity;

  // Cohort Initialization
  static int init_individuals;
  static double init_seedbank;

  static int plant_longevity;
  static int plant_age_maturity;

  static double recruit_postfire_min;
  static double recruit_postfire_max;
  static double recruit_postfire_mean;

  static int plant_senescence_age;
  static double plant_senescence_increase;


  static double recruit_interfire;     // percentage of post-fire recruitment

  static double plant_mort_age_a;
  static double plant_mort_age_b;
  static double plant_mort_age_c;

  static double plant_mort_weather_a;
  static double plant_mort_weather_b;

  static int cone_cycle;
  static int seed_longevity;

  static double flower_max;

  static double flower_age_a;
  static double flower_age_b;
  static double flower_age_c;

  static double flower_weather_a;
  static double flower_weather_b;
  static double flower_weather_c;
  static double flower_weather_d;
  static double flower_weather_e;

  static double pollination_success_min;
  static double pollination_success_max;
  static double pollination_success_step;

  static double num_follicles;
  static double num_seeds;

  static double firm_seeds;
  static double viable_seeds;

  static double insect_a;
  static double insect_b;

  static double decay_a;
  static double decay_b;

  static double open_follicles_a;
  static double open_follicles_b;
  static double open_follicles_c;
};

#endif // PARAMETERS_H
