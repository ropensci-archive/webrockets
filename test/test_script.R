#bokeh serve .python_test_server/circle_server.py --session-ids unsigned

#create connection
cc <- webrockets::chrome_connect("ws://localhost:5006/circle_server/ws?bokeh-session-id=test&bokeh-protocol-version=1.0")

#poll the connection
noise <- webrockets::ws_poll(cc)

#close the connection
webrockets::close(cc)

#check for more data
more_noise <- webrockets::ws_poll(cc)
