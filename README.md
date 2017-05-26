# webrockets

Webrockets is an #unconf17 project that implements a basic websocket listener in R. The implementation draws heavily on @hrbmstr's wrapper of `easywsclient` in https://github.com/ropenscilabs/decapitated.

# Example
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

        new_response <- ws_read_one(con, 0)
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

# Usage


# TODO
* Vignette

