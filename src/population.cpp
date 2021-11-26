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

#include "population.h"

//-----------------------------------------------------------------------------

Population::Population(
    const int carrying_capacity,
    const int time_since_fire,
    const int fire_interval,
    const std::vector<std::shared_ptr<Cohort>> &cohort_list,
    const int num_generations,
    const int persistence_time,
    const bool is_extinct)

  : carrying_capacity_(carrying_capacity)
  , time_since_fire_(time_since_fire)
  , fire_interval_(fire_interval)
  , cohort_list_(cohort_list)
  , num_generations_(num_generations)
  , persistence_time_(persistence_time)
  , is_extinct_(is_extinct)

{ }

//-----------------------------------------------------------------------------

void Population::AddEmptyCohort(const bool is_postfire_cohort)
{
  std::vector<double> init_empty_seedbank;

  init_empty_seedbank.resize(
        static_cast<unsigned>(Params::cone_cycle) +
        static_cast<unsigned>(Params::seed_longevity),
        0.0);

  auto cohort = std::make_shared<Cohort>(
                  Params::species_id,
                  0,
                  0,
                  init_empty_seedbank,
                  is_postfire_cohort);

  cohort_list_.emplace_back(std::move(cohort));
}

//-----------------------------------------------------------------------------

void Population::RemoveAllCohorts()
{
  cohort_list_.erase(
        std::remove_if(
          cohort_list_.begin(),
          cohort_list_.end(),
          [](const std::shared_ptr<Cohort>) { return true; }),
      cohort_list_.end());
}

//-----------------------------------------------------------------------------

void Population::RemoveEmptyCohorts()
{
  cohort_list_.erase(
        std::remove_if(
          cohort_list_.begin(),
          cohort_list_.end(),
          [](const std::shared_ptr<Cohort> &cohort)
          {
            return cohort->get_num_individuals() <= 0;
          }),
      cohort_list_.end());
}
