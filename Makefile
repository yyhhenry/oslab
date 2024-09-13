all:
	(cd linux-0.11; make)

clean:
	(cd linux-0.11; make clean)

BootImage:
	(cd linux-0.11; make BootImage)
