////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2010 - 2011, Paul Beckingham, Federico Hernandez.
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
//
//     Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor,
//     Boston, MA
//     02110-1301
//     USA
//
////////////////////////////////////////////////////////////////////////////////

#define L10N                                           // Localization complete.

#include <stdlib.h>
#include <string.h>
#include <RX.h>

//#define _POSIX_C_SOURCE 1      // Forgot why this is here.  Moving on...

////////////////////////////////////////////////////////////////////////////////
RX::RX ()
: _compiled (false)
, _pattern ("")
, _case_sensitive (true)
{
}

////////////////////////////////////////////////////////////////////////////////
RX::RX (
  const std::string& pattern,
  bool case_sensitive /* = true */)
: _compiled (false)
, _pattern (pattern)
, _case_sensitive (case_sensitive)
{
  compile ();
}

////////////////////////////////////////////////////////////////////////////////
RX::RX (const RX& other)
: _compiled (false)
, _pattern (other._pattern)
, _case_sensitive (other._case_sensitive)
{
}

////////////////////////////////////////////////////////////////////////////////
RX& RX::operator= (const RX& other)
{
  if (this != &other)
  {
    _compiled       = false;
    _pattern        = other._pattern;
    _case_sensitive = other._case_sensitive;
  }

  return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool RX::operator== (const RX& other) const
{
  return _pattern        == other._pattern &&
         _case_sensitive == other._case_sensitive;
}

////////////////////////////////////////////////////////////////////////////////
RX::~RX ()
{
  if (_compiled)
    regfree (&_regex);
}

////////////////////////////////////////////////////////////////////////////////
void RX::compile ()
{
  if (!_compiled)
  {
    memset (&_regex, 0, sizeof (regex_t));

    int result;
    if ((result = regcomp (&_regex, _pattern.c_str (),
                           REG_EXTENDED | /*REG_NOSUB |*/ REG_NEWLINE |
                           (_case_sensitive ? 0 : REG_ICASE))) != 0)
    {
      char message[256];
      regerror (result, &_regex, message, 256);
      throw std::string (message);
    }

    _compiled = true;
  }
}

////////////////////////////////////////////////////////////////////////////////
bool RX::match (const std::string& in)
{
  if (!_compiled)
    compile ();

  return regexec (&_regex, in.c_str (), 0, NULL, 0) == 0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
bool RX::match (
  std::vector<std::string>& matches,
  const std::string& in)
{
  if (!_compiled)
    compile ();

  regmatch_t rm[RX_MAX_MATCHES];
  if (regexec (&_regex, in.c_str (), RX_MAX_MATCHES, rm, 0) == 0)
  {
    for (unsigned int i = 1; i < 1 + _regex.re_nsub; ++i)
      matches.push_back (in.substr (rm[i].rm_so, rm[i].rm_eo - rm[i].rm_so));

    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
bool RX::match (
  std::vector <int>& start,
  std::vector <int>& end,
  const std::string& in)
{
  if (!_compiled)
    compile ();

  regmatch_t rm[RX_MAX_MATCHES];
  if (regexec (&_regex, in.c_str (), RX_MAX_MATCHES, rm, 0) == 0)
  {
    for (unsigned int i = 1; i < 1 + _regex.re_nsub; ++i)
    {
      start.push_back (rm[i].rm_so);
      end.push_back   (rm[i].rm_eo);
    }

    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////