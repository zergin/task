////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2006-2012, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#define L10N                                           // Localization complete.

#include <sstream>
#include <algorithm>
#include <Context.h>
#include <ViewText.h>
#include <main.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <CmdUDAs.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdUDAs::CmdUDAs ()
{
  _keyword     = "udas";
  _usage       = "task          udas";
  _description = STRING_CMD_UDAS_USAGE;
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdUDAs::execute (std::string& output)
{
  int rc = 0;
  std::stringstream out;

  std::vector <std::string> names;
  context.config.all (names);

  std::vector <std::string> udas;
  std::vector <std::string>::iterator name;
  for (name = names.begin (); name != names.end (); ++name)
  {
    if (name->substr (0, 4) == "uda." &&
        name->find (".type") != std::string::npos)
    {
      std::string::size_type period = name->find ('.', 4);
      if (period != std::string::npos)
        udas.push_back (name->substr (4, period - 4));
    }
  }

  if (udas.size ())
  {
    std::sort (udas.begin (), udas.end ());

    // Render a list of UDA name, type and label.
    ViewText view;
    view.width (context.getWidth ());
    view.add (Column::factory ("string", STRING_COLUMN_LABEL_UDA));
    view.add (Column::factory ("string", STRING_COLUMN_LABEL_TYPE));
    view.add (Column::factory ("string", STRING_COLUMN_LABEL_LABEL));


    std::vector <std::string>::iterator uda;
    for (uda = udas.begin (); uda != udas.end (); ++uda)
    {
      std::string type  = context.config.get ("uda." + *uda + ".type");
      std::string label = context.config.get ("uda." + *uda + ".label");
      if (label == "")
        label = *uda;

      int row = view.addRow ();
      view.set (row, 0, *uda);
      view.set (row, 1, type);
      view.set (row, 2, label);
    }

    out << optionalBlankLine ()
        << view.render ()
        << optionalBlankLine ()
        << (udas.size () == 1
              ? format (STRING_CMD_UDAS_SUMMARY,  udas.size ())
              : format (STRING_CMD_UDAS_SUMMARY2, udas.size ()))
        << "\n";
  }
  else
  {
    out << STRING_CMD_UDAS_NO << "\n";
    rc = 1;
  }

  output = out.str ();
  return rc;
}

///////////////////////////////////////////////////////////////////////////////
CmdCompletionUDAs::CmdCompletionUDAs ()
{
  _keyword     = "_udas";
  _usage       = "task          _udas";
  _description = STRING_CMD_UDAS_COMPL_USAGE;
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdCompletionUDAs::execute (std::string& output)
{
  std::vector <std::string> names;
  context.config.all (names);

  std::vector <std::string> udas;
  std::vector <std::string>::iterator name;
  for (name = names.begin (); name != names.end (); ++name)
  {
    if (name->substr (0, 4) == "uda." &&
        name->find (".type") != std::string::npos)
    {
      std::string::size_type period = name->find ('.', 4);
      if (period != std::string::npos)
        udas.push_back (name->substr (4, period - 4));
    }
  }

  if (udas.size ())
  {
    std::sort (udas.begin (), udas.end ());
    join (output, "\n", udas);
    output += "\n";
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
