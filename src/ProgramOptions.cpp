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
#include "ProgramOptions.hpp"

ProgramOptions::ProgramOptions(const std::string& program_name, const std::string& program_description)
  : program_name_( program_name ), program_description_( program_description ), n_optional_arguments_(0), n_positional_arguments_(0)
{}

void ProgramOptions::addOptionalArgumentFlag(const std::string& name, const std::string& description)
{
  optional_arguments_.add_options()
    (name.c_str(), description.c_str());

  n_optional_arguments_++;
}

void ProgramOptions::parse(int argc, char **argv)
{
  boost::program_options::options_description all_options;
  all_options.add( optional_arguments_ );
  all_options.add( hidden_arguments_ );

  // throws on error
  boost::program_options::store(
    boost::program_options::command_line_parser(argc, argv)
    .options( all_options )
    .positional( positional_arguments_ )
    .run()
  , vm_);
}

void ProgramOptions::notify()
{
  boost::program_options::notify( vm_ );
}

int ProgramOptions::count(const std::string& name) const
{
  return vm_.count( name );
}

std::ostream& operator << (std::ostream& os, const ProgramOptions& program_options)
{
  os << "Usage: " << program_options.program_name_ << " [OPTION]... ";

  for (size_t i=0; i<program_options.n_positional_arguments_; i++)
  {
    os << program_options.positional_arguments_.name_for_position( i ) << " ";

    // if the last one is an unlimited argument, specify additional arguments.
    // TODO this means one or more ('*' in python argparse),
    // maybe there should be an option for 0 or more ('+' in python argparse)
    // TODO actually, any one positional option could have infinite arguments,
    // it doesn't have to be the last one. The only requirement is that only one
    // of all options can have this property. I could notfind a way of
    // retrieving the count parameter from the boost object.
    if (i+1==program_options.n_positional_arguments_ && program_options.positional_arguments_.max_total_count() == std::numeric_limits<unsigned>::max())
    {
      os << "[" << program_options.positional_arguments_.name_for_position( i ) << "]...";
    }
  }
  os << std::endl;

  os << program_options.program_description_ << std::endl;

  os << std::endl;

  // TODO if I try to print the arguments using the ostream operator on the
  // descriptions, the names show up with the '--' prefix. To avoid this
  // maybe we can pass a custom value semantic to the description, but I could
  // not find a way to do it
  if ( 0 < program_options.hidden_arguments_.options().size() )
  {
    os << "positional arguments" << std::endl << std::endl;

    for (const auto& arg_ptr : program_options.hidden_arguments_.options())
      os << "  " << arg_ptr->long_name() << std::endl << "  \t" << arg_ptr->description() << std::endl;
  }

  os << std::endl;

  if ( 0 < program_options.optional_arguments_.options().size() )
  {
    os << "optional arguments" << std::endl << std::endl;

    os << program_options.optional_arguments_;
  }

  return os;
}
