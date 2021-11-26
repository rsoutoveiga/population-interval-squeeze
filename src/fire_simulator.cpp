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

#include "fire_simulator.h"

//-----------------------------------------------------------------------------

int FireSimulator::GetFireIntervalStochastic(const int mean,
                                             int lower_cut,
                                             const std::shared_ptr<RandomGenerator> &prng2)
{
  int fire_interval = 0;

  if (lower_cut >= mean)
  {
    lower_cut = mean;
  }

  const int upper_cut = (mean - lower_cut) + mean;

  // range rule for the SD
  double sd = static_cast<double>(upper_cut - lower_cut) / 4.0;

  if (sd < 0.0)
  {
    sd = 0.0;
  }

  do
  {
    fire_interval = static_cast<int>(prng2->GetGaussian(mean, sd) + 0.5);

  } while (fire_interval < lower_cut || fire_interval > upper_cut);

  return fire_interval;
}

//-----------------------------------------------------------------------------
