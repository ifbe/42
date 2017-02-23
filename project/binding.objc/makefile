all:
	gcc test.m \
	-I `gnustep-config --variable=GNUSTEP_SYSTEM_HEADERS` \
	-L `gnustep-config --variable=GNUSTEP_SYSTEM_LIBRARIES` \
	-lobjc -lgnustep-base -fconstant-string-class=NSConstantString \
	-D_NATIVE_OBJC_EXCEPTIONS
