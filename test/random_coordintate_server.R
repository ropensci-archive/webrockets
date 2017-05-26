library(httpuv)
library(jsonlite)

app <- list(
    call = function(req) {
        stop("call is not implemented. Something is wrong if you see this")
    },
    onWSOpen = function(ws) {
        id = 1
        while(TRUE){
            x = rnorm(1, 10, 2.5)
            y = rnorm(1, 10, 2.5)
            ws$send(jsonlite::toJSON(list("x" = x, "y" = y)))
            Sys.sleep(1)
            id = id+1
        }
    }
)
runServer("0.0.0.0", 5006, app, 250)

