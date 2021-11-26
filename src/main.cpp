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
 * @file mail.ccp
 *
 * @brief Start model. Read simulation scenarios
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <memory>

#include "parameters.h"
#include "random_generator.h"
#include "environment.h"

// ----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  if (argc == 3)
  {
    Params::name_sim_file = argv[1];
    Params::output_prefix = argv[2];
  }
  else
  {
    Params::name_sim_file = "data/in/simulations.txt";
    Params::output_prefix = "sim";
  }

  std::ifstream sim_file;

  sim_file.open(Params::name_sim_file.c_str(), std::ifstream::in);

  //RandomGenerator prng;  // pseudo-random number generator

  auto prng2 = std::make_shared<RandomGenerator>();

  // Checking if the file has been opened correctly
  if (!sim_file)
  {
    std::cout << "ERROR: could not open the simulation file." << std::endl;
    return -1;
  }

  // Temporary strings
  std::string data;
  std::string header;

  std::getline(sim_file, data);
  std::stringstream ss(data);

  int num_replications;

  ss >> header >> num_replications;  // read header and set num_replications

  std::getline(sim_file, header);    // skip the parameter names, i.e. header

  while (std::getline(sim_file, data))
  {
    std::unique_ptr<Environment> run(new Environment());
    run->GetSim(data);    // Get simulation parameters
    run->set_prng2(prng2);

    for (int i = Params::fire_interval_min;
         i <= Params::fire_interval_max;
         i += Params::fire_interval_step)
    {
      run->set_run_fire_interval(i);

      for (double j = Params::pollination_success_min;
           j <= Params::pollination_success_max;
           j += Params::pollination_success_step)
      {
        run->set_run_pollination_success(j);

        for (int k = 0; k < num_replications; ++k)
        {
          run->set_run_num(k);
          run->InitRun();
          run->OneRun();
        }
      }
    }
  }

  sim_file.close();

  std::cout << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << "All simulations are completed!" << std::endl;
  std::cout << std::endl;
  std::cout << '\a' << std::endl;

  return 0;
}

// ----------------------------------------------------------------------------
