library(shiny)
library(ggplot2)
library(jsonlite)
library(webrockets)
library(magrittr)
library(tidyr)

#con <- ws_connect("ws://localhost:5006/")
# Define the UI
ui <- fluidPage(
    
    tags$style(type="text/css",
               ".recalculating {opacity: 1.0;}"),
    #style change stops the plot from "flashing" on slower PCs while data is updated.
    plotOutput('plot')
)


# Define the server code
server <- function(input, output) {
    values <- reactiveValues(x = NULL, y = NULL, z = NULL, time = NULL)

    observeEvent(invalidateLater(50), {

        new_response <- ws_receive(con, 0)
        if (new_response != ""){
            new_point <- fromJSON(new_response)
            if (new_point$sensor == "accel"){
                values$x <- c(values$x, new_point$values$x)
                values$y <- c(values$y, new_point$values$y)
                values$z <- c(values$z, new_point$values$z)
                values$time <- c(values$time, length(values$y))
            }

        }
    }, ignoreNULL = FALSE)

    output$plot <- renderPlot({
        data.frame(xval = values$x, yval = values$y, zval = values$z, time = values$time) %>%
            gather("variable", "value", -time) %>%
        ggplot(aes(x = time, y=value)) + geom_path() + facet_grid(~variable)
    })
}

# Return a Shiny app object
shinyApp(ui = ui, server = server)
