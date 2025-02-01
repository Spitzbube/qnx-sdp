#
# Copyright 2019, QNX Software Systems Ltd. All Rights Reserved.
#
# This source code may contain confidential information of QNX Software
# Systems Ltd.  (QSSL) and its licensors. Any use, reproduction,
# modification, disclosure, distribution or transfer of this software,
# or any software which includes or is based upon any of this code, is
# prohibited unless expressly authorized by QSSL by written agreement. For
# more information (including whether this source code file has been
# published) please email licensing@qnx.com.
#

##############################################################################
#
# Include this file in your common.mk to automatically add branch and
# revision information in a program's use message. The use message must
# be contained in a separate file (e.g., myprogram.use). Use messages
# inside .c files will most likely not work or even fail to build.
#
# Two variables MUST be defined when including this file:
#    USEFILE -- the pathname of the file containing your use message
#    URL -- the SVN URL of the common.mk including this file.
#           To define this variable, add the following line to your
#           common.mk file:
#               URL:=<dollar>URL<dollar>
#           ...where each <dollar> is a single '$'.
#           Also, you will need to include "URL" in the "svn:keywords"
#           property of your common.mk file.
#
# Every instance of the string  %%BRANCH%%  within the USEFILE will be
# replaced by a string of the form <branch>/<dir> @<revision>
# where <branch> is the branch path inside the "product" SVN repository,
# <dir> is the relative path of the directory containing common.mk under-
# neath that branch, and <revision> is the working copy's revision of this
# directory and its descendants.
#
##############################################################################

ifeq ($(USEFILE),)
$(error make variable 'USEFILE' not set)
endif

ifeq ($(URL),)
$(error make variable 'URL' not set)
endif

PUREURL:=$(word 2,$(URL))
ifeq ($(PUREURL),)
$(error Malformed URL variable -- make sure your svn:keywords property contains "URL")
endif

BRANCHPATH:=$(lastword $(subst /product/, ,$(PUREURL)))
ifeq ($(BRANCHPATH),)
$(error SVN URL does not contain '/product/')
endif

REVDIR:=$(PROJECT_ROOT)
ifneq ($(WORKSPACE_TO_WORKING_COPY),)
REVDIR:=$(shell echo $(PROJECT_ROOT) | sed -e 's,$(subst %,[^/]*,$(WORKSPACE_TO_WORKING_COPY)),')
endif

REV:=$(or $(strip $(shell svn info --show-item revision $(REVDIR))),unknown)
BRANCH_REV:=$(patsubst %/,%,$(dir $(BRANCHPATH))) @$(REV)

USEFILE_SRC:=$(USEFILE)
undefine USEFILE
USEFILE:=$(USEFILE_SRC).tmp

.INTERMEDIATE: $(USEFILE)

$(USEFILE): $(USEFILE_SRC)
	sed -e 's,%%BRANCH%%,$(BRANCH_REV),g' $< > $@
