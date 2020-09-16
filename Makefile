disco:
	mbed compile -m DISCO_ORIGINAL --color --profile=release --profile=./profiles/leka.json

disco_flash:
	$(MAKE) disco
	cp build/DISCO_ORIGINAL/GCC_ARM-RELEASE/LekaOS.bin /Volumes/DIS_F769NI

leka:
	mbed compile -m LEKA_V1.0_DEV --color --profile=release --profile=./profiles/leka.json

leka_flash:
	$(MAKE) leka
	# to do

term:
	mbed sterm

vs:
	mv Makefile tmpMakefile
	mbed-vscode-generator -m disco_f769ni
	mv tmpMakefile Makefile

clone_mbed:
	rm -rf ./lib/_vendor/mbed-os
	git clone --depth=1 --branch=mbed-os-6.3.0 https://github.com/ARMmbed/mbed-os ./lib/_vendor/mbed-os
