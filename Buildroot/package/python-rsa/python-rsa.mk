################################################################################
#
# python-rsa
#
################################################################################

PYTHON_RSA_VERSION = 4.8
PYTHON_RSA_SOURCE = rsa-$(PYTHON_RSA_VERSION).tar.gz
PYTHON_RSA_SITE = https://files.pythonhosted.org/packages/8c/ee/4022542e0fed77dd6ddade38e1e4dea3299f873b7fd4e6d78319953b0f83
PYTHON_RSA_SETUP_TYPE = setuptools
PYTHON_RSA_LICENSE = FIXME: license id couldn't be detected
PYTHON_RSA_LICENSE_FILES = LICENSE

$(eval $(python-package))
