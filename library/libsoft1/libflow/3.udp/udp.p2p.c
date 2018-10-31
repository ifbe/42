//step1:
//	server.generate(pairkey)
//	a.send(pairkey)->server, b.send(pairkey)->server

//step2:
//	server.send(b.addr)->a, server.send(a.addr)->b
//	a.send(msg)->b, b.send(msg)->a

//step3:
//	a<-->b
