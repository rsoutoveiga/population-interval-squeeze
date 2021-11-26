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

#include "cohort.h"

// ----------------------------------------------------------------------------

Cohort::Cohort(const SpeciesId species_id,
               const int age,
               const int num_individuals,
               const std::vector<double> &seedbank,
               const bool is_postfire)

  : species_id_(species_id)
  , age_(age)
  , num_individuals_(num_individuals)
  , seedbank_(seedbank)
  , is_postfire_(is_postfire)
{ }

// ----------------------------------------------------------------------------

void Cohort::PlantMort(const std::vector<double> &weather,
                       const int carrying_capacity,
                       const std::shared_ptr<RandomGenerator> &rng2)
{
  int living_plants = num_individuals_;
  int dead_plants = 0;

  if (age_ > Params::plant_longevity)
  {
    living_plants = 0;
  }
  else if (age_ >= 0)
  {
    double mort_probability = PlantMortAge();

    if (Params::is_climate == true)
    {
      mort_probability *= PlantMortWeatherCoeff(weather);
    }

    if (age_ == 0)
    {
      mort_probability = SeedlingEstablishment(mort_probability);
    }

    // Probability must be between 0 and 1
    if (mort_probability > 1.0)
    {
      mort_probability = 1.0;
    }
    else if (mort_probability < 0.0)
    {
        mort_probability = 0.0;
    }

    if (living_plants > carrying_capacity)
    {
      dead_plants = rng2->GetBinomialInt(living_plants, mort_probability);
    }
    else
    {
      double random_number = 0.0;

      for (int i = 0; i < num_individuals_; ++i)
      {
        random_number = rng2->Get01();

        if (random_number < mort_probability)
        {
          ++dead_plants;
        }
      }
    }

    living_plants -= dead_plants;

    num_individuals_ = living_plants;
  }
}

// ----------------------------------------------------------------------------

double Cohort::SeedlingEstablishment(double mort_probability)
{
    if (mort_probability > Params::recruit_postfire_max)
    {
      mort_probability = Params::recruit_postfire_max;
    }
    else if (mort_probability < Params::recruit_postfire_min)
    {
      mort_probability = Params::recruit_postfire_min;
    }

    if (is_postfire_ == false)
    {
      double survival_prob = 1.0 - mort_probability;
      survival_prob = survival_prob * (Params::recruit_interfire / 100.0);
      mort_probability = 1.0 - survival_prob;
    }

    return mort_probability;
}

//-----------------------------------------------------------------------------

void Cohort::AgingPlantsSeeds()
{
  // Aging of the cohort, i.e., Plants within the cohort are one year older
  ++age_;

  // Aging of seeds, i.e., rotate the elements of seedbank vector to the right
  std::rotate(seedbank_.begin(), seedbank_.end() - 1, seedbank_.end());

  // Set to zero the first element to be safe due to std::rotate()
  seedbank_[0] = 0.0;
}

// ----------------------------------------------------------------------------

void Cohort::DensityRegulationAdultPlants(const int living_adults,
                                          const int carrying_capacity)
{
  int empty_grids = carrying_capacity - living_adults;

  if (num_individuals_ > empty_grids)
  {
    num_individuals_ = empty_grids;
  }
}

// ----------------------------------------------------------------------------

double Cohort::PostfireSeedDispersal()
{
  return get_seedbank() * num_individuals_;
}

// ----------------------------------------------------------------------------

void Cohort::FireMort()
{
  num_individuals_ = 0;
}

// ----------------------------------------------------------------------------

int Cohort::InterfireSeedLossAndDispersal()
{
  double prop_damaged_seeds    = 0.0;
  double prop_decayed_seeds    = 0.0;
  double prop_open_follicles   = 0.0;
  double seeds_lost            = 0.0;
  double seeds_dispersed       = 0.0;
  double total_seeds_dispersed = 0.0;

  int seed_age = 0;

  auto iter_seedbank = std::begin(seedbank_);

  iter_seedbank = iter_seedbank + Params::cone_cycle;

  for (auto end = std::end(seedbank_); iter_seedbank != end; ++iter_seedbank)
  {
    ++seed_age;

    seeds_lost = 0.0;
    seeds_dispersed = 0.0;

    if (*iter_seedbank <= 0)
    {
      continue;
    }
    else if (seed_age > 1)
    {
      prop_damaged_seeds = DamagedSeeds(seed_age) -
                           DamagedSeeds(seed_age - 1);

      prop_decayed_seeds = DecayedSeeds(seed_age) -
                           DecayedSeeds(seed_age - 1);

      prop_open_follicles = OpenFollicles(seed_age) -
                            OpenFollicles(seed_age - 1);
    }
    else if (seed_age == 1)
    {
      prop_damaged_seeds = DamagedSeeds(seed_age);
      prop_decayed_seeds = DecayedSeeds(seed_age);
      prop_open_follicles = OpenFollicles(seed_age);
    }

    seeds_lost = (prop_damaged_seeds + prop_decayed_seeds) * (*iter_seedbank);

    if (seeds_lost > *iter_seedbank)
    {
      seeds_lost = *iter_seedbank;
      *iter_seedbank = 0.0;
    }
    else
    {
      *iter_seedbank -= seeds_lost;
      seeds_dispersed = prop_open_follicles * (*iter_seedbank);

      if (seeds_dispersed > *iter_seedbank)
      {
        seeds_dispersed = *iter_seedbank;
      }

      *iter_seedbank -= seeds_dispersed;
      total_seeds_dispersed += seeds_dispersed;
    }
  }

  return static_cast<int>(num_individuals_ * total_seeds_dispersed);
}

// ----------------------------------------------------------------------------

void Cohort::Germination(const double &seeds_dispersed)
{
  num_individuals_ = static_cast<int>(seeds_dispersed * Params::viable_seeds);
}

// ----------------------------------------------------------------------------

void Cohort::SeedProduction(const std::vector<double> &weather,
                            const double pollination_success)
{
  double new_seeds = 0.0;

  new_seeds = FlowerProductionAge();

  if (Params::is_climate == true)
  {
    new_seeds *= FlowerWeatherCoeff(weather);
  }

  new_seeds *= (pollination_success / 100.0); // Pollination success sub-submodel

  new_seeds = PotentialSeeds(new_seeds);

  new_seeds *= (Params::firm_seeds);        // Firm seeds sub-submodel

  seedbank_[0] = new_seeds;

  if (seedbank_[0] < 0.0) { seedbank_[0] = 0.0; }
}

// ----------------------------------------------------------------------------

double Cohort::FlowerProductionAge()
{
  return Params::flower_age_a /
         (1.0 + std::exp(Params::flower_age_c *
                       (Params::flower_age_b - static_cast<double>(age_))));
}

// ----------------------------------------------------------------------------

double Cohort::FlowerWeatherCoeff(const std::vector<double> &weather)
{
  double model_longterm = 1.0;
  double model_scenario = 1.0;

  model_longterm = Params::flower_weather_a * Params::long_term_rain +
                   Params::flower_weather_b;

  switch (Params::flower_scenario)
  {
    case FlowerScenario::kBaseline :
      model_scenario = Params::flower_weather_a * weather[0] +
                       Params::flower_weather_b;
      break;

    case FlowerScenario::kCurrent :
      model_scenario = Params::flower_weather_c * weather[1] +
                       Params::flower_weather_d * weather[2] +
                       Params::flower_weather_e;
      break;

    case FlowerScenario::kCurrentNB :
      model_scenario = std::exp(Params::flower_weather_c * weather[1] +
                                Params::flower_weather_d * weather[2] +
                                Params::flower_weather_e);
      break;
  }

  model_longterm = FlowerTruncation(model_longterm);
  model_scenario = FlowerTruncation(model_scenario);

  return model_scenario / model_longterm;
}

// ----------------------------------------------------------------------------

double Cohort::FlowerTruncation(double flower_climate_model)
{
  if (flower_climate_model < 0.0)
  {
    flower_climate_model = 0.0;
  }
  else if (flower_climate_model > Params::flower_max)
  {
    flower_climate_model = Params::flower_max;
  }

  return flower_climate_model;
}

//-----------------------------------------------------------------------------

double Cohort::PotentialSeeds(const double num_fertile_cones)
{
  return num_fertile_cones * Params::num_follicles * Params::num_seeds;
}

// ----------------------------------------------------------------------------

double Cohort::OpenFollicles(const int seed_age)
{ 
  return Params::open_follicles_a /
      (1.0 + std::exp(Params::open_follicles_c *
                    (Params::open_follicles_b - static_cast<double>(seed_age))));
}

// ----------------------------------------------------------------------------

double Cohort::DamagedSeeds(const int seed_age)
{
  return Params::insect_a * static_cast<double>(seed_age) + Params::insect_b;
}

// ----------------------------------------------------------------------------

double Cohort::DecayedSeeds(const int seed_age)
{
  return std::exp(Params::decay_a * static_cast<double>(seed_age) + Params::decay_b);
}

// ----------------------------------------------------------------------------

double Cohort::PlantMortAge()
{
  double prob_plant_mort_age = 0.0;

  if (age_ == 0)
  {
    prob_plant_mort_age = Params::recruit_postfire_mean;

  }
  else if (age_ > 0 && age_ <= 15)
  {
    prob_plant_mort_age = Params::plant_mort_age_a +
                         (Params::plant_mort_age_b / static_cast<double>(age_)) +
                         (Params::plant_mort_age_c / std::pow(age_, 2));
  }
  else if (age_ > 15 && age_ <= Params::plant_senescence_age)
  {
    prob_plant_mort_age = Params::plant_mort_age_a +
                         (Params::plant_mort_age_b / 15.0) +
                         (Params::plant_mort_age_c / std::pow(15, 2));
  }
  else if (age_ > Params::plant_senescence_age)
  {
    double min_prob_mort = Params::plant_mort_age_a +
                          (Params::plant_mort_age_b /
                           static_cast<double>(Params::plant_senescence_age)) +
                          (Params::plant_mort_age_c /
                           std::pow(Params::plant_senescence_age, 2));

    prob_plant_mort_age = min_prob_mort +
                         (Params::plant_senescence_increase *
                          (age_ - Params::plant_senescence_age));
  }

  return prob_plant_mort_age;
}

// ----------------------------------------------------------------------------

double Cohort::PlantMortWeatherCoeff(const std::vector<double> &weather)
{
  return 1 + (Params::plant_mort_weather_a * weather[0] +
              Params::plant_mort_weather_b);
}

// ----------------------------------------------------------------------------
