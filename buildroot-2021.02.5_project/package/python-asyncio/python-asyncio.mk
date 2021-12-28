################################################################################
#
# python-asyncio
#
################################################################################

PYTHON_ASYNCIO_VERSION = 3.4.3
PYTHON_ASYNCIO_SOURCE = asyncio-$(PYTHON_ASYNCIO_VERSION).tar.gz
PYTHON_ASYNCIO_SITE = https://files.pythonhosted.org/packages/da/54/054bafaf2c0fb8473d423743e191fcdf49b2c1fd5e9af3524efbe097bafd
PYTHON_ASYNCIO_SETUP_TYPE = setuptools
PYTHON_ASYNCIO_LICENSE = Apache-2.0
PYTHON_ASYNCIO_LICENSE_FILES = COPYING

$(eval $(python-package))
