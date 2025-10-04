CC = g++

TARGET = out/Vault

SRC_DIR = src
INC_DIR = include
STYLE_DIR = style
IMAGES_DIR = images

SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cpp")

INCLUDES = \
    -I$(INC_DIR) \
    -I/usr/include/gtk-3.0 \
    -I/usr/include/gtk-layer-shell \
    -I/usr/include/glib-2.0 \
    -I/usr/lib/glib-2.0/include \
    -I/usr/include/pango-1.0 \
    -I/usr/include/cairo \
    -I/usr/include/harfbuzz \
    -I/usr/include/gdk-pixbuf-2.0 \
    -I/usr/include/atk-1.0

LIB_DIRS = -L/usr/lib

LIBS = \
    -lgtk-3 \
    -lgtk-layer-shell \
    -lgobject-2.0 \
    -lglib-2.0 \
    -lpangocairo-1.0 \
    -lpango-1.0 \
    -latk-1.0 \
    -lcairo \
    -lgdk-3 \
    -lgdk_pixbuf-2.0 \
    -lfontconfig \
    -lfreetype \
    -lX11 \
    -lpthread \
    -lm

CFLAGS = -Wall -Wextra -O2 $(INCLUDES)

CSS_FILES := $(shell find $(STYLE_DIR) -type f -name "*.css")
IMAGE_FILES := $(shell find $(IMAGES_DIR) -type f -name "*.png")

$(TARGET): $(SOURCES) $(CSS_FILES) $(IMAGE_FILES)
	@mkdir -p out/style
	@mkdir -p out/images
	@cp -r $(STYLE_DIR)/* out/style/
	@cp -r $(IMAGES_DIR)/* out/images/
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIB_DIRS) $(LIBS)

clean:
	rm -rf $(TARGET) out/style out/images

.PHONY: clean
