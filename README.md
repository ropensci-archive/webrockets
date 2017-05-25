# webrockets
Webrockets is an #unconf17 project to explore implementing a websocket listener in R.

The usecase we are targetting is to caputure streaming data and have a live view of that data updated in Shiny. 

We are starting from @hrbmstr's implementation in https://github.com/ropenscilabs/decapitated


TODO:
* Parse JSON returned
* close method
* Reactive shiny front end
* Vignette
* Fix: polling closed connection twice creates infinite loop.
* Fix: 404 terminates RStudio ungraciously.