2010-09-02  Loic Dachary  <loic@dachary.org>

	* Release 138.0

	* debian/rules: debian/stamp-autotools-configure replaces
	  config.status target in newer cdbs
	* debian/rules: debian/stamp-autotools-configure replaces
	  config.status target in newer cdbs
	* debian/control, debian/rules: automake1.9 is not longer required
	* Makefile.am, examples/Makefile.am: manually expand wild cards in
	  variables for POSIX conformance
	* include/deck_undef.h: add missing undefs as suggested by
	  http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=488747
	* configure.ac: add AM_PROG_CC_C_O to allow per object flags
	* ChangeLog, NEWS, configure.ac: bump to version 138.0
	* debian/changelog, debian/libpoker-eval-dev.docs: clear spurious
	  example file

2010-02-10  Loic Dachary <loic@dachary.org>

	* Release 137.0

	* include/enumerate.h: Apply 64bits fix from
	  https://bugs.launchpad.net/ubuntu/+source/pypoker-eval/+bug/508739/comments/2

2009-10-12  Loic Dachary <loic@dachary.org>

	* tests/bug1823: bash4 patch from Ralf Corsepius via Christopher
	  Stone

2009-08-12  Loic Dachary <loic@dachary.org>

	* ChangeLog, NEWS, configure.ac, debian/changelog: Bump to version
	  137.0

2009-07-09  Loic Dachary <loic@dachary.org>

	* Release 136.0

	* debian/rules: Compensate for the case where autoconf has a
	  compatibility problem with libtoolize that can be solved by
	  copying the libtool script directly instead of going thru
	  autoconf ( shows as an error of the type libtool: line 821:
	  X--tag=CC: command not found etc.).

2009-07-07  Loic Dachary <loic@dachary.org>

	* lib/evx_generate.c: move misplaced parenthesis
	* configure.ac, include/poker_wrapper.h, lib/Makefile.am,
	  lib/deck.c, lib/evx_generate.c, lib/poker_wrapper.c,
	  tests/enumtest3.c, tests/poker_wrapper.c: Mac build patches,
	  courtesy Clifford T. Matthews

2009-01-14  Loic Dachary <loic@dachary.org>

	* include/deck.h: Deck_cardString & Deck_printCard must provide
	  &CurDeck instead of CurDeck

2009-01-02  Loic Dachary <loic@dachary.org>

	* include/enumerate.h, lib/combinations.c, lib/evx_generate.c:
	  replace exit() with return because it is bad practice for a
	  library to call
	  http://xulchris.fedorapeople.org/poker-eval-135.0-exit.patch

2008-12-27  Loic Dachary  <loic@dachary.org>

	* configure.ac: remove AC_PROG_CXX because c++ has nothing to do
	  with poker-eval
	* Makefile.am: Add GPLv3 and GPLv3-assent.txt to the distribution
	* LICENSE: update copyrights. Igor was removed because his
	  contribution was made under contract for Mekensleep who has the
	  corresponding copyright.
	* ChangeLog, NEWS, configure.ac, debian/changelog: bump to version
	  136

2008-12-02  Bradley M. Kuhn  <bkuhn@ebb.org>

	* Release 135.0

	* configure.ac (AC_PROG_CPP): AC_PROG_CPP appears to be
	deprecated; switched to AC_PROG_CXX

2008-11-17  Loic Dachary <loic@dachary.org>

	* lib/md5.h, lib/md5c.c: get rid of unused md5 implementation under
	  a proprietary license

2008-04-02  Loic Dachary <loic@dachary.org>

	* GPLv3.txt: 2008: license GPLv3+ added to poker-eval

2008-03-21  Loic Dachary <loic@dachary.org>

	* include/deck_astud.h, include/deck_joker.h, include/deck_std.h,
	  lib/deck.c: Fix overflow as suggested by W. None
	  <miathan6@gmail.com>

2007-12-03  Loic Dachary <loic@dachary.org>

	* tests/Makefile.am: move razz.c to extra dist
	* Makefile.am: add csharp/TODO to distribution
	* tests/Makefile.am: don't do razz for now
	* tests/Makefile.am: bug1823 in EXTRA_DIST
	* tests/razz.c: remove duplicate declaration
	* tests/run.in: run all bugs scripts as standalone
	* tests/bug1823: Run thru all poker-eval/lib/enumerate.c #define
	  INNER_LOOP(evalwrap)
	* Makefile.am: compile examples before tests
	* tests/Makefile.am: bug1823 test case
	* lib/enumerate.c: compare double with > 0.99 instead of 1

2007-06-23  jvivenot

	* java/README: removed

2007-06-19  jvivenot

	* java/org/pokersource/util/test/NestedLoopEnumerationTest.java:
	  enum package name (keyword) => enumerate
	* java/org/pokersource/enum: enum package name (keyword) =>
	  enumerate
	* java/org/pokersource/enumerate/BaseHandGroup.java,
	  java/org/pokersource/enumerate/BeliefVector.java,
	  java/org/pokersource/enumerate/Enumerate.java,
	  java/org/pokersource/enumerate/EnumerateImp.c,
	  java/org/pokersource/enumerate/HandGroup.java,
	  java/org/pokersource/enumerate/HandMatchup.java,
	  java/org/pokersource/enumerate/HandValuation.java,
	  java/org/pokersource/enumerate/HoldemAbdulGroup.java,
	  java/org/pokersource/enumerate/HoldemAtomicGroup.java,
	  java/org/pokersource/enumerate/HoldemBeliefVector.java,
	  java/org/pokersource/enumerate/HoldemCanonGroup.java,
	  java/org/pokersource/enumerate/HoldemHandGroup.java,
	  java/org/pokersource/enumerate/HoldemHandGroupFactory.java,
	  java/org/pokersource/enumerate/HoldemHandOrdering.java,
	  java/org/pokersource/enumerate/HoldemSMGroup.java,
	  java/org/pokersource/enumerate/HoldemThresholdGroup.java,
	  java/org/pokersource/enumerate/HoldemUniversalGroup.java,
	  java/org/pokersource/enumerate/MatchupOutcome.java,
	  java/org/pokersource/enumerate/RankOrdering.java,
	  java/org/pokersource/enumerate/SAIE.java,
	  java/org/pokersource/enumerate/SAIEMain.java,
	  java/org/pokersource/enumerate/ThresholdHandGroup.java,
	  java/org/pokersource/enumerate/package.html,
	  java/org/pokersource/enumerate/test,
	  java/org/pokersource/enumerate/test/.cvsignore,
	  java/org/pokersource/enumerate/test/AllTests.java,
	  java/org/pokersource/enumerate/test/EnumerateTest.java,
	  java/org/pokersource/enumerate/test/HandMatchupTest.java,
	  java/org/pokersource/enumerate/test/HandValuationTest.java,
	  java/org/pokersource/enumerate/test/HoldemAbdulGroupTest.java,
	  java/org/pokersource/enumerate/test/HoldemAtomicGroupTest.java,
	  java/org/pokersource/enumerate/test/HoldemBeliefVectorTest.java,
	  java/org/pokersource/enumerate/test/HoldemCanonGroupTest.java,
	  java/org/pokersource/enumerate/test/HoldemHandGroupFactoryTest.java,
	  java/org/pokersource/enumerate/test/HoldemHandOrderingTest.java,
	  java/org/pokersource/enumerate/test/HoldemSMGroupTest.java,
	  java/org/pokersource/enumerate/test/HoldemThresholdGroupTest.java,
	  java/org/pokersource/enumerate/test/HoldemUniversalGroupTest.java,
	  java/org/pokersource/enumerate/test/SAIETest.java: enum package
	  name (keyword) => enumerate
	* java/org/pokersource/enumerate: enum package name (keyword) =>
	  enumerate
	* java/org/pokersource/AllTests.java: enum package name (keyword)
	  => enumerate

2007-06-19  Loic Dachary <loic@dachary.org>

	* java/README: Note about building with jdk-1.4.

2007-05-16  Loic Dachary <loic@dachary.org>

	* NEWS, debian/changelog: Version bump.
	* tests/Makefile.am, tests/razz.c, tests/run.in: Add fail test for
	  razz.
	* configure.ac: Remove complex GCC flags in favor of simpler one.

2006-12-19  Johan Euphrosine <proppy@aminche.com>

	* Makefile.am, fink, fink/libpoker-eval.info: added fink package

2006-12-17  Loic Dachary  <loic@dachary.org>

	* Release 134.0

2006-12-17  Johan Euphrosine  <proppy@aminche.com>

	* Makefile.am, fink, fink/libpoker-eval.info: added fink package

2006-10-12  proppy  <proppy@call>

	* Release 133.0

	* csharp/TODO: update TODO

	* csharp/Test.cs: added exhaustive unit test for wrap function

2006-10-08  proppy  <proppy@call>

	* csharp/Makefile: update clean rule

	* csharp/API.cs: enum in API

	* csharp/Test.cs: split Test

	* csharp/Makefile: update Makefile

	* csharp/Enum.cs, csharp/EnumResult.cs: EnumResult split

	* csharp/GameParams.cs: GameParams split

	* csharp/StdDeck.cs: StdDeck split

	* csharp/Enum.cs, csharp/API.cs: OrderingMode

	* csharp/API.cs, csharp/Enum.cs: api split

	* csharp/Enum.cs: enum_ fonction

	* csharp/Enum.cs: wrap_ function

	* csharp/Enum.cs: Enum.Sample

	* csharp/Enum.cs: CardMask.Set
	CardMask.Reset

2006-10-07  Loic Dachary  <loic@dachary.org>

	* tests/run.in: tests use a shell script multiplexer for versatility

	* tests/poker_wrapper.c:
	add poker_wrapper helper tests for foreign languages bindings

	* tests/Makefile.am:
	tests use a shell script multiplexer for versatility
	all tests use gcov when available and clean the result afterwards
	add poker_wrapper helper tests for foreign languages bindings

	* lib/poker_wrapper.c:
	add poker_wrapper helper implementation for foreign languages bindings

	* lib/Makefile.am:
	use gcov compile options if available and clean gcov specific files if needed
	add poker_wrapper helper implementation for foreign languages bindings

	* include/poker_wrapper.h:
	add poker_wrapper helper header for foreign languages bindings

	* include/deck_std.h: define the StdDeck_CardMask_EQUAL macro

	* include/Makefile.am: add poker_wrapper helper header in distribution

	* examples/Makefile.am:
	use gcov compile options if available and clean gcov specific files if needed

	* config/gcov.m4: gcov compile options

	* configure.ac: detect valgrind and gcov

	* Makefile.am: gcov.m4 in distribution files

2006-10-07  proppy  <proppy@call>

	* csharp/Enum.cs: StdDeck.StringToCard
	StdDeck.CardToString

	* csharp/Makefile, csharp/Enum.cs:
	c# wrapping of enumResultClear and enumResultPrint

2006-10-07  Loic Dachary  <loic@dachary.org>

	* AUTHORS, ChangeLog, LICENSE, Makefile.am, README, WHATS-HERE, configure.ac, debian/copyright, examples/Makefile.am, examples/pokenum.c, examples/utest1, include/Makefile.am, include/combinations.h, include/deck.h, include/deck_astud.h, include/deck_joker.h, include/deck_std.h, include/deck_undef.h, include/enumdefs.h, include/enumerate.h, include/enumord.h, include/evx_defs.h, include/game_astud.h, include/game_joker.h, include/game_std.h, include/handval.h, include/handval_low.h, include/inlines/eval_low27.h, include/poker_config.h.in, include/poker_defs.h, include/pokereval_export.h, include/rules_astud.h, include/rules_joker.h, include/rules_std.h, include/rules_undef.h, lib/Makefile.am, lib/deck.c, lib/deck_astud.c, lib/deck_joker.c, lib/deck_std.c, lib/evx.c, lib/lowball.c, lib/mktab_astud.c, lib/mktab_basic.c, lib/mktab_evx.c, lib/mktab_joker.c, lib/mktab_lowball.c, lib/mktab_packed.c, lib/mktable.c, lib/mktable.h, lib/mktable_utils.c, lib/rules_astud.c, lib/rules_joker.c, lib/rules_std.c, poker-eval.spec.in, tests/Makefile.am, tests/enumtest1.c, tests/enumtest2.c, tests/enumtest3.c, tests/enumtest5.c, tests/enumtest7.c, tests/joktest1.c, tests/run.in:
	update copyrights

2006-10-01  cpinson  <cpinson@call>

	* gentoo/dev-games/poker-eval/poker-eval-132.0-r1.ebuild: update pkg

2006-08-21  Loic Dachary  <loic@dachary.org>

	* packaging-farm.bat: Script for building windows package using
	http://gna.org/projects/packaging-farm/
	
	* lib/mktable.c: MINGW32 uses LL not i64
	https://gna.org/support/index.php?func=detailitem&item_id=1128

	* lib/Makefile.am:
	$(EXEEXT) added to executable files for cross-platform compatibility

2006-07-24  Loic Dachary  <loic@dachary.org>

	* Release 132.0

	* README: force automake-1.9

Fri Jun 16 2006  Loic Dachary  <loic@dachary.org>

	* configure.ac: s/AC_CONFIG_HEADER/AM_CONFIG_HEADER/

	* make-master.sh, poker-eval.nsi : windows bundle scripts

Fri Jun 09 2006  Cedric Pinson  <cpinson@freesheep.org>

	* gentoo/dev-games/pypoker-eval/poker-eval-131.0-r1.ebuild:
	Update gentoo package

Sat Apr 15 2006  Loic Dachary  <loic@dachary.org>

	* Release 131.0

	* include */*.dos && prevent creation of * dir

	* update all copyright notices

	* Release 130.0

	* configure.ac: all auxiliary files in config

	* bootstrap: prefer autoreconf

	* include/poker_config.h.in: prevent re-definition of autoconf symbols

	* configure.ac: dont compute sizeof int

	* include/poker_defs.h: remove MSC #define HAVE_SYS_STAT_H

	* include/inlines/evx7.h, include/inlines/evx5.h: commit the files 
	  because they are stable enough and won't change a bit in the next
	  month / years

	* lib/t_*.c: removed from distribution and generated at build time
	
	* examples/Makefile.am (EXTRA_DIST): *.vcproj + getopt_w32.c

	* update all copyright dates + change sf.net to gna.org

Thu Apr 13 2006  Loic Dachary  <loic@dachary.org>

	* include/poker_config.h.in, poker-eval/include/poker_defs.h,
	poker-eval/configure.ac, poker-eval/include/config.h.in,
	poker-eval/include/deck_joker.h, poker-eval/include/deck_std.h:

	Neil Burch <burch@cs.ualberta.ca>
	
         - configure tests for the existance of int64 and sets the HAVE_INT64 define
         - configure tests for the existance of long long and sets HAVE_LONG_LONG
         - in include/poker_defs.h, the definition of CardMasks only uses
           HAVE_INT64 to determine whether to use 64 bit integers or not,
           so 32 bit integers will be used even if long long is defined
         - also, if HAVE_INT64 _is_ defined (which is true if int64 was
           discovered to be a valid type by configure), it will then check if
           HAVE_LONG_LONG is defined and try to do a typedef of uint64 to long long.
         
         
         Further, it tests for <inttypes.h> and uses that in the tests, but
         never includes it in the source, even if it exists and was the reason
         why other tests in configure succeeded.  Finally, on all of the
         machines I have access to (SGI Origin, a multiprocessor IBM Power 5
         machine, 32 bit and 64 bit Linux PCs, and some miscellaneous Sun
         boxen) <inttypes.h> defines int64_t, not int64.
         
         There is also a small problem where the initialisation of the static
         tables will fail if HAVE_INT64 is defined, because the CardMask union
         will only define the single 64 bit integer, and the initialisers are
         all pairs of 32 bit integers.
         
         I've include some patches below.  They check for a native int64,
         int64_t, and long long, and will define HAVE_INT64 if any of those are
         found, and create a typedef for uint64 if it was not native.  It also
         causes both the pair of 32 bit integers as well as the 64 bit integer
         to be in the CardMask union so the initialisation of the static
         CardMask tables still works correctly.  I'm also pretty sure the line
         numbers for the configure 	

Sun Mar 26 2006  Loic Dachary  <loic@dachary.org>

	* gentoo packaging files.
	
	* lib/enumord.c (enum_ordering_rank): fix signedness

Sat Jan 14 2006  Loic Dachary  <loic@dachary.org>

	* Release 129.0

	* lib/Makefile.am: -no-undefined for libtool to work with cygwin

	* examples/five_card_hands.c: getopt_w32 not available on cygwin

Thu Sep 22 2005  Loic Dachary  <loic@dachary.org>

	* Release 128.0

	* lib/Makefile.am: remove generated tables on maintainer-clean

	* Portability and test fixes.

Mon Jul 04 16:55:02 2005  Loic Dachary  <loic@dachary.org>

	* Release 127.0	

	* include/inlines/eval_low27.h: A-5 is not a straight

Fri Dec 17 13:41:28 2004  Loic Dachary  <loic@dachary.org>

	*  RELEASE renamed to RELEASE.old to avoid file name conflicts with some
	   build systems.

Fri Dec 10 14:16:27 2004  Loic Dachary  <loic@dachary.org>

	* Release 126.0

	* include/pokereval_export.h: accomodate dynamic/static libraries

	* include/std_deck.h: do not use non portable const 
	
Fri Dec 03 13:31:2200.04  Loic Dachary  <loic@dachary.org>

	* Release 125.0

	* config/build.m4 : removed because too complex to maintain

	* configure.ac, bootstrap: lower autoconf requirements to 2.53

Thu Dec 02 3:00:0200.04  Tim Showalter  <tjs@psaux.com>

	* Updated Mac builds; currently, things seem to build with only
	  one config setting (--with-mac-target=darwin).
	* Make it unnecessary to have '.' in one's path when building.
	* bootstrap now checks for 'libtoolize' in both places, not 'libtool'
	  (My Debian system has no 'libtool', but my Mac does, and it's
	  not actually related to GNU libtool.)
	* boostrap now moves configure.in aside to shut up all the programs
	  that worry about configure.in's presence.
	* Fink now detected; --with-fink will default to /sw, as the
	  help claimed it did.
	* Default Mac build type is now darwin, since we have no carbon build.

Thu Nov 25 12:27:09 2004  Loic Dachary  <loic@dachary.org>

	* "Nick Wilton" <info@nickwilton.info> header patches + Makefiles for compilation on windows

Mon Nov 15 20:20:43 2004  Loic Dachary  <loic@dachary.org>

	* poker-eval.spec.in: by Jean-Christophe Duberga <jeanchristophe.duber@free.fr>

Sun Nov 07 15:53:27 2004  Loic Dachary  <loic@dachary.org>

	* Release 124.0
	
	* include/enumerate.h: apply macro/comma related patch from Martin Stjernholm
	  http://sourceforge.net/mailarchive/forum.php?thread_id=5586390&forum_id=40226

Sun Nov 07 13:55:00 2004  Loic Dachary  <loic@dachary.org>

	* WHATS-HERE.Java: cut/paste java related stuff from WHATS-HERE

	* WHATE-HERE, README: review and update
	
	* test/digest[57].c: rewrite into enumtest[57] to get rid of md5 
	  dependency
	
	* lib/*md5*,include/md5.h: license problem and do not belong here
	
	* include/{,inlines/}*.h: add or update copyright notice at the beginning of the file.
	  Add POKEREVAL_EXPORT markers to extern symbols, include pokereval_export.h when
	  doing so. Add multiple inclusion guards when missing.
	
	* include/enumerate.h: random is called rand if WIN32 is defined

	* include/pokereval_export.h: add file to cope with
	  POKEREVAL_EXPORT keyword made to enable the use of poker-eval as
	  a shared library on windows.

	* include/Makefile.am (nobase_pkginclude_HEADERS): pokereval_export.h

	* lib/mktable.c: %xLL is %xi64 if WIN32 is defined

Sun Nov 07 11:42:07 2004  Loic Dachary  <loic@dachary.org>

	* tests/run.in : tests from Makefile.in are now in a script. Accomodate for
	  md5sum change in calling syntax

	* configure.ac: detect awk / md5sum
	
	* configure.ac/Makefile.am: java is not included in the distribution until
	  licensing issues are solved.
	
	* include/deck_std.h (StdDeck_CardMask_OP): missing backslash at end of line

	* config/ccache.m4, config/build.m4: macros for configure.ac

	* configure.ac: modern configure for autoconf2.5, do not override configure.in that
	  is still the default for older configures

	* include/poker_config.h.in: obsoletes include/config.h.in to avoid name conflicts
	  when distributing the headers. include/config.h is still being used for inclusions
	  within the poker-eval sources but is not published.

	* bootstrap: script to re-generate autotools files

	* poker-eval.pc.in: add pkg-config declaration file

	* AUTHORS: add missing list of authors

	* NEWS: RELEASE is now NEWS (standard file) but RELEASE is kept back

	* {include,tests,lib,examples}/Makefile.am : automake compatibility

Local Variables:
compile-command: "svn update ; svn2cl --group-by-day --authors=$(echo $HOME)/.svn2cl --stdout | head -2000 | sed -n '0,/^2010-06-20/p'"
End:
