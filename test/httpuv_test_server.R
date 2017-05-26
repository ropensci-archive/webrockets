library(httpuv)

app <- list(
    call = function(req) {
        stop("call is not implemented. Something is wrong if you see this")
    },
    onWSOpen = function(ws) {
        messages_id <- 1:10
        lapply(messages_id,
            function(id){
                ws$send(paste0("TEST MESSAGE",id))
                Sys.sleep(5)
            })
    }
)
runServer("0.0.0.0", 5006, app, 250)
