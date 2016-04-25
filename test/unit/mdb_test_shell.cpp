// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "test_config.hpp"
#include "mdb_test_shell.hpp"

#include <metashell/shell.hpp>
#include <metashell/engine_constant.hpp>

#include <metashell/data/config.hpp>

namespace
{
  boost::filesystem::path temp_dir() { return boost::filesystem::path(); }
  boost::filesystem::path mdb_temp_dir() { return boost::filesystem::path(); }

  boost::filesystem::path env_filename() { return boost::filesystem::path(); }

  metashell::shell& get_shell()
  {
    static metashell::shell sh(metashell::test_config(), temp_dir(),
                               env_filename(), mdb_temp_dir(),
                               metashell::create_failing_engine());
    return sh;
  }
}

mdb_test_shell::mdb_test_shell(const std::string& line)
  : metashell::mdb_shell(get_shell().get_config(),
                         get_shell().env(),
                         get_shell().engine(),
                         get_shell().env_path(),
                         mdb_temp_dir(),
                         nullptr)
{
  env.append(line);
}

mdb_test_shell::mdb_test_shell(metashell::shell& shell, const std::string& line)
  : metashell::mdb_shell(shell.get_config(),
                         shell.env(),
                         shell.engine(),
                         shell.env_path(),
                         mdb_temp_dir(),
                         nullptr)
{
  env.append(line);
}

bool mdb_test_shell::has_metaprogram() const { return static_cast<bool>(mp); }

const metashell::metaprogram& mdb_test_shell::get_metaprogram() const
{
  return *mp;
}

const metashell::breakpoints_t& mdb_test_shell::get_breakpoints() const
{
  return breakpoints;
}
