osc-get: osc-get.o
	$(CC) -ljson-c -pedantic -Wall -Wextra osc-get.o -o osc-get

appimagetool-x86_64.AppImage:
	wget https://github.com/AppImage/AppImageKit/releases/download/12/appimagetool-x86_64.AppImage
	chmod +x appimagetool-x86_64.AppImage

osc-get-x86_64.AppImage: osc-get appimagetool-x86_64.AppImage
	mkdir -p osc-get.AppDir/usr/
	mkdir -p osc-get.AppDir/usr/bin/
	mkdir -p osc-get.AppDir/usr/lib/
	cp osc-get osc-get.AppDir/usr/bin/
	cp $(shell ldd osc-get | grep json | cut -d ' ' -f  3) osc-get.AppDir/usr/lib/
	./appimagetool-x86_64.AppImage osc-get.AppDir
