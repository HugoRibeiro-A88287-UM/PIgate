################################################################################
#
# python-pyrebase
#
################################################################################

PYTHON_PYREBASE_VERSION = 3.0.27
PYTHON_PYREBASE_SOURCE = Pyrebase-$(PYTHON_PYREBASE_VERSION).tar.gz
PYTHON_PYREBASE_SITE = https://files.pythonhosted.org/packages/3a/41/eb969d9e524c1d21fc3460b3f1efbae604d02861c472e6eaaf39a4f8df6a
PYTHON_PYREBASE_SETUP_TYPE = setuptools

$(eval $(python-package))
