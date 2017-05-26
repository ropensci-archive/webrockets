#include <Rcpp.h>
using namespace Rcpp;

#include "easywsclient.hpp"
#include "ws.h"

//std::mutex mtx;
//std::condition_variable cv;
std::string msg;
bool ready = false;

void handle_message(const std::string & message) {
  //std::unique_lock<std::mutex> lock(mtx);
  //lock.unlock();
  msg = message;
  ready = true;
  //lock.lock();
  //cv.notify_one();
}

//' Connect to websocket
//'
//' @param url ws URL
//' @export
// [[Rcpp::export]]
SEXP ws_connect(std::string url) {

  chromeWsPtr x = new(chromeWs);
  x->ws = easywsclient::WebSocket::from_url(url);
  x->response = std::string("Hi");
  x->ready = false;

  if (!x->ws){
      throw std::invalid_argument("Invalid URL");
  }

  while(x->ws->getReadyState() != easywsclient::WebSocket::OPEN) {
    Rcpp::checkUserInterrupt();
  }

  return(XPtrWs(x));

};


//' Consume one message
//'
//' @param ws_ptr pointer from \{code}\link{ws_connect}
//' @param timeout
//' @export
//' @return list of strings with each message
//' @examples
//'
//' \dontrun{
//'     con <- ws_connect(ws://localhost:5006/") # Need to have websocket server
//'     ws_poll(con, 5) # returns one message
//' }
//' @export
// [[Rcpp::export]]
std::string ws_poll(SEXP ws_ptr, int timeout=5) {

  chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));

  ready = false;

  while (!ready) {
    std::cout << "." ;
    wsp->ws->poll(timeout);
    wsp->ws->dispatch(handle_message);
  }

  std::cout<<std::endl;
  std::string out = msg;
  msg = "";
  ready = false;

  return(out);

}

//' Consume n events
//' @param ws_ptr pointer from \{code}\link{ws_connect}
//' @param eventlimit limit of events
//' @return list of strings with each message
//' @examples
//'
//' \dontrun{
//'     con <- ws_connect(ws://localhost:5006/") # Need to have websocket server
//'     ws_poll_list(con, 3) # returns 3 messages
//' }
//' @export
// [[Rcpp::export]]
std::vector<std::string> ws_poll_list(SEXP ws_ptr, unsigned int eventlimit){
    chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));

    int nevents = 1;
    std::vector<std::string> out;
    while (nevents <= eventlimit) {
        Rcpp::Rcout << "Events read = " << nevents << std::endl;
        wsp->ws->poll(-1);
        wsp->ws->dispatch(handle_message);
        nevents++;
        out.push_back(msg);
        msg = "";
    }

    std::cout<<std::endl;
    return out;
}


//' ws_close
//'
//' Close a socket connection
//'
//' @param Chrome ws ptr object.
//' @export
// [[Rcpp::export]]
void ws_close(SEXP ws_ptr){
    chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));
    wsp->ws->close();
    //DO WE NEED TO DELETE
    //delete wsp->ws;
    //delete wsp;
}

//' ws_read_one
//'
//' Consume one message within timeout
//'
//' @param ws_ptr pointer from \{code}\link{ws_connect}
//' @param timeout time in milliseconds before timing out, see details
//' @details If timout is set to a negative number, the process should
//' block until a message arrives. If set to 0, no blocking is performed --
//' a message is returned if it is there.  If set to a positive value,
//' the function will wait until there is a message or a timeout
//' @return one message, if one occurs within timeout. otherwise, empty string
//' @examples
//'
//' \dontrun{
//'     con <- ws_connect(ws://localhost:5006/") # Need to have websocket server
//'     ws_read_one(con, 5)
//' }
//' @export
// [[Rcpp::export]]
std::string ws_read_one(SEXP ws_ptr, int timeout=5) {
    msg = "";
    chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));
    wsp->ws->poll(timeout);
    wsp->ws->dispatch(handle_message);
    std::cout<<std::endl;

    return(msg);

}

