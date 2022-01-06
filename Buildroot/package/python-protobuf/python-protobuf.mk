################################################################################
#
# python-protobuf
#
################################################################################

PYTHON_PROTOBUF_VERSION = 3.19.1
PYTHON_PROTOBUF_SOURCE = protobuf-$(PYTHON_PROTOBUF_VERSION).tar.gz
PYTHON_PROTOBUF_SITE = https://files.pythonhosted.org/packages/37/52/4e40f7513b44671817a92dc566f4a6e8eba65bfc94f79da23186e6c127ce
PYTHON_PROTOBUF_SETUP_TYPE = setuptools
PYTHON_PROTOBUF_LICENSE = FIXME: license id couldn't be detected
PYTHON_PROTOBUF_LICENSE_FILES = LICENSE

$(eval $(python-package))
