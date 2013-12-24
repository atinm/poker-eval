#
# Copyright (C) 2004-2010 Loic Dachary <loic@dachary.org>
# Copyright (C) 2004-2006 Mekensleep
#
# Mekensleep
# 24 rue vieille du temple
# 75004 Paris
#       licensing@mekensleep.com
#
# This program gives you software freedom; you can copy, convey,
# propagate, redistribute and/or modify this program under the terms of
# the GNU General Public License (GPL) as published by the Free Software
# Foundation (FSF), either version 3 of the License, or (at your option)
# any later version of the GPL published by the FSF.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program in a file in the toplevel directory called "GPLv3".
# If not, see <http://www.gnu.org/licenses/>.
#
# Authors:
#  Loic Dachary <loic@dachary.org>
#
# 
EXTRA_DIST = \
	GPLv3 \
	GPLv3-assent.txt \
	config/ccache.m4 \
	config/gcov.m4 \
	examples/Makefile.dos \
	include/Makefile.dos \
	java/Makefile.dos \
	lib/Makefile.dos \
	tests/Makefile.dos \
	mktab_astud.vcproj \
	mktab_basic.vcproj \
	mktab_evx.vcproj \
	mktab_joker.vcproj \
	mktab_lowball.vcproj \
	mktab_packed.vcproj \
	poker-eval.vcproj \
	poker-eval.sln \
	WHATS-HERE.Java \
	WHATS-HERE \
	gentoo/dev-games/poker-eval/poker-eval-133.0-r1.ebuild \
	csharp/API.cs \
	csharp/Enum.cs \
	csharp/EnumResult.cs \
	csharp/GameParams.cs \
	csharp/StdDeck.cs \
	csharp/Test.cs \
	csharp/Makefile \
	csharp/TODO \
	packaging-farm.bat \
	poker-eval.nsi \
	make_master.sh \
	fink/libpoker-eval.info

SUBDIRS = \
	lib \
	include \
	examples \
	tests

MAINTAINERCLEANFILES = INSTALL Makefile.in aclocal.m4 configure \
	$(filter-out %.m4,$(wildcard config/[a-z]*)) 

pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = poker-eval.pc

ACLOCAL_AMFLAGS = -I config
