cflags := -O0 -g -fno-omit-frame-pointer -Wall -Werror -fpic -Iinclude

src := \
	src/window.c \
	src/renderer.c \
	src/renderer/vulkan/instance.c \
	src/renderer/vulkan/physical_devices.c \
	src/renderer/vulkan/device.c \
	src/renderer/vulkan/queue.c \
	src/renderer/vulkan/surface.c \
	src/renderer/vulkan/swapchain.c \
	src/renderer/vulkan/images.c \
	src/renderer/vulkan/image_views.c \
	src/renderer/vulkan/shader_module.c \
	src/renderer/vulkan/pipeline_layout.c \
	src/renderer/vulkan/render_pass.c \
	src/renderer/vulkan/pipeline.c \
	src/renderer/vulkan/framebuffers.c \
	src/renderer/vulkan/command_pool.c \
	src/renderer/vulkan/command_buffer.c \
	src/renderer/vulkan/semaphore.c \
	src/renderer/vulkan/fence.c \
	src/util/resource.c \
	src/util/resources.c \
	src/util/plataform/unix/file.c

# src/renderer/vulkan/pipeline.c \

obj := $(src:.c=.o)
deps := $(src:.c=.d)

shaders := \
	shaders/base.vert \
	shaders/base.frag

spirv := $(addsuffix .spirv,$(shaders))

.PHONY: compile
compile: libphoenix.so libphoenix.a

.PHONY: clear
clear:
	@#bundler src/bundler.o src/static.o src/bundler.d
	@rm -rf libphoenix.so libphoenix.a bundler $(obj) src/bundler.o $(spirv) $(deps) src/bundler.d 

bundler:
	@echo "Linking $@"
	@clang $(cflags) -MMD -MF src/bundler.d -o $@ src/bundler.o src/util/plataform/unix/file.o

%.o: %.c
	@echo "Compiling source '$<'"
	@clang $(cflags) -MMD -MF $(@:.o=.d) -c -o $@ $<

%.vert.spirv: %.vert
	@echo "Compiling shader '$<'"
	@glslc -o $@ $<

%.frag.spirv: %.frag
	@echo "Compiling shader '$<'"
	@glslc -o $@ $<

src/util/resources.c:
	@echo "Generating source '$@'"
	@./bundler $@ resources.csv

libphoenix.so: 
	@echo "Linking $@"
	@#src/static.o
	@clang -shared -o $@ $(obj) 

libphoenix.a:
	@echo "Linking $@"
	@#src/static.o
	@ar rcs $@ $(obj) 

# DEPENDENCIES
bundler: src/bundler.o src/util/plataform/unix/file.o

src/util/resources.c: bundler $(spirv)

#src/static.o
libphoenix.so: src/util/resources.c $(obj)

#src/static.o
libphoenix.a: src/util/resources.c $(obj)

-include $(deps)
