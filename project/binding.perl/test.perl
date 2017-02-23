#!/usr/bin/perl
use Inline C => Config => MYEXTLIB => './42.so';
use strict;

print "before:\n";
my return_c_so();
print "after\n";

__END__
__C__
#include 
#include 
int return_c_so (int z) {
	birth();
	death();
	return 0;
}
