/**
 * This file is part of ProgramOptions.
 *
 * Copyright (C) Thomas Fischer
 * 
 * ProgramOptions is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProgramOptions is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProgramOptions.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Thomas Fischer
 */
#pragma once

#include <string>
#include <boost/program_options.hpp>

class ProgramOptions
{
  public:

    ProgramOptions(const std::string& program_name, const std::string& program_description);

    template<typename T>
    void addOptionalArgument(const std::string& name, const std::string& description, T& value);

    template<typename T>
    void addPositionalArgument(const std::string& name, const std::string& description, T& value, int count = 1);

    void addOptionalArgumentFlag(const std::string& name, const std::string& description);

    /**
     * @brief parse command line values.
     */
    void parse(int argc, char **argv);

    /**
     * @brief throws argument parsing exceptions.
     *
     * TODO if a positional option is missing, it shows up with '--' prefix in
     * the error string. The same happens when printing arguments. To avoid
     * this maybe we can pass a custom value semantic to the description, but
     * I could not find a way to do it. If we manage this, maybe we can print
     * the help string in the standard way.
     */
    void notify();

    /**
     * @brief count given values for an argument.
     */
    int count(const std::string& name) const;

    /**
     * @brief Print usage message to stream.
     */
    friend std::ostream& operator << ( std::ostream& os, const ProgramOptions& program_options );

  private:

    const std::string program_name_, program_description_;

    size_t n_optional_arguments_;
    boost::program_options::options_description optional_arguments_;

    size_t n_positional_arguments_;
    boost::program_options::options_description hidden_arguments_;
    boost::program_options::positional_options_description positional_arguments_;

    boost::program_options::variables_map vm_;
};

template<typename T>
void ProgramOptions::addPositionalArgument(const std::string& name, const std::string& description, T& value, int count)
{
  hidden_arguments_.add_options()
    (name.c_str(), boost::program_options::value<T>(&value)->required(), description.c_str());

  positional_arguments_.add(name.c_str(), count);

  n_positional_arguments_++;
}

template<typename T>
void ProgramOptions::addOptionalArgument(const std::string& name, const std::string& description, T& value )
{
  optional_arguments_.add_options()
    (name.c_str(), boost::program_options::value<T>(&value), description.c_str());

  n_optional_arguments_++;
}
