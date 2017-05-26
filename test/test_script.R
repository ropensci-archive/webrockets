#create connection
cc <- webrockets::chrome_connect("ws://localhost:5006/")

#poll the connection
noise <- webrockets::ws_poll(cc)

webrockets::ws_test_read_one(cc)

#close the connection
webrockets::close(cc)

#check for more data
more_noise <- webrockets::ws_poll(cc)

noise <- webrockets::ws_poll(cc, -1)
noise <- webrockets::ws_poll(cc, 0)
