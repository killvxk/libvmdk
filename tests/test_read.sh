#!/bin/bash
#
# VMware Virtual Disk (VMDK) format library read buffer testing script
#
# Copyright (c) 2009-2012, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

INPUT="input";
TMP="tmp";

LS="ls";
TR="tr";
WC="wc";

VMDK_TEST_READ="vmdk_test_read";

test_read()
{ 
	echo "Testing read of input:" $*;

	./${VMDK_TEST_READ} $*;

	RESULT=$?;

	echo "";

	return ${RESULT};
}

if ! test -x ${VMDK_TEST_READ};
then
	echo "Missing executable: ${VMDK_TEST_READ}";

	exit ${EXIT_FAILURE};
fi

if ! test -d ${INPUT};
then
	echo "No input directory found, to test read create input directory and place test files in directory.";

	exit ${EXIT_IGNORE};
fi

RESULT=`${LS} ${INPUT}/*.[iI][mM][gG] | ${TR} ' ' '\n' | ${WC} -l`;

if test ${RESULT} -eq 0;
then
	echo "No files found in input directory, to test read place test files in directory.";

	exit ${EXIT_IGNORE};
fi

for VMDK_FILE in `${LS} ${INPUT}/*.[iI][mM][gG] | ${TR} ' ' '\n'`;
do
	if ! test_read ${VMDK_FILE};
	then
		exit ${EXIT_FAILURE};
	fi
done

exit ${EXIT_SUCCESS};

