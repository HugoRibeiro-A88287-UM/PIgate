PROJ_NAME = PIgate
RASPIP = 10.42.0.114

SRC_FOLDER = src
RELAY_FOLDER = DDrelay
LEDRGB_FOLDER = DDleds
DOCS_FOLDER = docs
FIREBASE_FILE = Firebase/firebase.py
DOXYGEN = /usr/local/bin/doxygen

.PHONY: all
all: PIgateFolder BuildPIgateSystem documentation transferPIgateSystem clean sendPIgateToRasp
	
PIgateFolder:
	mkdir -p $(PROJ_NAME)

BuildPIgateSystem: 
	$(MAKE) -C $(SRC_FOLDER)
	$(MAKE) -C $(RELAY_FOLDER)
	$(MAKE) -C $(LEDRGB_FOLDER)

transferPIgateSystem:
	cp -t $(PROJ_NAME) $(DOCS_FOLDER)/PIgateID.txt $(FIREBASE_FILE) $(SRC_FOLDER)/PIgateProgram.elf
	cp -t $(PROJ_NAME) $(RELAY_FOLDER)/relay.ko $(LEDRGB_FOLDER)/ledRGB.ko
	cp -t $(PROJ_NAME) $(DOCS_FOLDER)/config

sendPIgateToRasp:
	scp -r $(PROJ_NAME) root@$(RASPIP):/etc
	scp $(DOCS_FOLDER)/eng.mypatterns root@$(RASPIP):/usr/share/tessdata
	scp $(DOCS_FOLDER)/cascade.xml root@$(RASPIP):/usr/share/OpenCV/haarcascades

documentation:
	$(DOXYGEN) Doxyfile

.PHONY: clean 
clean: 
	$(MAKE) -C $(SRC_FOLDER) clean
	$(MAKE) -C $(LEDRGB_FOLDER) clean
	$(MAKE) -C $(RELAY_FOLDER) clean




	