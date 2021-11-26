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

#include "weather_simulator.h"

//-----------------------------------------------------------------------------

std::vector<double> WeatherSimulator::GetYearRandom(
    const ClimateReader &climate,
    const std::shared_ptr<RandomGenerator> &prng2)
{
  next_year_ = prng2->GetUniformUnsignedInt(
                 climate.get_climate_variables().size() - 1);

  return climate.get_climate_variables()[next_year_];
}

//-----------------------------------------------------------------------------
