CPPFLAGS=`root-config --cflags` -I/usr/local/root/include
LDFLAGS=`root-config --libs` -lMinuit
Skimmer: Skimmer.cc
	g++ $(CPPFLAGS) $(LDFLAGS) -o $@ $^
