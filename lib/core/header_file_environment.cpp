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

#include <metashell/header_file_environment.hpp>
#include <metashell/data/headers.hpp>
#include <metashell/data/config.hpp>
#include <metashell/exception.hpp>
#include <metashell/unsaved_file.hpp>
#include <metashell/headers.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/join.hpp>

#include <boost/range/adaptors.hpp>

#include <fstream>
#include <vector>
#include <sstream>

using namespace metashell;

namespace
{
  std::string seq_formatter(const std::string& name_)
  {
    // clang-format off
    return
      "#include <boost/mpl/fold.hpp>\n"
      "#include <boost/mpl/" + name_ + ".hpp>\n"

      "namespace boost_"
      "{"
        "namespace mpl"
        "{"
          "template <class... Ts>"
          "struct " + name_ +
          "{"
            "typedef " + name_ + " type;"
          "};"
        "}"
      "}"

      "namespace metashell"
      "{"
        "namespace impl "
        "{ "
          "template <class C, class Item> "
          "struct " + name_ + "_builder;\n"

          "template <class... Ts, class Item>"
          "struct "
            + name_ + "_builder<::boost_::mpl::" + name_ + "<Ts...>, Item> : "
            "::boost_::mpl::" + name_ + "<"
              "Ts..., typename ::metashell::format<Item>::type"
            ">"
          "{};"
        "} "

        "template <> "
        "struct format_impl<::boost::mpl::" + name_ + "<>::tag> "
        "{ "
          "typedef format_impl type; "

          "template <class V> "
          "struct apply : "
            "::boost::mpl::fold<"
              "V,"
              "::boost_::mpl::" + name_ + "<>,"
              "::metashell::impl::" + name_ + "_builder<"
                "::boost::mpl::_1, "
                "::boost::mpl::_2"
              ">"
            ">"
          "{};"
        "};"
      "}"
      "\n"
      ;
    // clang-format on
  }

  std::string include_formatter(const std::string& name_)
  {
    using std::string;
    using boost::filesystem::path;

    return "#include <" +
           (path("metashell") / "formatter" / (name_ + ".hpp")).string() + ">";
  }

  void add_internal_headers(data::headers& headers_)
  {
    using boost::algorithm::join;
    using boost::adaptors::transformed;
    using boost::filesystem::path;

    using std::string;

    const path internal_dir(headers_.internal_dir());
    const string hpp(".hpp");

    const char* formatters[] = {"vector", "list", "set", "map"};

    for (const char* f : formatters)
    {
      headers_.add(internal_dir / "metashell" / "formatter" / (f + hpp),
                   seq_formatter(f));
    }

    const path vector_formatter =
        path("metashell") / "formatter" / "vector.hpp";

    headers_.add(internal_dir / "metashell" / "formatter" / "deque.hpp",
                 "#include <" + vector_formatter.string() + ">\n");

    headers_.add(
        internal_dir / "metashell" / "formatter.hpp",
        join(formatters | transformed(include_formatter), "\n") + "\n");

    headers_.add(internal_dir / "metashell" / "scalar.hpp",
                 "#include <type_traits>\n"

                 "#define SCALAR(...) "
                 "std::integral_constant<"
                 "std::remove_reference<"
                 "std::remove_cv<"
                 "std::remove_reference<"
                 "decltype((__VA_ARGS__))"
                 ">::type"
                 ">::type"
                 ">::type,"
                 "(__VA_ARGS__)"
                 ">\n");

    headers_.add(internal_dir / "metashell" / "instantiate_expression.hpp",
                 "namespace metashell\n"
                 "{\n"
                 "  template <bool> struct expression_instantiated;\n"
                 "}\n"

                 "#define METASHELL_INSTANTIATE_EXPRESSION(...) \\\n"
                 "  ::metashell::expression_instantiated<true ? true : "
                 "((__VA_ARGS__), false)>\n");
  }
}

header_file_environment::header_file_environment(
    iface::engine& engine_,
    const data::config& config_,
    const boost::filesystem::path& internal_dir_,
    const boost::filesystem::path& env_filename_)
  : _internal_dir(internal_dir_),
    _env_filename(env_filename_),
    _buffer(),
    _headers(internal_dir_),
    _use_precompiled_headers(config_.use_precompiled_headers),
    _engine(engine_)
{
  add_internal_headers(_headers);

  save();
  generate(_headers);
}

void header_file_environment::append(const std::string& s_)
{
  if (_buffer.empty())
  {
    _buffer = s_;
  }
  else
  {
    _buffer += '\n' + s_;
  }

  save();
}

std::string header_file_environment::get() const
{
  return _use_precompiled_headers ?
             std::string() : // The -include directive includes the header
             "#include <" + _env_filename.string() + ">\n";
}

std::string header_file_environment::get_appended(const std::string& s_) const
{
  return get() + s_;
}

void header_file_environment::save()
{
  const boost::filesystem::path fn = _headers.internal_dir() / _env_filename;
  if (!internal_dir().empty())
  {
    const std::string filename = fn.string();
    std::ofstream f(filename.c_str());
    if (f)
    {
      f << _buffer;
    }
    else
    {
      throw exception("Error saving environment to " + filename);
    }
  }

  if (_use_precompiled_headers)
  {
    _engine.generate_precompiled_header(fn);
  }
}

boost::filesystem::path header_file_environment::internal_dir() const
{
  return _internal_dir;
}

const data::headers& header_file_environment::get_headers() const
{
  return _headers;
}

std::string header_file_environment::get_all() const { return _buffer; }
