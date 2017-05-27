# webrockets

Webrockets is an #unconf17 project that implements a basic websocket listener in R. The implementation draws heavily on @hrbmstr's wrapper of `easywsclient` in https://github.com/ropenscilabs/decapitated.

# Example
This example plots coordinate pairs streamed from a [server](https://github.com/ropenscilabs/webrockets/blob/master/tests/random_coordintate_server.R) using the websocket protocol. Points are stored in a shiny `reactiveValues` object which cause shiny to trigger an update of the plot when new data is received.

![Blastoff!](https://raw.githubusercontent.com/ropenscilabs/webrockets/master/inst/media/example1.gif)

```   
library(shiny)
library(ggplot2)
library(jsonlite)

#Create a connection to a server sending x,y json pairs
con <- ws_connect("ws://localhost:5006/")

# Define the Shiny UI
ui <- fluidPage(
    plotOutput('plot')
)


# Define the server code
server <- function(input, output) {
    values <- reactiveValues(x = NULL, y = NULL)

    observeEvent(invalidateLater(100), {

        new_response <- ws_receive(con, 0)
        if (new_response != ""){
            new_point <- fromJSON(new_response)
            values$x <- c(values$x, new_point$x)
            values$y <- c(values$y, new_point$y)
        }
    }, ignoreNULL = FALSE)

    output$plot <- renderPlot({
        ggplot(data.frame(xval = values$x, yval = values$y),
               aes(x = xval, y=yval)) + geom_point()
    })
}

# Return a Shiny app object
shinyApp(ui = ui, server = server)
```

# API

## Opening a connection
`con <- ws_connect(url = "ws://localhost:5006/")`

## Checking for messages

### Blocking
`ws_poll(ws_ptr = con, timeout = 5)` 

### Blocking forever
`ws_receive(ws_ptr = con, timeout = -1)` should block till a message arrives.

### Non-blocking
`ws_receive(ws_ptr = con, timeout = 0)` will get a message if there is one, return empty string if not.

### Getting one message
`ws_receive_one(ws_ptr = con, frequency = 20)` will get 1 message.

### Getting N messages
`ws_receive_multiple(ws_ptr = con, nevents = 20)` will get 20 messages.


# TODO
* Vignette
* Functional close method for closing connection
* Automated tests
