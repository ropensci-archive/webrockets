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

//' Connect to Chrome Remote
//'
//' @param url ws URL
//' @export
// [[Rcpp::export]]
SEXP chrome_connect(std::string url) {

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

//' Instrument Chrome
//'
//' @export
// [[Rcpp::export]]
std::string instrument(SEXP ws_ptr, std::string cmd) {

  //int i=0;

  chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));

  ready = false;

  wsp->ws->send(cmd);

  // while (!ready) {
  //   i++;
  //   if((i % 100) == 0) Rcpp::checkUserInterrupt();
    // std::cout << "." ;
  wsp->ws->poll(-1);
  wsp->ws->dispatch(handle_message);
  // }

  // std::cout<<std::endl;

  return(msg);

}

//' Consume event
//'
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

// [[Rcpp::export]]
std::vector<std::string> ws_poll_list(SEXP ws_ptr, unsigned int eventlimit){
    chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));

    int nevents = 0;
    std::vector<std::string> out;
    while (nevents < eventlimit) {
        Rcpp::Rcout << "nevents = " << nevents << std::endl;
        wsp->ws->poll(-1);
        wsp->ws->dispatch(handle_message);
        nevents++;
        out.push_back(msg);
        msg = "";
    }

    std::cout<<std::endl;
    return out;
}


//' Close a socket connection
//'
//' @param Chrome ws ptr object.
//' @export
// [[Rcpp::export]]
void close(SEXP ws_ptr){
    chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));
    wsp->ws->close();
    //DO WE NEED TO DELETE
    //delete wsp->ws;
    //delete wsp;
}

//' Consume 1 event
//'
//' @export
// [[Rcpp::export]]
std::string ws_read_one(SEXP ws_ptr, int timeout=5) {
    msg = "";
    chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));
    std::cout << "." ;
    wsp->ws->poll(timeout);
    wsp->ws->dispatch(handle_message);
    std::cout<<std::endl;

    return(msg);

}

//' Consume 1 event and return a distinctive message if no message was recieved
//' Testing use only. To be removed.
//'
//' @export
// [[Rcpp::export]]
std::string ws_test_read_one(SEXP ws_ptr, int timeout=5) {
    msg = "";
    chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));
    std::cout << "." ;
    wsp->ws->poll(timeout);
    wsp->ws->dispatch(handle_message);
    std::cout<<std::endl;
    if(msg == ""){
        msg = "NO_MESSAGE_FOUND";
        }
    return(msg);
}
