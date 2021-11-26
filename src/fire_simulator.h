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
 * @file fire_simulator.h
 *
 * @brief Pick the next fire interval at random
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */

#ifndef FIRE_SIMULATOR_H
#define FIRE_SIMULATOR_H

#include <math.h>
#include <memory>

#include "random_generator.h"

class FireSimulator
{
 public:
  FireSimulator() { }

  int GetFireIntervalStochastic(const int mean,
                                int lower_cut,
                                const std::shared_ptr<RandomGenerator> &prng2);
};

#endif // FIRE_SIMULATOR_H
