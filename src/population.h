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
 * @file population.h
 *
 * @brief Each population entity can hold several cohorts
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>
#include <memory>
#include <utility>

#include "cohort.h"


class Population
{
 private:
  int carrying_capacity_;
  int time_since_fire_;
  int fire_interval_;
  std::vector<std::shared_ptr<Cohort>> cohort_list_;
  int num_generations_;
  int persistence_time_;
  bool is_extinct_;

 public:
  Population(const int carrying_capacity,
             const int time_since_fire,
             const int fire_interval,
             const std::vector<std::shared_ptr<Cohort>> &cohort_list,
             const int num_generations,
             const int persistence_time,
             const bool is_extinct);

  void RemoveAllCohorts();
  void RemoveEmptyCohorts();
  void AddEmptyCohort(const bool is_postfire_cohort);

  void IncrementTimeSinceFire() { ++time_since_fire_; }
  void IncrementNumGenerations() { ++num_generations_; }
  void IncrementPersistenceTime() { ++persistence_time_; }

  // setters and getters
  void set_time_since_fire(const int time_since_fire) {
    time_since_fire_ = time_since_fire;
  }

  void set_fire_interval(const int fire_interval) {
    fire_interval_ = fire_interval;
  }

  void set_is_extinct(const bool is_extinct) {
    is_extinct_ = is_extinct;
  }

  int get_carrying_capacity() const { return carrying_capacity_; }
  int get_time_since_fire() const { return time_since_fire_; }
  int get_fire_interval() const { return fire_interval_; }

  std::vector<std::shared_ptr<Cohort>> get_cohorts() const
  {
    return cohort_list_;
  }

  int get_num_generations() const { return num_generations_; }
  int get_persistence_time() const { return persistence_time_; }
  bool get_is_extinct() const { return is_extinct_; }
};

#endif // POPULATION_H
