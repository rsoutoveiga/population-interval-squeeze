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

#include "environment.h"

//-----------------------------------------------------------------------------

int Environment::year_grid_;
int Environment::sim_id_;
int Environment::run_num_;
std::vector<double> Environment::weather_grid_;

int Environment::run_fire_interval_mean_;
double Environment::run_pollination_success_mean_;

std::vector<std::shared_ptr<Population>> Environment::population_list_;

//-----------------------------------------------------------------------------

Environment::Environment()
{
  year_grid_ = 0;
  sim_id_ = 0;
  run_num_ = 0;
}

//-----------------------------------------------------------------------------

Environment::~Environment()
{
  // clear stuff
  for (auto const &population : population_list_)
  {
    population->RemoveAllCohorts();
  }

  RemoveAllPopulations();
}

//-----------------------------------------------------------------------------

void Environment::InitRun()
{
  for (auto const &population : population_list_)
  {
    population->RemoveAllCohorts();
  }

  RemoveAllPopulations();

  InitPopulations();

  weather_grid_ = weather_sim_.GetYearRandom(climate_grid_, prng2_);
}

//-----------------------------------------------------------------------------

void Environment::InitPopulations()
{
  std::vector<double> init_empty_seedbank;

  init_empty_seedbank.resize(
        static_cast<unsigned>(Params::cone_cycle) +
        static_cast<unsigned>(Params::seed_longevity),
        0.0);

  // storing the initial seed in the first year of the seed to simplify
  init_empty_seedbank[static_cast<unsigned>(Params::cone_cycle)] =
      Params::init_seedbank;

  auto init_cohort = std::make_shared<Cohort>(
                  Params::species_id,
                  17,
                  Params::init_individuals,
                  init_empty_seedbank,
                  true);

  std::vector<std::shared_ptr<Cohort>> init_cohort_list;

  init_cohort_list.emplace_back(std::move(init_cohort));

  // init population. time since fire and fire interval are not important
  // because all study area (i.e. all populations) burn in the first year sim
  auto population = std::make_shared<Population>(
                      Params::carrying_capacity,
                      0,
                      0,
                      std::move(init_cohort_list),
                      0,
                      0,
                      false);

  population_list_.emplace_back(std::move(population));

  // remove pointers within the temporal cohort list.
  init_cohort_list.erase(
        std::remove_if(
          init_cohort_list.begin(),
          init_cohort_list.end(),
          [](const std::shared_ptr<Cohort>) { return true; }),
      init_cohort_list.end());
}

//-----------------------------------------------------------------------------

void Environment::OneRun()
{
  while (++year_grid_ <= Params::run_time)
  {
    OneYear();

    if (Params::is_seed_dynamics == true)
    {
      for (auto const &population : population_list_)
      {
        unique_output_->PrintSeedDynamics(
              population,
              run_num_,
              year_grid_,
              run_fire_interval_mean_,
              run_pollination_success_mean_);
      }
    }
  }

  if (Params::is_persistence == true)
  {
    for (auto const &population : population_list_)
    {
      unique_output_->PrintPersistence(
            population,
            run_num_,
            run_fire_interval_mean_,
            run_pollination_success_mean_);
    }
  }

  year_grid_ = 0;
}

//-----------------------------------------------------------------------------

void Environment::OneYear()
{
  weather_grid_ = weather_sim_.GetYearRandom(climate_grid_, prng2_);

  bool is_fire_event = false;

  for (auto const &population : population_list_)
  {
    population->IncrementTimeSinceFire();

    is_fire_event = IsFireEvent(population);

    if (is_fire_event == true)
    {
      FireEventYes(population);
      is_fire_event = false;
    }
    else
    {
      FireEventNo(population);
    }

    if (population->get_cohorts().empty() == false)
    {
      population->IncrementPersistenceTime();
    }
    else
    {
      population->set_is_extinct(true);
    }
  }
}

//-----------------------------------------------------------------------------

bool Environment::IsFireEvent(const std::shared_ptr<Population> &population)
{
  if (year_grid_ == 1) { return true; }

  if (Params::is_fire == true)
  {
    if (population->get_time_since_fire() >= population->get_fire_interval())
    {
      return true;
    }
  }

  return false;
}

//-----------------------------------------------------------------------------

void Environment::FireEventYes(const std::shared_ptr<Population> &population)
{
  double seeds_dispersed = 0.0;

  SetNextLocalFire(population);

  population->set_time_since_fire(0);

  for (auto const &cohort : population->get_cohorts())
  {
    seeds_dispersed += cohort->PostfireSeedDispersal();
    cohort->FireMort();
  }

  population->RemoveEmptyCohorts();

  if (seeds_dispersed > 0.0)
  {
    population->AddEmptyCohort(true);
    population->get_cohorts().back()->Germination(seeds_dispersed);
    population->RemoveEmptyCohorts();
  }
}

//-----------------------------------------------------------------------------

void Environment::FireEventNo(const std::shared_ptr<Population> &population)
{
  int living_adults = 0;
  double seeds_dispersed = 0.0;

  for (auto const &cohort : population->get_cohorts())
  {
    cohort->PlantMort(weather_grid_,
                      population->get_carrying_capacity(),
                      prng2_);

    if (cohort->get_age() >= Params::plant_age_maturity)
    {
      living_adults += cohort->get_num_individuals();
    }
  }

  population->RemoveEmptyCohorts();

  for (auto const &cohort : population->get_cohorts())
  {
    cohort->AgingPlantsSeeds();
  }

  for (auto const &cohort : population->get_cohorts())
  {
    if (cohort->get_age() == Params::plant_age_maturity)
    {
      cohort->DensityRegulationAdultPlants(living_adults,
                                           population->get_carrying_capacity());

      if (cohort->get_num_individuals() > 0 &&
          cohort->get_is_postfire() == true)
      {
        population->IncrementNumGenerations();
      }
    }
  }

  population->RemoveEmptyCohorts();

  for (auto const &cohort : population->get_cohorts())
  {
    if (cohort->get_seedbank() > 0.0)
    {
      seeds_dispersed += cohort->InterfireSeedLossAndDispersal();
    }
  }

  if (seeds_dispersed > 0.0)
  {
    population->AddEmptyCohort(false);

    for (auto const &cohort : population->get_cohorts())
    {
      if (cohort->get_age() == 0)
      {
        cohort->Germination(seeds_dispersed);
      }
    }
  }

  for (auto const &cohort : population->get_cohorts())
  {
    if (cohort->get_age() >= Params::plant_age_maturity)
    {
      cohort->SeedProduction(weather_grid_, run_pollination_success_mean_);
    }
  }
}

//-----------------------------------------------------------------------------

void Environment::GetClimate()
{
  const std::string dir = "data/in/climate_scn/";

  const std::string filename = dir + Params::climate_scenario + ".csv";

  climate_grid_.ReadClimate(filename, true, ',');
}

//-----------------------------------------------------------------------------

///@note NOTE this is local fire event, i.e., for one population
void Environment::SetNextLocalFire(
  const std::shared_ptr<Population> &population)
{
  // set next fire interval depending on the fire scenario
  switch (Params::fire_scenario)
  {
    case FireScenario::kDeterministic :
      population->set_fire_interval(run_fire_interval_mean_);
      break;

    case FireScenario::kStochastic :
      population->set_fire_interval(
            fire_sim_.GetFireIntervalStochastic(
              run_fire_interval_mean_,
              Params::fire_lower_limit,
              prng2_));
      break;
  }
}

//-----------------------------------------------------------------------------
// Read simulation
void Environment::GetSim(const std::string &data)
{
  int fire_scenario;
  int species_id;
  int flower_scenario;

  std::stringstream ss(data);

  ss  >> Params::sim_id
      >> Params::is_seed_dynamics
      >> Params::is_persistence
      >> Params::run_time
      >> Params::is_climate
      >> Params::long_term_rain
      >> Params::climate_scenario
      >> Params::is_fire
      >> fire_scenario
      >> Params::fire_interval_min
      >> Params::fire_interval_max
      >> Params::fire_interval_step
      >> Params::fire_lower_limit
      >> species_id
      >> flower_scenario
      >> Params::carrying_capacity
      >> Params::init_individuals
      >> Params::init_seedbank
      >> Params::plant_longevity
      >> Params::plant_age_maturity
      >> Params::recruit_postfire_min
      >> Params::recruit_postfire_max
      >> Params::recruit_postfire_mean
      >> Params::plant_senescence_age
      >> Params::plant_senescence_increase
      >> Params::recruit_interfire
      >> Params::plant_mort_age_a
      >> Params::plant_mort_age_b
      >> Params::plant_mort_age_c
      >> Params::plant_mort_weather_a
      >> Params::plant_mort_weather_b
      >> Params::cone_cycle
      >> Params::seed_longevity
      >> Params::flower_max
      >> Params::flower_age_a
      >> Params::flower_age_b
      >> Params::flower_age_c
      >> Params::flower_weather_a
      >> Params::flower_weather_b
      >> Params::flower_weather_c
      >> Params::flower_weather_d
      >> Params::flower_weather_e
      >> Params::pollination_success_min
      >> Params::pollination_success_max
      >> Params::pollination_success_step
      >> Params::num_follicles
      >> Params::num_seeds
      >> Params::firm_seeds
      >> Params::viable_seeds
      >> Params::insect_a
      >> Params::insect_b
      >> Params::decay_a
      >> Params::decay_b
      >> Params::open_follicles_a
      >> Params::open_follicles_b
      >> Params::open_follicles_c
      ;

  switch (fire_scenario)
  {
    case 0 :
      Params::fire_scenario = FireScenario::kDeterministic;
      break;

    case 1 :
      Params::fire_scenario = FireScenario::kStochastic;
      break;

    default :
      std::cout << "The parameter 'fire_scenario' is not valid:\n"
                   "- value '0' for deterministic fire events\n"
                   "- value '1' for stochastic fire events"
                << std::endl;
      std::exit(EXIT_FAILURE);
  }

  switch (species_id)
  {
    case 0 :
      Params::species_id = SpeciesId::kBanksiaHookeriana;
      break;

    default:
      std::cout << "The parameter 'species_id' is not valid:\n"
                   "- value '0' for 'Banksia hookeriana' fire events"
                << std::endl;
      std::exit(EXIT_FAILURE);
  }

  switch (flower_scenario)
  {
    case 0 :
      Params::flower_scenario = FlowerScenario::kBaseline;
      break;

    case 1 :
      Params::flower_scenario = FlowerScenario::kCurrent;
      break;

    case 2 :
      Params::flower_scenario = FlowerScenario::kCurrentNB;
      break;

    default :
      std::cout << "The parameter 'flower_scenario' is not valid:\n"
                   "- value '0' for baseline scenario\n"
                   "- value '1' for current scenario using lmer\n"
                   "- value '2' for current scenario using glmer.nb"
                << std::endl;
      std::exit(EXIT_FAILURE);
  }

  GetClimate();

  // Design output file names
  const std::string dir = "data/out/";              // output directory
  const std::string fid = Params::output_prefix;    // file identifier
  const std::string sim_id = Params::sim_id;

  std::string seed_dynamics = dir + fid + "_dynamics_" + sim_id + ".csv";
  std::string persistence = dir + fid + "_persistence_" + sim_id + ".csv";

  unique_output_ = std::unique_ptr<Output>( new Output() );
  unique_output_->SetupOutput(seed_dynamics, persistence);

  auto probando = std::make_shared<int>(1);

}

//-----------------------------------------------------------------------------
