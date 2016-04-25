// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma_evaluate.hpp>
#include <metashell/shell.hpp>
#include <metashell/metashell.hpp>

using namespace metashell;

pragma_evaluate::pragma_evaluate(shell& shell_) : _shell(shell_) {}

iface::pragma_handler* pragma_evaluate::clone() const
{
  return new pragma_evaluate(_shell);
}

std::string pragma_evaluate::arguments() const { return "<code>"; }

std::string pragma_evaluate::description() const
{
  return "Evaluates code as a metaprogram. Use this if Metashell thinks about "
         "the"
         " code that it is an addition to the environment.";
}

void pragma_evaluate::run(const data::command::iterator&,
                          const data::command::iterator&,
                          const data::command::iterator& args_begin_,
                          const data::command::iterator& args_end_,
                          iface::displayer& displayer_) const
{
  const std::string cmd = tokens_to_string(args_begin_, args_end_);

  _shell.run_metaprogram(cmd, displayer_);

  if (!is_environment_setup_command(args_begin_, args_end_))
  {
    displayer_.show_comment(
        data::text{data::paragraph("You don't need the evaluate add pragma to "
                                   "evaluate this metaprogram."
                                   " The following command does this as well:"),
                   data::paragraph(cmd)});
  }
  else
  {
    displayer_.show_comment(data::text(
        "Metashell (incorrectly) thinks that this command should be added to"
        " the environment and would not execute it as a metaprogram without"
        " the \"evaluate\" pragma. Please file a bug report containing"
        " this command (" +
        cmd +
        ") at"
        " https://github.com/metashell/metashell/issues. Thank you."));
  }
}
