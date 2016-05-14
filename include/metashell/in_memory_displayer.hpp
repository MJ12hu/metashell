#ifndef METASHELL_IN_MEMORY_DISPLAYER_HPP
#define METASHELL_IN_MEMORY_DISPLAYER_HPP

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

#include <metashell/iface/displayer.hpp>

#include <vector>

namespace metashell
{
  class in_memory_displayer : public iface::displayer
  {
  public:
    typedef std::vector<data::call_graph_node> call_graph;

    virtual void show_raw_text(const std::string& text_) override;
    virtual void show_error(const std::string& msg_) override;
    virtual void show_type(const data::type& type_) override;
    virtual void show_comment(const data::text& msg_) override;
    virtual void show_cpp_code(const std::string& code_) override;

    virtual void show_backtrace(const data::backtrace& trace_) override;
    virtual void show_file_section(const data::file_location& location_,
                                   const std::string& env_buffer_) override;
    virtual void show_frame(const data::frame& frame_) override;
    virtual void show_call_graph(const iface::call_graph& cg_) override;

    virtual void show_filename_list(
        const std::vector<boost::filesystem::path>& filenames_) override;

    virtual void show_filename_set(
        const std::set<boost::filesystem::path>& filenames_) override;

    const std::vector<std::string>& errors() const;
    const std::vector<std::string>& raw_texts() const;
    const std::vector<data::type>& types() const;
    const std::vector<data::text>& comments() const;
    const std::vector<std::string>& cpp_codes() const;

    const std::vector<data::frame>& frames() const;
    const std::vector<data::file_location>& file_locations() const;
    const std::vector<data::backtrace>& backtraces() const;
    const std::vector<call_graph>& call_graphs() const;

    const std::vector<std::vector<boost::filesystem::path>>&
    filename_lists() const;
    const std::vector<std::set<boost::filesystem::path>>& filename_sets() const;

    bool empty() const;
    void clear();

  private:
    std::vector<std::string> _errors;
    std::vector<std::string> _raw_texts;
    std::vector<data::type> _types;
    std::vector<data::text> _comments;
    std::vector<std::string> _cpp_codes;
    std::vector<data::frame> _frames;
    std::vector<data::file_location> _file_locations;
    std::vector<data::backtrace> _backtraces;
    std::vector<call_graph> _call_graphs;
    std::vector<std::vector<boost::filesystem::path>> _filename_lists;
    std::vector<std::set<boost::filesystem::path>> _filename_sets;
  };
}

#endif
