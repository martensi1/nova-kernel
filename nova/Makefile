# Nova OS
# Copyright (C) 2024 Simon Alm Mårtensson
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented;
#    you must not claim that you wrote the original software.
#    If you use this software in a product, an acknowledgment
#    in the product documentation would be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such,
#    and must not be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.
#

#########################################
# Programs, commands and constants
#########################################
CXX := g++
AS := nasm
MV := mv -f
CP := cp
RM := rm -f
RMDIR := rm -rf
MKDIR := mkdir -p
SED := sed

ANSI_GREEN := \033[1;32m
ANSI_RED := \033[1;31m
ANSI_NC := \033[0m

DATETIME := $(shell date +%Y%m%d%H%M%S)

# Output directories
BUILD_DIR := ../build
OUT_DIR := $(BUILD_DIR)
#OUT_DIR := $(BIN_DIR)/$(ARCH)-$(DATETIME)

BIN_DIR := $(OUT_DIR)/bin
OBJ_DIR := $(OUT_DIR)/obj
BOOT_DIR := $(OUT_DIR)/boot


#########################################
# Rules
#########################################
ifeq ($(MAKECMDGOALS),clean)

.PHONY: clean
clean:
	$(RMDIR) $(BUILD_DIR)

else

include build-rules.mk

endif




