################################################################################
#
# python-pyrebase4
#
################################################################################

PYTHON_PYREBASE4_VERSION = 4.5.0
PYTHON_PYREBASE4_SOURCE = Pyrebase4-$(PYTHON_PYREBASE4_VERSION).tar.gz
PYTHON_PYREBASE4_SITE = https://files.pythonhosted.org/packages/00/cd/7b05f0c2808cfcdb824175c033e253ab7511656ee9f78112eac50035418a
PYTHON_PYREBASE4_SETUP_TYPE = setuptools

$(eval $(python-package))
