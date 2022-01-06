################################################################################
#
# python-pycryptodome
#
################################################################################

PYTHON_PYCRYPTODOME_VERSION = 3.12.0
PYTHON_PYCRYPTODOME_SOURCE = pycryptodome-$(PYTHON_PYCRYPTODOME_VERSION).zip
PYTHON_PYCRYPTODOME_SITE = https://files.pythonhosted.org/packages/dd/3b/38d83c2725fec5b837fc05d5c6e0178a420ff839a6ad5872f10c89244899
PYTHON_PYCRYPTODOME_SETUP_TYPE = setuptools
PYTHON_PYCRYPTODOME_LICENSE = FIXME: license id couldn't be detected
PYTHON_PYCRYPTODOME_LICENSE_FILES = LICENSE.rst Doc/src/license.rst

define PYTHON_PYCRYPTODOME_EXTRACT_CMDS
	$(UNZIP) -d $(@D) $(PYTHON_PYCRYPTODOME_DL_DIR)/$(PYTHON_PYCRYPTODOME_SOURCE)
	mv $(@D)/pycryptodome-$(PYTHON_PYCRYPTODOME_VERSION)/* $(@D)
	$(RM) -r $(@D)/pycryptodome-$(PYTHON_PYCRYPTODOME_VERSION)
endef

$(eval $(python-package))
