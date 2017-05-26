#bokeh serve .python_test_server/circle_server.py --session-ids unsigned

#create connection
cc <- webrockets::ws_connect("ws://localhost:5006/")

#poll the connection
noise <- webrockets::ws_poll(cc)

webrockets::ws_test_read_one(cc)

#close the connection
webrockets::close(cc)

#check for more data
more_noise <- webrockets::ws_poll(cc)

noise <- webrockets::ws_poll(cc, -1)
noise <- webrockets::ws_poll(cc, 0)

# Make plot
library(ggplot2)
p <- ggplot()
i <- 0

while (i < 5){
    new_point <- fromJSON(ws_poll(cc, -1))
    p <- p + annotate(geom = "point", x = new_point$x, y = new_point$y)
    print(new_point)
    print(p)
    i = i + 1
}
