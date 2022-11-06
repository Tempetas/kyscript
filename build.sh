gcc main.c -Os -flto -fuse-ld=gold -o kyscript

strip kyscript -s -R .comment -R .gnu.version --strip-unneeded

./kyscript examples/add.kyscript 3242 3243
