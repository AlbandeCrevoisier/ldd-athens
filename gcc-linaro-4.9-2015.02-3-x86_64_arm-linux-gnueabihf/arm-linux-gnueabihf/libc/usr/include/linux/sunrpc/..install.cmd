cmd_/home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/sunrpc/.install := /bin/bash scripts/headers_install.sh /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/sunrpc ./include/uapi/linux/sunrpc debug.h; /bin/bash scripts/headers_install.sh /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/sunrpc ./include/linux/sunrpc ; /bin/bash scripts/headers_install.sh /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/sunrpc ./include/generated/uapi/linux/sunrpc ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/sunrpc/$$F; done; touch /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/sunrpc/.install
