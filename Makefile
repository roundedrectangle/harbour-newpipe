SUBDIRS := cpp

HEADERS = appwrapper_dynamic.h appwrapper.h graal_isolate_dynamic.h graal_isolate.h
LIBS = appwrapper.so
HEADERS_FROM = ./java/NewPipeExtractor/appwrapper/build/native/nativeCompile/
HEADERS_TO = ./cpp/include/
LIBS_FROM = ./java/NewPipeExtractor/appwrapper/build/native/nativeCompile/
LIBS_TO = ./cpp/lib/

all: $(addprefix $(HEADERS_TO),$(HEADERS)) $(addprefix $(LIBS_TO),$(LIBS)) $(SUBDIRS)

$(addprefix $(HEADERS_TO),$(HEADERS)): $(addprefix $(HEADERS_FROM),$(HEADERS))
	mkdir -p $(@D)
	cp $? $(@D)

$(addprefix $(LIBS_TO),$(LIBS)): $(addprefix $(LIBS_FROM),$(LIBS))
	mkdir -p $(@D)
	cp $? $(@D)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all clean $(SUBDIRS)

SUBCLEAN = $(addsuffix .clean,$(SUBDIRS))

clean: $(SUBCLEAN)
	-rm -f $(addprefix $(HEADERS_TO),$(HEADERS))
	-rm -f $(addprefix $(LIBS_TO),$(LIBS))

$(SUBCLEAN): %.clean:
	$(MAKE) -C $* clean

run: all
	LD_LIBRARY_PATH=$(LIBS_TO) ./cpp/main
