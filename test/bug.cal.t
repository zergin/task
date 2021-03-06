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
use Test::More tests => 29;

# Create the rc file.
if (open my $fh, '>', 'cal.rc')
{
  print $fh "data.location=.";

  close $fh;
  ok (-r 'cal.rc', 'Created cal.rc');
}

# Bug: The 'cal' command can fail when provided with challenging arguments.

# Should not fail (because they are correct):
my $output = qx{../src/task rc:cal.rc cal 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal');

# y due 2010 donkey 8
$output = qx{../src/task rc:cal.rc cal y 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y');
$output = qx{../src/task rc:cal.rc cal 8 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 8');
$output = qx{../src/task rc:cal.rc cal due 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal due');
$output = qx{../src/task rc:cal.rc cal 2010 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 2010');
$output = qx{../src/task rc:cal.rc cal donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal donkey');

# y due 2010 donkey 8
$output = qx{../src/task rc:cal.rc cal y due 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y due');
$output = qx{../src/task rc:cal.rc cal y 8 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y 8');
$output = qx{../src/task rc:cal.rc cal y 2010 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y 2010');
$output = qx{../src/task rc:cal.rc cal y donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y donkey');
$output = qx{../src/task rc:cal.rc cal 8 due 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 8 due');
$output = qx{../src/task rc:cal.rc cal 8 2010 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 8 2010');
$output = qx{../src/task rc:cal.rc cal 8 donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 8 donkey');
$output = qx{../src/task rc:cal.rc cal due 2010 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal due 2010');
$output = qx{../src/task rc:cal.rc cal due donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal due donkey');
$output = qx{../src/task rc:cal.rc cal 2010 donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 2010 donkey');

# y 8 due 2010 donkey
$output = qx{../src/task rc:cal.rc cal y 8 due 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y 8 due');
$output = qx{../src/task rc:cal.rc cal y 8 2010 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y 8 2010');
$output = qx{../src/task rc:cal.rc cal y 8 donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y 8 donkey');
$output = qx{../src/task rc:cal.rc cal y due 2010 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y due 2010');
$output = qx{../src/task rc:cal.rc cal y due donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y due donkey');
$output = qx{../src/task rc:cal.rc cal y 2010 donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal y 2010 donkey');
$output = qx{../src/task rc:cal.rc cal 8 due 2010 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 8 due 2010');
$output = qx{../src/task rc:cal.rc cal 8 due donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 8 due donkey');
$output = qx{../src/task rc:cal.rc cal 8 2010 donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal 8 2010 donkey');
$output = qx{../src/task rc:cal.rc cal due 2010 8 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal due 2010 8');
$output = qx{../src/task rc:cal.rc cal due 2010 donkey 2>&1};
unlike ($output, qr/(?:Assertion failed|Could note recognize|not a valid)/, 'cal due 2010 donkey');

# Cleanup.
unlink qw(pending.data completed.data undo.data backlog.data synch.key cal.rc);
ok (! -r 'pending.data'   &&
    ! -r 'completed.data' &&
    ! -r 'undo.data'      &&
    ! -r 'backlog.data'   &&
    ! -r 'synch.key'      &&
    ! -r 'cal.rc', 'Cleanup');

exit 0;
