// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma_included_headers.hpp>
#include <metashell/shell.hpp>
#include <metashell/exception.hpp>

using namespace metashell;

pragma_included_headers::pragma_included_headers(shell& shell_) : _shell(shell_)
{
}

iface::pragma_handler* pragma_included_headers::clone() const
{
  return new pragma_included_headers(_shell);
}

std::string pragma_included_headers::arguments() const
{
  return "[<expression>]";
}
std::string pragma_included_headers::description() const
{
  return "Displays the list of header files (recursively) included into the "
         "environment. When <expression> is provided, it displays the headers "
         "added to the envrionment by <expression>. Headers that are included "
         "multiple times are listed only once. Headers that are not included "
         "because of being in a conditional (#if ... #endif) part that is "
         "skipped are not listed.";
}

void pragma_included_headers::run(const data::command::iterator&,
                                  const data::command::iterator&,
                                  const data::command::iterator& args_begin_,
                                  const data::command::iterator& args_end_,
                                  iface::displayer& displayer_) const
{
  const std::string env = _shell.env().get_all();

  const auto by_current_env = _shell.engine().files_included_by(env);

  if (args_begin_ == args_end_)
  {
    displayer_.show_filename_set(by_current_env);
  }
  else
  {
    const auto by_ext_env = _shell.engine().files_included_by(
        env + "\n" + data::tokens_to_string(args_begin_, args_end_));

    std::set<boost::filesystem::path> new_headers;
    for (const boost::filesystem::path& p : by_ext_env)
    {
      if (by_current_env.find(p) == by_current_env.end())
      {
        new_headers.insert(p);
      }
    }

    displayer_.show_filename_set(new_headers);
  }
}
