#ifndef METASHELL_COMMAND_PROCESSOR_QUEUE_HPP
#define METASHELL_COMMAND_PROCESSOR_QUEUE_HPP

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

#include <metashell/iface/command_processor.hpp>

#include <vector>
#include <memory>

namespace metashell
{
  class command_processor_queue
  {
  public:
    bool empty() const;
    void push(std::unique_ptr<iface::command_processor> item_);
    void pop(iface::displayer& displayer_);
    void pop_stopped_processors(iface::displayer& displayer_);

    void cancel_operation();
    void line_available(const std::string& cmd_, iface::displayer& displayer_);
    void code_complete(const std::string& s_,
                       std::set<std::string>& out_) const;

    std::string prompt() const;

    void history(iface::history& history_);

  private:
    iface::history* _history; // not owning
    std::vector<std::unique_ptr<iface::command_processor>> _items;
  };
}

#endif
