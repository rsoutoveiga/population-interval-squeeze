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

#include "output.h"

//-----------------------------------------------------------------------------

const std::vector<std::string> Output::seed_dynamics_header_
({
   "sim_id", "run_num", "year", "is_climate", "climate_scn", "is_fire_event",
   "fire_scn", "fire_mean", "pollination_mean", "time_since_fire",
   "fire_interval", "is_postfire_coh", "age", "individuals",
   "firm_seeds", "viable_seeds", "total_firm_seeds", "total_viable_seeds"
});

const std::vector<std::string> Output::persistence_header_
({
  "sim_id", "run_num", "is_climate", "climate_scn", "is_fire_event",
  "fire_scn", "pollination_mean", "fire_mean", "is_extinct",
  "persistence_time", "num_generations"
});

//-----------------------------------------------------------------------------

Output::Output()
  : seed_dynamics_fn_("data/out/seed_dynamics.txt")
  , persistence_fn_("data/out/persistence.txt")
{}

//-----------------------------------------------------------------------------

Output::~Output()
{
  Cleanup();
}

//-----------------------------------------------------------------------------
///@todo TODO check how it works mid_batch
void Output::SetupOutput(const std::string &seed_dynamics_fn,
                         const std::string &persistence_fn)
{
  seed_dynamics_fn_ = seed_dynamics_fn;  // file name of seed_dynamics
  persistence_fn_   = persistence_fn;    // files name of persistence

  if (Params::is_seed_dynamics == true)
  {
    seed_dynamics_stream_.open(
          seed_dynamics_fn_.c_str(),
          std::ios_base::app);

    assert(seed_dynamics_stream_.good());

    PrintHeader(seed_dynamics_header_, seed_dynamics_stream_);

    std::cout << "Seed dynamics simulation is running..." << std::endl;
    std::cout << "Sim ID: " << Params::sim_id             << std::endl;
  }

  if (Params::is_persistence == true)
  {
    persistence_stream_.open(
          persistence_fn_.c_str(),
          std::ios_base::app);

    assert(persistence_stream_.good());

    PrintHeader(persistence_header_, persistence_stream_);

    std::cout << "Persistence simulation is running..." << std::endl;
    std::cout << "Sim ID: " << Params::sim_id           << std::endl;
  }
}

//-----------------------------------------------------------------------------

bool Output::IsFileExist(const char *file_name)
{
  std::fstream infile(file_name);
  return infile.good();
}

//-----------------------------------------------------------------------------

void Output::Cleanup()
{
  if (seed_dynamics_stream_.is_open())
  {
    seed_dynamics_stream_.close();
    seed_dynamics_stream_.clear();

    std::cout << "Seed dynamics simulation is complete!" << std::endl;
    std::cout << std::endl;
  }

  if (persistence_stream_.is_open())
  {
    persistence_stream_.close();
    persistence_stream_.clear();

    std::cout << "Peristence simulation is complete!" << std::endl;
    std::cout << std::endl;
  }
}

//-----------------------------------------------------------------------------

void Output::PrintSeedDynamics(const std::shared_ptr<Population> &population,
                               const int run_num,
                               const int year,
                               const int run_fire_interval_meam,
                               const double run_pollination_success_mean)
{
  std::ostringstream ss;

  if (population->get_cohorts().empty() == false)
  {
    for (auto const &coh : population->get_cohorts())
    {
      ss << Params::sim_id                          << ","
         << run_num                                 << ","
         << year                                    << ","
         << Params::is_climate                      << ","
         << Params::climate_scenario                << ","
         << Params::is_fire                         << ","
         << static_cast<int>(Params::fire_scenario) << ","
         << run_fire_interval_meam                  << ","
         << run_pollination_success_mean            << ","
         << population->get_time_since_fire()       << ","
         << population->get_fire_interval()         << ","
         << coh->get_is_postfire()                  << ","
         << coh->get_age()                          << ","
         << coh->get_num_individuals()              << ","
         << coh->get_seedbank()                     << ","
         << coh->get_seedbank() *
            Params::viable_seeds                    << ","
         << coh->get_seedbank() *
            coh->get_num_individuals()              << ","
         << coh->get_seedbank() *
            Params::viable_seeds *
            coh->get_num_individuals()              << std::endl;
    }
  }
  else
  {
    ss << Params::sim_id                          << ","
       << run_num                                 << ","
       << year                                    << ","
       << Params::is_climate                      << ","
       << Params::climate_scenario                << ","
       << Params::is_fire                         << ","
       << static_cast<int>(Params::fire_scenario) << ","
       << run_fire_interval_meam                  << ","
       << run_pollination_success_mean            << ","
       << population->get_time_since_fire()       << ","
       << population->get_fire_interval()         << ","
       << 0                                       << ","
       << 0                                       << ","
       << 0                                       << ","
       << 0                                       << ","
       << 0                                       << ","
       << 0                                       << ","
       << 0                                       << std::endl;
  }

  PrintRow(ss, seed_dynamics_stream_);
}

//-----------------------------------------------------------------------------

void Output::PrintPersistence(const std::shared_ptr<Population> &population,
                              const int run_num,
                              const int run_fire_interval_mean,
                              const double run_pollination_success_mean)
{
  std::ostringstream ss;

  ss << Params::sim_id                          << ","
     << run_num                                 << ","
     << Params::is_climate                      << ","
     << Params::climate_scenario                << ","
     << Params::is_fire                         << ","
     << static_cast<int>(Params::fire_scenario) << ","
     << run_pollination_success_mean            << ","
     << run_fire_interval_mean                  << ","
     << population->get_is_extinct()            << ","
     << population->get_persistence_time()      << ","
     << population->get_num_generations()       << std::endl;

  PrintRow(ss, persistence_stream_);
}

//-----------------------------------------------------------------------------

void Output::PrintRow(std::ostringstream &ss, std::ofstream &stream)
{
  assert(stream.good());

  stream << ss.str();
  stream.flush();
}

//-----------------------------------------------------------------------------

void Output::PrintHeader(std::vector<std::string> row, std::ofstream &stream)
{
  assert(stream.good());

  std::ostringstream ss;

  std::copy(
        row.begin(), row.end() - 1,
        std::ostream_iterator<std::string>(ss, ", "));

  ss << row.back();
  stream << ss.str() << std::endl;
  stream.flush();
}

//-----------------------------------------------------------------------------
