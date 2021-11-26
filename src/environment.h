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
 * @file environment.h
 *
 * @brief Class where each simulation run occurs
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "climate_reader.h"
#include "weather_simulator.h"
#include "fire_simulator.h"
#include "population.h"
#include "cohort.h"
#include "random_generator.h"
#include "output.h"


class Environment
{
 private:
  static int year_grid_;                       // current year
  static int sim_id_;                          // simulation ID
  static int run_num_;                         // run number i.e. replication
  static std::vector<double> weather_grid_;    // annual weather conditions
  static int run_fire_interval_mean_;          // mean fire interval in the run sim
  static double run_pollination_success_mean_; // pollination success in the run sim

  static std::vector<std::shared_ptr<Population>> population_list_;

  std::shared_ptr<RandomGenerator> prng2_;
  ClimateReader climate_grid_;    // climate in the entire study area (grid)
  FireSimulator fire_sim_;
  WeatherSimulator weather_sim_;
  std::unique_ptr<Output> unique_output_;

  void GetClimate();

  void InitPopulations();

  void OneYear();

  void SetNextLocalFire(const std::shared_ptr<Population> &population);

  bool IsFireEvent(const std::shared_ptr<Population> &population);

  void FireEventYes(const std::shared_ptr<Population> &population);
  void FireEventNo(const std::shared_ptr<Population> &population);

  void RemoveAllPopulations()
  {
    population_list_.erase(
          std::remove_if(
            population_list_.begin(),
            population_list_.end(),
            [](std::shared_ptr<Population>) { return true; }),
        population_list_.end());
  }

 public:
  Environment();
  ~Environment();

  void GetSim(const std::string &data);  // simulation read in

  void InitRun();
  void OneRun();

  // setters
  void set_run_num(const int run_num) {
    run_num_ = run_num;
  }

  void set_prng2(const std::shared_ptr<RandomGenerator> &prngenerator) {
    prng2_ = prngenerator;
  }

  void set_run_fire_interval(const int fire_interval_mean) {
    run_fire_interval_mean_ = fire_interval_mean;
  }

  void set_run_pollination_success(const double pollination_success_mean) {
    run_pollination_success_mean_ = pollination_success_mean;
  }
};

#endif  // ENVIRONMENT_H
