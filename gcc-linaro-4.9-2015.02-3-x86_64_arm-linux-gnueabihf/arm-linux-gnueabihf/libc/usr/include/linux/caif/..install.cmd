cmd_/home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/caif/.install := /bin/bash scripts/headers_install.sh /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/caif ./include/uapi/linux/caif caif_socket.h if_caif.h; /bin/bash scripts/headers_install.sh /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/caif ./include/linux/caif ; /bin/bash scripts/headers_install.sh /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/caif ./include/generated/uapi/linux/caif ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/caif/$$F; done; touch /home/buildslave/workspace/BinaryRelease/label/hetzner/target/arm-linux-gnueabihf/_build/sysroots/arm-linux-gnueabihf/usr/include/linux/caif/.install
