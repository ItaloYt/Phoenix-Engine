cflags := -O0 -g -fno-omit-frame-pointer -Wall -Werror -fpic -Iinclude

src := \
	src/window/glfw/window.c \
	src/vulkan/instance.c \
	src/vulkan/physical_devices.c \
	src/vulkan/surface_formats.c \
	src/vulkan/present_modes.c \
	src/vulkan/device.c \
	src/vulkan/queue.c \
	src/vulkan/swapchain.c \
	src/vulkan/images.c \
	src/vulkan/image_views.c \
	src/vulkan/shader_module.c \
	src/vulkan/pipeline_layout.c \
	src/vulkan/render_pass.c \
	src/vulkan/framebuffers.c \
	src/vulkan/command_pool.c \
	src/vulkan/command_buffer.c \
	src/vulkan/semaphore.c \
	src/vulkan/fence.c \
	src/vulkan/raw_buffer.c \
	src/vulkan/device_memory.c \
	src/vulkan/buffer.c \
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
