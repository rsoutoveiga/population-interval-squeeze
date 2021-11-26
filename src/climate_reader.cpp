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

#include "climate_reader.h"

//-----------------------------------------------------------------------------

ClimateReader::ClimateReader(const std::string &file_name,
                             const bool header,
                             const char delimeter)
{
  ReadClimate(file_name, header, delimeter);
}

//-----------------------------------------------------------------------------

void ClimateReader::ReadClimate(const std::string &file_name,
                                const bool header,
                                const char delimeter)
{
  std::ifstream ifile;

  ifile.open(file_name, std::ifstream::in);

  std::string line = "";

  // skip header row
  if (header == true)
  {
    std::getline(ifile, line);
  }

  while (std::getline(ifile, line))
  {
    std::vector<double> weather_variables_set;

    std::istringstream iss{line};

    // read the tokens from current line separated by delimiter
    std::vector<std::string> tokens;  // excel row
    std::string token;                // excel cell

    while (std::getline(iss, token, delimeter))
    {
      tokens.push_back(token);
    }

    // Process the tokens
    for (const auto &elem : tokens)
    {
      weather_variables_set.push_back(std::stod(elem));
    }

    climate_variables_.push_back(weather_variables_set);
  }

  ifile.close();
}
