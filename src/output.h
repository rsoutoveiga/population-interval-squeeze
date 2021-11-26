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
 * @file output.h
 *
 * @brief Write and save simulations in CSV files
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

#include "cohort.h"
#include "parameters.h"
#include "population.h"


class Output
{
 private:
  // Describe the measued variables of the annual seedbank dynamics
  static const std::vector<std::string> seed_dynamics_header_;

  // Describes the measured variables of the max persistence time per run
  static const std::vector<std::string> persistence_header_;

  std::ofstream seed_dynamics_stream_;
  std::ofstream persistence_stream_;

  // Filenames
  std::string seed_dynamics_fn_;
  std::string persistence_fn_;

  bool IsFileExist(const char *file_name);
  void PrintHeader(std::vector<std::string> row, std::ofstream &stream);
  void PrintRow(std::ostringstream &ss, std::ofstream &stream);

 public:
  Output();
  ~Output();

  void SetupOutput(const std::string &seed_dynamics_fn,
                   const std::string &persistence_fn);
  void Cleanup();

  void PrintSeedDynamics(const std::shared_ptr<Population> &population,
                         const int run_num,
                         const int year,
                         const int run_fire_interval_mean,
                         const double run_pollination_success_mean);

  void PrintPersistence(const std::shared_ptr<Population> &population,
                        const int run_num,
                        const int run_fire_interval_mean,
                        const double run_pollination_success_mean);
};

#endif // OUTPUT_H
