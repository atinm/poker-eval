#!/bin/bash
#
# Copyright (C) 2006 Mekensleep
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
#  Johan Euphrosine <proppy@aminche.com>
#

BUILDDIR=$1
MASTERDIR=$2
MODULE=poker-eval
CONFIGURATION=Release

SRCDIR=${BUILDDIR}
DISTDIR=${MASTERDIR}
rm -fR ${DISTDIR}
mkdir -p ${DISTDIR}

rsync -avr ${SRCDIR}/include/ ${DISTDIR}/include/ --include='*.h' --include='inlines/' --include='inlines/*.h' --exclude='**'
rsync -av ${SRCDIR}/include/poker_config.h.in ${DISTDIR}/include/poker_config.h

LIBS=${CONFIGURATION}/poker-eval.lib
rsync -av ${SRCDIR}/${LIBS} ${DISTDIR}/lib/

DOCS="\
 ${SRCDIR}/NEWS\
 ${SRCDIR}/README\
 ${SRCDIR}/TODO\
 ${SRCDIR}/AUTHORS\
 ${SRCDIR}/WHATS-HERE\
 ${SRCDIR}/debian/copyright"
rsync -av $DOCS $DISTDIR/doc/
rsync -av ${SRCDIR}/ChangeLog $DISTDIR/doc/changelog

rsync -avr ${SRCDIR}/examples/ ${DISTDIR}/examples/ --include='*.vcproj' --include='*.c' --exclude='**'
