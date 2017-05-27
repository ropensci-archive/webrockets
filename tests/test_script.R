#create connection
cc <- webrockets::ws_connect("ws://localhost:5006/")

#poll the connection
noise <- webrockets::ws_receive_one(cc)

#close the connection -- DOES NOT WORK
# webrockets::close(cc)

#check for more data
more_noise <- webrockets::ws_receive_multiple(cc)

noise <- webrockets::ws_receive_one(cc, -1)
noise <- webrockets::ws_receive_one(cc, 0)

# Make plot
library(ggplot2)
p <- ggplot()
i <- 0

while (i < 5){
    new_point <- fromJSON(ws_receive_one(cc, -1))
    p <- p + annotate(geom = "point", x = new_point$x, y = new_point$y)
    print(new_point)
    print(p)
    i = i + 1
}
