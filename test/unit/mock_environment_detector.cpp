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

#include <metashell/data/config.hpp>

#include "mock_environment_detector.hpp"

mock_environment_detector::mock_environment_detector()
  : _search_clang_binary_called_times(0),
    _file_exists_returns(true),
    _file_exists_called_times(0),
    _on_windows_returns(false),
    _on_windows_called_times(0),
    _on_osx_returns(false),
    _on_osx_called_times(0),
    _directory_of_executable_called_times(0)
{
}

void mock_environment_detector::search_clang_binary_returns(
    const boost::filesystem::path& result_)
{
  _search_clang_binary_returns = result_;
}

boost::filesystem::path mock_environment_detector::search_clang_binary()
{
  ++_search_clang_binary_called_times;
  return _search_clang_binary_returns;
}

int mock_environment_detector::search_clang_binary_called_times() const
{
  return _search_clang_binary_called_times;
}

bool mock_environment_detector::file_exists(
    const boost::filesystem::path& path_)
{
  ++_file_exists_called_times;
  _file_exists_last_arg = path_;
  return _file_exists_returns;
}

int mock_environment_detector::file_exists_called_times() const
{
  return _file_exists_called_times;
}

const boost::filesystem::path&
mock_environment_detector::file_exists_last_arg() const
{
  return _file_exists_last_arg;
}

void mock_environment_detector::file_exists_returns(bool result_)
{
  _file_exists_returns = result_;
}

bool mock_environment_detector::on_windows()
{
  ++_on_windows_called_times;
  return _on_windows_returns;
}

void mock_environment_detector::on_windows_returns(bool result_)
{
  _on_windows_returns = result_;
}

int mock_environment_detector::on_windows_called_times() const
{
  return _on_windows_called_times;
}

bool mock_environment_detector::on_osx()
{
  ++_on_osx_called_times;
  return _on_osx_returns;
}

void mock_environment_detector::on_osx_returns(bool result_)
{
  _on_osx_returns = result_;
}

int mock_environment_detector::on_osx_called_times() const
{
  return _on_osx_called_times;
}

boost::filesystem::path mock_environment_detector::directory_of_executable()
{
  ++_directory_of_executable_called_times;
  return _directory_of_executable_returns;
}

void mock_environment_detector::directory_of_executable_returns(
    const boost::filesystem::path& result_)
{
  _directory_of_executable_returns = result_;
}

int mock_environment_detector::directory_of_executable_called_times() const
{
  return _directory_of_executable_called_times;
}
