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
#include <Context.h>
#include <Nibbler.h>
#include <text.h>
#include <i18n.h>
#include <DOM.h>
#include <cmake.h>

#ifdef HAVE_LIBLUA
extern "C"
{
  #include <lua.h>
}
#endif

extern Context context;

////////////////////////////////////////////////////////////////////////////////
DOM::DOM ()
{
}

////////////////////////////////////////////////////////////////////////////////
DOM::~DOM ()
{
}

////////////////////////////////////////////////////////////////////////////////
const std::vector <std::string> DOM::get_references () const
{
  std::vector <std::string> refs;

  refs.push_back ("context.program");
  refs.push_back ("context.args");
  refs.push_back ("context.width");
  refs.push_back ("context.height");
  refs.push_back ("system.version");
  refs.push_back ("system.lua.version");
  refs.push_back ("system.os");

  return refs;
}

////////////////////////////////////////////////////////////////////////////////
// DOM Supported References:
//   rc.<name>
//
//   context.program
//   context.args
//   context.width
//   context.height
//
//   TODO stats.<name>           <-- context.stats
//
//   system.version
//   system.lua.version
//   system.os
const std::string DOM::get (const std::string& name)
{
  // Cache test.
/*
  std::map <std::string, std::string>::iterator hit = _cache.find (name);
  if (hit != _cache.end ())
    return hit->second;
*/

  int len = name.length ();
  Nibbler n (name);

  // rc. --> context.config
  if (len > 3 &&
      name.substr (0, 3) == "rc.")
  {
    return /*_cache[name] =*/ context.config.get (name.substr (3));
  }

  // context.*
  else if (len > 8 &&
           name.substr (0, 8) == "context.")
  {
         if (name == "context.program") return /*_cache[name] =*/ context.a3[0]._raw;
    else if (name == "context.args")    return /*_cache[name] =*/ context.a3.combine ();
    else if (name == "context.width")   return /*_cache[name] =*/ format (context.terminal_width);
    else if (name == "context.height")  return /*_cache[name] =*/ format (context.terminal_height);
    else                                throw format (STRING_DOM_UNREC, name);
  }

  // TODO stats.<name>

  // system. --> Implement locally.
  else if (len > 7 &&
           name.substr (0, 7) == "system.")
  {
    // Taskwarrior version number.
    if (name == "system.version")
      return /*_cache[name] =*/ VERSION;

#ifdef HAVE_LIBLUA
    // Lua version number.
    else if (name == "system.lua.version")
      return /*_cache[name] =*/ LUA_RELEASE;
#endif

    // OS type.
    else if (name == "system.os")
#if defined (DARWIN)
      return /*_cache[name] =*/ "Darwin";
#elif defined (SOLARIS)
      return /*_cache[name] =*/ "Solaris";
#elif defined (CYGWIN)
      return /*_cache[name] =*/ "Cygwin";
#elif defined (OPENBSD)
      return /*_cache[name] =*/ "OpenBSD";
#elif defined (HAIKU)
      return /*_cache[name] =*/ "Haiku";
#elif defined (FREEBSD)
      return /*_cache[name] =*/ "FreeBSD";
#elif defined (LINUX)
      return /*_cache[name] =*/ "Linux";
#else
      return /*_cache[name] =*/ STRING_DOM_UNKNOWN;
#endif

    else
      throw format (STRING_DOM_UNREC, name);
  }

  // Pass-through.
  return /*_cache[name] =*/ name;
}

////////////////////////////////////////////////////////////////////////////////
// DOM Supported References:
//
//   TODO <id>.{entry,start,end,scheduled,due,until,wait}
//   TODO <id>.description
//   TODO <id>.project
//   TODO <id>.priority
//   TODO <id>.parent
//   TODO <id>.status
//   TODO <id>.tags
//   TODO <id>.urgency
//   TODO <id>.recur
//   TODO <id>.depends
//
//   TODO <uuid>.{entry,start,end,scheduled,due,until,wait}
//   TODO <uuid>.description
//   TODO <uuid>.project
//   TODO <uuid>.priority
//   TODO <uuid>.parent
//   TODO <uuid>.status
//   TODO <uuid>.tags
//   TODO <uuid>.urgency
//   TODO <uuid>.recur
//   TODO <uuid>.depends
//
//   {entry,start,end,scheduled,due,until,wait}
//   description
//   project
//   priority
//   parent
//   status
//   tags
//   urgency
//   recur
//   depends
//
const std::string DOM::get (const std::string& name, const Task& task)
{
  // Cache test.
/*
  std::map <std::string, std::string>::iterator hit = _cache.find (name);
  if (hit != _cache.end ())
    return hit->second;
*/

  Nibbler n (name);
  int id;
  std::string uuid;
  std::string canonical;

  // <attr>
       if (name == "id")                       return format (task.id);
  else if (name == "urgency")                  return format (task.urgency_c (), 4, 3);
  else if (A3::is_attribute (name, canonical)) return task.get (canonical);

  // <id>.<name>
  else if (n.getInt (id))
  {
    if (n.skip ('.'))
    {
      Task ref;
      if (id == task.id)
        ref = task;
      else
        context.tdb2.get (id, ref);

      std::string attr;
      n.getUntilEOS (attr);

           if (attr == "id")                       return format (ref.id);
      else if (attr == "urgency")                  return format (ref.urgency_c (), 4, 3);
      else if (A3::is_attribute (attr, canonical)) return ref.get (canonical);
    }
  }

  // <uuid>.<name>
  else if (n.getUUID (uuid))
  {
    if (n.skip ('.'))
    {
      Task ref;
      if (uuid == task.get ("uuid"))
        ref = task;
      else
        context.tdb2.get (uuid, ref);

      std::string attr;
      n.getUntilEOS (attr);

           if (name == "id")                       return format (ref.id);
      else if (name == "urgency")                  return format (ref.urgency_c (), 4, 3);
      else if (A3::is_attribute (attr, canonical)) return ref.get (canonical);
    }
  }

  // Delegate to the context-free version of DOM::get.
  return this->get (name);
}

// TODO Need a context-specific DOM::set.  For Lua.  Probably.

////////////////////////////////////////////////////////////////////////////////
void DOM::set (const std::string& name, const std::string& value)
{
  int len = name.length ();

  // rc. --> context.config
  if (len > 3 &&
      name.substr (0, 3) == "rc.")
  {
    context.config.set (name.substr (3), /*_cache[name] =*/ value);
  }

  // Unrecognized --> error.
  else
    throw format (STRING_DOM_CANNOT_SET, name);
}

////////////////////////////////////////////////////////////////////////////////
