#gv files automatically created with ~/Projects/MARTe2CfgToGraphviz/Build/linux/Source/CfgToDot.ex ~/Projects/Fast-Control-Falcon/Configurations/BareConfiguration-2017_05_31.cfg -o FalconFastControl

OUTPUT=png
IMAGES:= $(patsubst %.gv,%.$(OUTPUT),$(wildcard *.gv))

%.$(OUTPUT): %.gv
	dot -T$(OUTPUT) $< -o $@

all: $(IMAGES)

clean:
	rm $(IMAGES)
