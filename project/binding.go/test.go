package main

/*
void do_birth();
void do_say();
void do_death();
#cgo LDFLAGS: -ldl
*/
import "C"

func main() {
	C.do_birth()
	C.do_say()
	C.do_death()
}
