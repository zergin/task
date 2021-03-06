#! /usr/bin/perl
################################################################################
## taskwarrior - a command line task list manager.
##
## Copyright 2006-2012, Paul Beckingham, Federico Hernandez.
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included
## in all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
## OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
## THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
## SOFTWARE.
##
## http://www.opensource.org/licenses/mit-license.php
##
################################################################################

use strict;
use warnings;
use Test::More tests => 4;

# Create the rc file.
if (open my $fh, '>', 'color.rc')
{
  print $fh "data.location=.\n",
            "color.due.today=red\n",
            "_forcecolor=1\n";
  close $fh;
  ok (-r 'color.rc', 'Created color.rc');
}

# Test the add command.
qx{../src/task rc:color.rc add due:12/31/2037 nothing};
qx{../src/task rc:color.rc add due:5minutes red};
my $output = qx{../src/task rc:color.rc list};

like ($output, qr/ (?!<\033\[\d\dm) \d{1,2}\/\d{1,2}\/\d{4} (?!>\033\[0m) .* nothing /x, 'none');
like ($output, qr/ (?:\033\[31m|\033\[38;5;9m)        .* red .* \033\[0m/x, 'color.due.today');

# Cleanup.
unlink qw(pending.data completed.data undo.data backlog.data synch.key color.rc);
ok (! -r 'pending.data'   &&
    ! -r 'completed.data' &&
    ! -r 'undo.data'      &&
    ! -r 'backlog.data'   &&
    ! -r 'synch.key'      &&
    ! -r 'color.rc', 'Cleanup');

exit 0;

