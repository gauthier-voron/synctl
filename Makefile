OBJ := obj/
BIN := bin/


sources := common/AdapterInputStream common/AdapterOutputStream \
           common/Directory \
           common/EOFException common/FileInputStream common/FileOutputStream \
           common/FirstMatchFilter common/FSException common/GlobPattern \
           common/HashOutputStream common/InputStream common/IOException \
           common/OutputStream \
           common/Reference
sources += ui/main
sources += client/EntryFactory client/DirectoryV1 client/RegularV1 \
           client/SendContext client/Sender client/SymlinkV1
sources += server/Branch server/BranchStore server/DirectoryV1 \
           server/ObjectFactory \
           server/ObjectStore \
           server/RegularV1 \
           server/Repository server/Snapshot server/SymlinkV1 \
           server/TransientOutputStream

objects := $(patsubst %, $(OBJ)%.o, $(sources))


check: $(BIN)synctl
	./$< client | ./$< server

all: $(BIN)synctl


$(BIN)synctl: $(objects) | $(BIN)
	g++ $^ -o $@ -lssl -lcrypto


$(OBJ)common/%.o: src/common/%.cxx | $(OBJ)common
	g++ -Wall -Wextra -g -c $< -o $@ -Iinclude/

$(OBJ)ui/%.o: src/ui/%.cxx | $(OBJ)ui
	g++ -Wall -Wextra -g -c $< -o $@ -Iinclude/

$(OBJ)client/%.o: src/client/%.cxx | $(OBJ)client
	g++ -Wall -Wextra -g -std=c++17 -c $< -o $@ -Iinclude/ -include iostream

$(OBJ)server/%.o: src/server/%.cxx | $(OBJ)server
	g++ -Wall -Wextra -g -std=c++17 -c $< -o $@ -Iinclude/ -include iostream


$(OBJ)common $(OBJ)client $(OBJ)server $(OBJ)ui: | $(OBJ)
	mkdir $@

$(OBJ) $(BIN):
	mkdir $@


clean:
	-rm -rf $(OBJ) $(BIN)
