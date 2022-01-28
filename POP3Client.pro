TEMPLATE = subdirs
SUBDIRS += POP3ClientApp \
    POP3ClientTest \

POP3ClientTest.depends = POP3ClientApp
