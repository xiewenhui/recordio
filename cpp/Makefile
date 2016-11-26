WORKROOT = ../../../../../../../
INCLUDES =  -I$(WORKROOT) \
					-I$(WORKROOT)/third-64/snappy/include/  \
					-I$(WORKROOT)/third-64/boost/include/  \
					-I$(WORKROOT)/third-64/boost/include/boost/tr1/  \
					-I$(WORKROOT)/third-64/glog/include/   \
					-I$(WORKROOT)/third-64/gtest/include/   \
					-I$(WORKROOT)/third-64/protobuf/include/   \
					-I./

LIBS    	  =  \
				-L$(WORKROOT)/third-64/glog/lib -lglog \
				-L$(WORKROOT)/third-64/gtest/lib -lgtest  -lgtest_main\
				-L$(WORKROOT)/third-64/protobuf/lib -lprotobuf \
				-L$(WORKROOT)/third-64/snappy/lib/  -lsnappy  \
			  -lpthread -lrt -ldl



CPPFLAGS = -g -O2 -DHAVE_NETINET_IN_H -Wall -Wextra -Wno-unused -pipe -fpermissive  $(INCLUDES)
CC       = g++ $(CPPFLAGS)

MODULE_NAME =  recordio_test
OBJ = file.o join.o stringpiece.o  stringprintf.o recordio.o

all : protofile $(MODULE_NAME)

$(MODULE_NAME) : $(OBJ) document.pb.o recordio_test.o
	$(CC) -o $@ $^ $(LIBS)

protofile:
	../../../../../../../third-64/protobuf/bin/protoc --cpp_out=.  document.proto
	$(CC) -c document.pb.cc -o document.pb.o

%.o	: %.cc
	$(CC) -c $< -o $@
%.o	: %.cpp
	$(CC) -c $< -o $@
	
clean:
	rm -rf  $(MODULE_NAME)
	rm -rf ./*.o


