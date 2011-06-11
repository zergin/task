////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2006 - 2011, Paul Beckingham, Federico Hernandez.
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

#include <fstream>
#include <sstream>
#include <Context.h>
#include <Uri.h>
#include <Transport.h>
#include <text.h>
#include <util.h>
#include <CmdMerge.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdMerge::CmdMerge ()
{
  _keyword     = "merge";
  _usage       = "task merge URL";
  _description = "Merges the specified undo.data file with the local data files.";
  _read_only   = false;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdMerge::execute (std::string& output)
{
/*
  std::string file = trim (context.task.get ("description"));
  std::string pushfile = "";
  std::string tmpfile = "";

  std::string sAutopush = lowerCase (context.config.get        ("merge.autopush"));
  bool        bAutopush =            context.config.getBoolean ("merge.autopush");

  Uri uri (file, "merge");
  uri.parse();

  if (uri.data.length ())
  {
    Directory location (context.config.get ("data.location"));

    // be sure that uri points to a file
    uri.append ("undo.data");

    Transport* transport;
    if ((transport = Transport::getTransport (uri)) != NULL )
    {
      tmpfile = location.data + "/undo_remote.data";
      transport->recv (tmpfile);
      delete transport;

      file = tmpfile;
    }
    else
      file = uri.path;

    context.tdb.lock (context.config.getBoolean ("locking"));
    context.tdb.merge (file);
    context.tdb.unlock ();

    output += "Merge complete.\n";

    if (tmpfile != "")
      remove (tmpfile.c_str ());

    if ( ((sAutopush == "ask") && (confirm ("Would you like to push the merged changes to \'" + uri.data + "\'?")) )
       || (bAutopush) )
    {
      context.task.set ("description", uri.data);

      std::string out;
      context.commands["push"]->execute (out);
    }
  }
  else
    throw std::string ("No uri was specified for the merge.  Either specify "
                       "the uri of a remote .task directory, or create a "
                       "'merge.default.uri' entry in your .taskrc file.");
*/

  return 0;
}

////////////////////////////////////////////////////////////////////////////////