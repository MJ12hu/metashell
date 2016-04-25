// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include "console_config.hpp"

#include <metashell/parse_config.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/shell.hpp>
#include <metashell/logger.hpp>
#include <metashell/fstream_file_writer.hpp>
#include <metashell/engine_clang.hpp>
#include <metashell/engine_entry.hpp>

#include <metashell/version.hpp>
#include <metashell/wave_tokeniser.hpp>
#include <metashell/readline/version.hpp>

#include <metashell/data/config.hpp>

#include <boost/range/adaptor/map.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>

#include <just/temp.hpp>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>
#include <map>

namespace
{
  std::map<std::string, std::string> get_dependency_versions()
  {
    const std::string readline_name =
#ifdef USE_EDITLINE
        "Libedit"
#else
        "Readline"
#endif
        ;

    return {{"Boost.Wave", metashell::wave_version()},
            {readline_name, metashell::readline::version()}};
  }
}

int main(int argc_, const char* argv_[])
{
  const std::string env_filename = "metashell_environment.hpp";

  try
  {
    using metashell::parse_config;
    using metashell::parse_config_result;

    const std::map<std::string, metashell::engine_entry> engines{
        {"internal", metashell::get_internal_templight_entry()},
        {"clang", metashell::get_engine_clang_entry()}};

    metashell::default_environment_detector det(argv_[0]);

    const parse_config_result r =
        parse_config(argc_, argv_, engines, det, &std::cout, &std::cerr);

    metashell::console_config ccfg(
        r.cfg.con_type, r.cfg.indent, r.cfg.syntax_highlight);

    metashell::fstream_file_writer file_writer;
    metashell::logger logger(ccfg.displayer(), file_writer);
    switch (r.cfg.log_mode)
    {
    case metashell::data::logging_mode::none:
      // do nothing
      break;
    case metashell::data::logging_mode::console:
      logger.log_to_console();
      break;
    case metashell::data::logging_mode::file:
      logger.log_into_file(r.cfg.log_file);
      break;
    }

    METASHELL_LOG(&logger, "Start logging");

    const auto eentry = engines.find(r.cfg.engine);
    if (eentry == engines.end())
    {
      throw std::runtime_error(
          "Engine " + r.cfg.engine + " not found. Available engines: " +
          boost::algorithm::join(engines | boost::adaptors::map_keys, ", "));
    }
    else
    {
      if (r.should_run_shell())
      {
        using boost::filesystem::path;

        METASHELL_LOG(&logger, "Running shell");

        just::temp::directory dir;

        const path temp_dir = path(dir.path()) / "shell";
        const path mdb_dir = path(dir.path()) / "mdb";

        create_directories(temp_dir);
        create_directories(mdb_dir);

        std::unique_ptr<metashell::shell> shell(new metashell::shell(
            r.cfg, ccfg.processor_queue(), temp_dir, env_filename, mdb_dir,
            eentry->second.build(
                r.cfg, temp_dir, env_filename, det, ccfg.displayer(), &logger),
            &logger));

        if (r.cfg.splash_enabled)
        {
          shell->display_splash(ccfg.displayer(), get_dependency_versions());
        }

        ccfg.processor_queue().push(move(shell));

        METASHELL_LOG(&logger, "Starting input loop");

        metashell::input_loop(
            ccfg.processor_queue(), ccfg.displayer(), ccfg.reader());

        METASHELL_LOG(&logger, "Input loop finished");
      }
      else
      {
        METASHELL_LOG(&logger, "Not running shell");
      }
      return r.should_error_at_exit() ? 1 : 0;
    }
  }
  catch (std::exception& e_)
  {
    std::cerr << "Error: " << e_.what() << std::endl;
    return 1;
  }
}
