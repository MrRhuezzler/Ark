A FEW NOTES ABOUT GERBIL

Bruce Moreland (brucemo@seanet.com)
21-Jun-2001

CONTENTS:

0) SOME NOTES REGARDING COPYRIGHT AND LICENSING
1) COMPILING THE PROJECT
2) RUNNING GERBIL
3) SOME NOTES REGARDING EDITING SOURCE CODE
4) EDITING THE OPENING BOOK
5) PROBLEMS
6) COMMAND-LINE ARGUMENTS
7) EPD2WB
8) LOSER'S CHESS
9) HOW YOU CAN HELP


0) SOME NOTES REGARDING COPYRIGHT AND LICENSING

All of the sources and executables contained within this project are covered
by the GNU GPL, a copy of which is also included along with this project.

The basic idea is that if you can't sell the project executables without
providing source code, even if you modify the source code, and the source code
must be more or less freely provided and be redistributable.  So you can't
turn this project into proprietary software, even if you make a lot of cool
changes to it.  The copyright on the software is retained by me.

If you have any comments regarding free software, the GPL, my interpretation
of the GPL, or if you have anything to ask or suggest regarding my plans for
this software, please drop me an email.


1) COMPILING THE PROJECT

If you just want to use the chess engine, an executable is provided along
with everything else, so you can skip this section.  You don't need to compile
the project to use it!

If you want to compile the project, first edit "paths.mak" so that your
compiler knows where to find this project, and its own binaries, libraries,
and include files.

Once you do that, you will be able to sit in the "src" directory and type ...

    nmake

... to make the executables.  They will be put in the "ship" subdirectory,
along with the object modules and so on.

If you type ...

    nmake debug=1

... it will make a debug version, which will go into the "debug" directory.
A debug version has asserts (debug checks) enabled in the engine, and it is
compiled without compiler optimizations so it is easier to debug.

The debug version shouldn't be used to play serious chess, since it is very
much slower than the ship version.


2) RUNNING GERBIL

Gerbil is a winboard engine.  I believe that it will work with any version of
Winboard, but it will work best with the newest version.  You can get a new
version from Tim Mann at:

    http://www.tim-mann.org/xboard.html

I have tested Gerbil with:

    4.2.3, current version
    3.4 patchlevel 1, circa 1995

It works well with both of these.

To play against the program, under winboard, the command line is:

    winboard /cp /fcp gerbil.exe

To tell it to play on a chess server such as ICC, the command line is:

    winboard /cp /zp /fcp gerbil.exe /ics /icshost chessclub.com /icshelper timestamp.exe

To get it to work, you might have to move "gerbil.exe" and "gerbil.opn" into
the same directory as Winboard, or mess with Winboard's command line,
Winboard's INI file, and/or Gerbil's INI file ("gerbil.ini").

The INI file expects to be in the same directory as "gerbil.exe", and expects
the first line to be "[settings]".  Documented variables in the INI file are:

    "UseBook".  Accepts 0 or 1.  If non-zero, it will make moves from its
    opening book.  The default is 1.

    "BookFile".  Accepts a filename.  The file where the book is.  Currently,
    the file must exist and contain a valid book, even if "UseBook" is zero.
    The default is "gerbil.opn".

    "MaxHash".  Accepts a positive integer.  Maximum hash table size, in
    bytes.  It will use less than or equal to this number of bytes for
    transposition hash table.  The default is 10,000,000 bytes.  Most chess
    programs will play better if you make this value larger, without using so
    much memory that your computer runs out of physical memory and thrashes.
    I haven't test it, but Gerbil is probably no exception.  The default is
    intended to be an amount that works on any sensible machine, but if you
    have a better machine, you should adjust it upward.

    "MaxPawnHash".  Accepts a positive integer.  Maximum pawn hash table size,
    in bytes.  It will use less than or equal to this number of bytes for
    the pawn structure hash table.  The default is 1,000,000 bytes.  This
    value will typically be set lower than the transposition hash size.  It
    probably doesn't make much sense to fiddle with this parameter.

    "MaxBook".  Accepts a positive integer.  The book is read from disk and
    stored in memory.  If you edit the opening book and make it huge, you
    may have to change this value.  The default is 262144 bytes.  The program
    can use 75% of book storage, and each unique position takes 8 bytes.  It
    will output an error if you try to make the program eat a book that is
    larger than it can handle.  The only reason you'd mess with this is if
    you make major additions to the opening book, and the program will no
    longer initialize (it will spit out an error message if it can't load
    its book).

    "Resign".  Accepts 0 or 1.  If non-zero, the engine will resign if it is
    getting badly beaten and the opponent has a reasonable amount of time.
    The default is 1.  Don't expect the program to resign if you win a piece,
    and in fact it may not resign if it's a queen down.  You have to smash it
    very flat in order to get it to resign.

Example INI file:

    [settings]
    UseBook = 1
    BookFile = gerbil.opn
    MaxHash = 10000000
    MaxPawnHash = 1000000
    MaxBook = 262144
    Resign = 1

You can override any of these INI file values via use of command-line
switches, which are documented later in this document (see the table of
contents at the top).

If you can't get the program to boot, try passing the "/debug" switch to
Winboard, then look at the "winboard.debug" log file after trying to start
Winboard.  There might be a clue in there.  If you see something serious,
please report it to me.


3) SOME NOTES REGARDING EDITING SOURCE CODE

Tabs in the sources are set at 4 characters.  If you try to use an editor that
has tabs set at some other number of characters, everything will look like
hell.

Please see my "hungary.txt" document for some notes regarding my variable
naming convention.


4) EDITING THE OPENING BOOK

The opening book is in text format.  You may wish to use something like
notepad to edit it, since the lines are greater than 80 characters long.

Any edits you make will take effect next time the program is booted.  If you
look at the file format, it should be obvious what kinds of edits are allowed
and make sense.

If you want to "turn off" a book move, for instance "1. e4", change it to
"1. e4?", and it won't play it, nor will it try to transpose via the opening
book into any of the lines (for white) following 1. e4.  Meaning, if you turn
off 1. e4, the program will still play 1. ... c5 or 1. ... e5 as black, after
the opponent plays 1. e4, but if the program plays 1. d4 and the opponent
replies with 1. ... e6, the program will not play 2. e4, since that position
is has essentially been deleted from the program's list of possibilities.

You can probably do alright just by editing the book that is there, or making
your own book that is in the same format, but there are a few tools that will
help you, although they may be hard to use.

If you type from the command line ...

    gerbil -df

... or ...

    gerbil -df

... the program will dump its book to the console.  You can redirect this by
using the ">" symbole, for example:

    gerbil -df >foo

You can redirect to some other file than "foo" if you want, but *do not* try
to redirect onto the book file.  That will delete the book file before the
program has a chance to read it, as I found out the hard way.

If you've used "-df" ("full" mode), your file will contain full opening book
lines, meaning that the program won't try to diff each line with the previous
book line.

If you use "-dc", it will remove some redundant information.  It may be easier
to edit this kind of book if you want to make just a few changes, and this
style is provided as the default opening book in the release.

You can edit either style of book all you want, then copy it over the old
book.

My own suggestion as to how to best create a book is as follows:

    1) Dump the book in full mode.
    2) Edit it all you want.
    3) Sort the new book in ascii order.
    4) Copy it over the old book.
    5) Dump the new book in compact mode.
    6) Copy it over the old book.

You must have "UseBook" turned on in the INI file or none of this will work.

If you want to edit the book, you need to use SAN (Standard Algebraic
Notation).  The book provided is in SAN, and if you want to edit it you need
to use SAN or you'll get an "illegal move" error when you try to load or dump
the book.  Some SAN gotchas:

A) If you have a knight on b1 and another one on d5, it's "Nbc3", not "N1c3".
You disambiguate using a file specifier if possible.

B) If you have a king on e1, kinghts on c3 and g1, and there is an enemy
bishop on b4 pinning the knight against your king, it's "Ne2", not "Nge2".
SAN does not require (or allow) you to consider illegal moves when
disambiguating.

C) Castling is O-O (oh-oh), not 0-0, (zero-zero).

D) Promotion is "e8=Q".  Note the equals sign.


5) PROBLEMS

If you have problems with this project, please take a good solid shot at
working through them, but if you have real trouble, please let me know.  Don't
expect me to do a lot of work for you or teach you C or something.  This
program is supposed to be an opportunity for you to have some harmless and
semi-educational fun for free.  It is not supposed to be a giant rock that I
have to balance on my head for the rest of my life.


6) COMMAND-LINE ARGUMENTS

This is current output produced by "gerbil -?":

    # Usage: gerbil [flags]
    #   -?         | Usage
    #   -b<B>      | Override INI file "UseBook" with B
    #   -bf<F>     | Override INI file "BookFile" with F
    #   -bs<N>     | Override INI file "MaxBook" with N
    #   -dc        | Dump book in compact format (then exit)
    #   -df        | Dump book in fuller format (then exit)
    #   -hp<N>     | Override INI file "MaxPawnHash" with N
    #   -ht<N>     | Override INI file "MaxHash" with N
    #   -p         | Tell the engine to reduce its system priority
    #   -r<B>      | Override INI file "Resign" with B
    #   -t<F> <N>  | Profile FEN F for N seconds

The documentation above is mostly self-explanatory.

The "-p" switch will cause the program to set its priority to slightly below
normal.  This means that if you pass this switch, it will defer to other
processes.  Don't use this switch if you are going to run a match on a single-
processor machine, between two engines, both with pondering (thinking on the
opponent's time) turned on.  If you do this, Gerbil will get very little of
the CPU time.

If you want to pass any of these switches to Winboard, you can do something
like this:

    winboard /cp /fcp "gerbil.exe -ht 25000000"

The space between the "ht" and the "25000000" above is optional.

The "-t" switch is not documented as of now.

7) EPD2WB

EPD2WB is a utility that allows you to run test suites with Winboard engines.
Specifically it works with Gerbil, but it may work with others.

Additional information about this utility is available in the file
"epd2wb.txt".


8) LOSER'S CHESS

Loser's chess is an ICC variant.  You win by being checkmated, being
stalemated, or losing all of your pieces except the king.  Captures are
compulsory, like in checkers, as long as the capture is legal under the rules
of regular chess.

Every move in Loser's chess is a legal chess move, but not all legal chess
moves are legal in Loser's chess.

You can compile the Loser's chess version by typing:

    nmake losers=1

This will create an exectable that plays "Loser's" chess.  It goes into the
"losers" subdirectory and is called "lgerbil.exe".

If you download the project, there will already be an "lgerbil.exe" in there.

By default, the Losers's chess version will not use an opening book.  You are
welcome to create one and tell it to use it.

The book loader will not accept moves that are legal in normal chess and
illegal in Loser's chess, so if you try to use a regular book with the Loser's
engine, you'll get errors at boot.


9) HOW YOU CAN HELP

I need a better opening book.  Anyone who wants to fiddle around with that is
welcome to do so, and if you think you've added an important line or fixed a
bug, please let me know.

The program has a bad Loser's chess evaluation function.  If you have any
ideas about how that can be improved, please let me know.

EPD2WB may not support some Winboard engines.  You can try it with any that
you have lying around, and tell me if they work.  Please look in "epd2wb.txt"
for more information.

If you want to do evaluation work, or if you want to add fancy extensions or
anything like that, go for it, but I probably won't pick up these changes,
even if they make the program better, if they also make it a giant mess, or
divert attention from how the engine works.  The search, move generation,
attack detection, general evaluation, pawn evaluation, and king safety
evaluation need to be extremely clear and obvious, so I am going to be
especially careful about what is added in those areas.

Bug reports are always welcome.

Thank you!


--
Copyright (C) Bruce Moreland, 2001.  All rights reserved.
Please look in "gpl.txt" for information on the GNU General Public License.
