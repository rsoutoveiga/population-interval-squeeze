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
 * @file climate_reader.h
 *
 * @brief Read the climate variables from a csv file
 *
 * @author Rodrigo Souto-Veiga
 * Contact: rsoutoveiga@protonmail.com
 */


#ifndef CLIMATE_READER_H
#define CLIMATE_READER_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

class ClimateReader
{

private:

  std::vector<std::vector<double>> climate_variables_;

public:

  ClimateReader() {}

  ClimateReader(const std::string &file_name,
                const bool header = true,
                const char delimeter = ',');

  std::vector<std::vector<double>> get_climate_variables() const {
    return climate_variables_;
  }

  void ReadClimate(const std::string &file_name,
                   const bool header = true,
                   const char delimeter = ',');
};

#endif // CLIMATE_READER_H
