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
 * @file random_generator.h
 *
 * @brief Pseudo-random number generator
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>
#include <chrono>


class RandomGenerator
{
 private:
  long long int seed_;
  std::mt19937 engine_{seed_};

 public:
  RandomGenerator(const long long int seed =
      std::chrono::system_clock::now().time_since_epoch().count())
    : seed_(seed)
  { }

  unsigned int GetUniformUnsignedInt(const unsigned int max)
  {
    std::uniform_int_distribution<unsigned int>
        distribution(0, max);

    return distribution(engine_);
  }

  double Get01()
  {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(engine_);
  }

  int GetBinomialInt(const int living_plants,
                     const double prob_plant_mortality)
  {
    std::binomial_distribution<int>
        distribution(living_plants, prob_plant_mortality);

    return distribution(engine_);
	}

  double GetGaussian(const int mean, const double sd)
  {
    std::normal_distribution<double> distribution(mean, sd);
    return distribution(engine_);
  }
};

#endif // RANDOM_GENERATOR_H
