package main

/*
void do_birth();
void do_death();
#cgo LDFLAGS: -ldl
*/
import "C"

func main() {
	C.do_birth()
	C.do_death()
}
