#ifndef METASHELL_PRAGMA_WHICH_HPP
#define METASHELL_PRAGMA_WHICH_HPP

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

#include <metashell/iface/pragma_handler.hpp>
#include <metashell/data/include_type.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/operators.hpp>

#include <iosfwd>

namespace metashell
{
  class shell;

  class pragma_which : public iface::pragma_handler
  {
  public:
    struct parsed_arguments : boost::equality_comparable<parsed_arguments>
    {
      parsed_arguments(data::include_type include_type_,
                       const boost::filesystem::path& path_,
                       bool all_);
      data::include_type include_type;
      boost::filesystem::path path;
      bool all;
    };

    explicit pragma_which(shell& shell_);

    virtual iface::pragma_handler* clone() const override;

    virtual std::string arguments() const override;
    virtual std::string description() const override;

    virtual void run(const data::command::iterator& name_begin_,
                     const data::command::iterator& name_end_,
                     const data::command::iterator& args_begin_,
                     const data::command::iterator& args_end_,
                     iface::displayer& displayer_) const override;

    static parsed_arguments
    parse_arguments(const data::command::iterator& name_begin_,
                    const data::command::iterator& name_end_,
                    const data::command::iterator& args_begin_,
                    const data::command::iterator& args_end_);

  private:
    shell& _shell;
  };

  std::ostream& operator<<(std::ostream& out_,
                           const pragma_which::parsed_arguments& args_);
  bool operator==(const pragma_which::parsed_arguments& a_,
                  const pragma_which::parsed_arguments& b_);
}

#endif
