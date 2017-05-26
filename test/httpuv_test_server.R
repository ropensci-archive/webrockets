library(httpuv)

app <- list(
    call = function(req) {
        stop("call is not implemented. Something is wrong if you see this")
    },
    onWSOpen = function(ws) {
        id = 1
        while(TRUE){
                ws$send(paste0("TEST MESSAGE",id))
                Sys.sleep(5)
                id = id+1
            }
    }
)
runServer("0.0.0.0", 5006, app, 250)
