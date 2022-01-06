################################################################################
#
# python-jwt
#
################################################################################

PYTHON_JWT_VERSION = 3.3.1
PYTHON_JWT_SOURCE = python_jwt-$(PYTHON_JWT_VERSION).tar.gz
PYTHON_JWT_SITE = https://files.pythonhosted.org/packages/f9/bd/45716038cf4aa993294bb5db6b5acbd057e97e170ece8e9399f60d15aa54
PYTHON_JWT_SETUP_TYPE = setuptools
PYTHON_JWT_LICENSE = MIT
PYTHON_JWT_LICENSE_FILES = LICENCE

$(eval $(python-package))
