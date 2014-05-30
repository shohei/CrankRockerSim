COMPILE = g++  
LIBS = `wx-config --libs` 
FLAGS = `wx-config --cxxflags`
SOURCE = *.cpp
OBJ = $(SOURCE:.cpp=.o)
ICONFILE = cba.icns
BUNDLE = simulator.app
BUNDLESIGNATURE = ????
MACPACKAGEINFO=$(BUNDLE)/Contents/PkgInfo

all: $(OBJ)
	$(COMPILE) $(LIBS) $(FLAGS) -o main $(OBJ)
.cpp.o:
	$(COMPILE) $(LIBS) $(FLAGS) -c $<  
.PHONY: clean
clean:
	rm -f main $(OBJ)
hoge:	
	#cp version.plist YourApp.app/Contents/
	#cp cba.icns AnotherResource.txt YourApp.app/Contents/Resources/

main.app: Info.plist main InfoPlist.strings cba.icns 
	SetFile -t APPL main 
	-mkdir main.app    
	-mkdir main.app/Contents
	-mkdir main.app/Contents/MacOS
	-mkdir main.app/Contents/Resources
	-mkdir main.app/Contents/Resources/English.lproj
	cp Info.plist main.app/Contents/
	cp InfoPlist.strings main.app/Contents/Resources/English.lproj/
	echo -n 'APPL????' > main.app/Contents/PkgInfo
	cp main YourApp.app/Contents/MacOS/main
	cp cba.icns main.app/Contents/Resources/


simulator.app: Info.plist $(ICONFILE)
	@echo " Building: $(BUNDLE)"
	mkdir -p $(BUNDLE)/Contents
	mkdir -p $(BUNDLE)/Contents/MacOS
	mkdir -p $(BUNDLE)/Contents/Resources
	cp Info.plist $(BUNDLE)/Contents/Info.plist
	echo -n "APPL$(BUNDLESIGNATURE)" > $(MACPACKAGEINFO)
#    cp -f $(ICONFILE) $(BUNDLE)/Contents/Resources/$(ICONFILE)
	cp -f $(ICONFILE) $(BUNDLE)/Contents/Resources/wxmac.icns
