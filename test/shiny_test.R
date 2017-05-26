library(shiny)
library(ggplot2)
library(jsonlite)

# Define the UI
ui <- fluidPage(
    plotOutput('plot')
)


# Define the server code
server <- function(input, output) {

    cc <- webrockets::chrome_connect("ws://localhost:5006/")
    values <- reactiveValues(x = NULL, y = NULL)

    observeEvent(invalidateLater(100), {

        new_response <- ws_read_one(cc, 0)
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
