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
 * @file cohort.h
 *
 * @brief Group of plants of the same species and age
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#ifndef COHORT_H
#define COHORT_H


#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <numeric>
#include <algorithm>
#include <functional>
#include <memory>
#include <cassert>

#include "parameters.h"
#include "random_generator.h"

class Cohort
{

private:

  SpeciesId species_id_; // THIS IS UNUSED PARAMETER!
  int age_;
  int num_individuals_;
  std::vector<double> seedbank_;
  bool is_postfire_;

  // Truncation of the flower climate equations in FlowerWeatherCoeff()
  double FlowerTruncation(double flower_climate_model);

  // # Sub-submodels
  // ## Seed production sub-submodels
  // Note: the sub-submodels firm seeds and viable seeds are just proportions
  // thus, they are placed directly in the SeedProduction() submodel
  double FlowerProductionAge();
  double FlowerWeatherCoeff(const std::vector<double> &weather);
  double PotentialSeeds(const double num_fertile_cones);

  // ## Inter-fire seed loss and seed dispersal sub-submodels
  double DamagedSeeds(const int seed_age);
  double DecayedSeeds(const int seed_age);
  double OpenFollicles(const int seed_age);

  // ## Plant mortality sub-submodels
  double PlantMortAge();
  double PlantMortWeatherCoeff(const std::vector<double> &weather);
  double SeedlingEstablishment(double mort_probability);
  double TruncateMortality(double mortality);


public:

  Cohort(const SpeciesId species_id,
         const int age,
         const int num_individuals,
         const std::vector<double> &seedbank,
         const bool is_postfire);

  void PlantMort(const std::vector<double> &weather,
                 const int carrying_capacity,
                 const std::shared_ptr<RandomGenerator> &rng2);

  void AgingPlantsSeeds();

  void DensityRegulationAdultPlants(const int living_adults,
                                    const int carrying_capacity);

  int InterfireSeedLossAndDispersal();

  void Germination(const double &seeds_dispersed);

  void SeedProduction(const std::vector<double> &weather,
                      const double pollination_success);

  double PostfireSeedDispersal();

  void FireMort();  // B. hookeriana is a fire-killed species

  // getters
  int get_age() const { return age_; }
  int get_num_individuals() const { return num_individuals_; }
  bool get_is_postfire() const { return is_postfire_; }

  double get_seedbank() const
  {
    return std::accumulate(seedbank_.begin() + Params::cone_cycle,
                           seedbank_.end(),
                           0.0,
                           std::plus<double>());
  }
};

#endif // COHORT_H
